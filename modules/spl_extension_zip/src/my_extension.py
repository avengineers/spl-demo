"""
Requirements:
    - the configuration is in a json file
    - shall pack the binary into a zip file
    - shall also create a byproduct file .7z with the same content
    - the zip file shall contain the binary file and a readme.txt
    - the readme.txt shall contain the config fom the json file
    - shall be able to run from the command line
"""

import sys
from pathlib import Path

# TODO: this is a hack to be able to import the modules from the root folder
sys.path.append(Path(__file__).parent.as_posix())

from dataclasses import dataclass, field
import json
from typing import List
import zipfile
import argparse
from abc import ABC, abstractmethod
from mashumaro.mixins.json import DataClassJSONMixin
from generators.cmake_file import (
    CMakeComment,
    CMakeCustomCommand,
    CMakeCustomTarget,
    CMakeFile,
)


@dataclass
class Variant:
    flavor: str
    subystem: str

    @classmethod
    def from_string(cls, name: str):
        flavor, subsystem = name.replace("\\", "/").split("/")
        return cls(flavor, subsystem)

    def __str__(self) -> str:
        return f"{self.flavor}/{self.subystem}"


@dataclass
class SplPaths:
    def __init__(
        self, project_root_dir: Path, variant: Variant, build_kit: str
    ) -> None:
        self.project_root_dir = project_root_dir
        self.variant = variant
        self.build_kit = build_kit

    @property
    def build_dir(self) -> Path:
        return self.project_root_dir.joinpath(f"build/{self.variant}/{self.build_kit}")


class KConfigArtifacts:
    def __init__(self, spl_paths: SplPaths) -> None:
        self.spl_paths = spl_paths

    @property
    def kconfig_json(self) -> Path:
        return self.spl_paths.build_dir.joinpath("include/generated/autoconf.json")


class LinkerArtifacts:
    def __init__(
        self,
        spl_paths: SplPaths,
        linker_output_file_basename: str,
        linker_output_file_extensions: List[str],
    ) -> None:
        self.spl_paths = spl_paths
        self.linker_output_file_basename = linker_output_file_basename
        self.linker_output_file_extensions = linker_output_file_extensions

    def get_generated_file(self, extension: str) -> Path:
        return self.spl_paths.build_dir.joinpath(
            f"{self.linker_output_file_basename}.{extension}"
        )

    def get_generated_artifacts(self) -> List[Path]:
        return [
            self.get_generated_file(extension)
            for extension in self.linker_output_file_extensions
        ]


class Extension(ABC):
    @abstractmethod
    def generate(self):
        pass

    @abstractmethod
    def run(self):
        pass


@dataclass
class SplExtensionsConfig(DataClassJSONMixin):
    linker_output_file_basename: str = field(
        default="main", metadata={"alias": "LINK_OUT_FILE_BASENAME"}
    )
    linker_output_file_extensions: List[str] = field(
        default_factory=lambda: ["exe"],
        metadata={
            "alias": "LINK_OUT_FILE_EXTENSIONS",
            "deserialize": lambda s: s.split(","),
        },
    )

    @classmethod
    def from_json_file(cls, file_path: Path):
        return cls.from_dict(json.loads(file_path.read_text())["features"])


class BinaryPacker(Extension):
    def __init__(self, spl_paths: SplPaths, spl_config: SplExtensionsConfig):
        self.spl_paths = spl_paths
        self.spl_config = spl_config

    @property
    def generated_cmake_file(self) -> Path:
        return self.spl_paths.build_dir.joinpath("my_extension.cmake")

    def generate(self):
        print(f"Generating cmake file {self.generated_cmake_file}")
        self._generate(self.generated_cmake_file)

    def _generate(self, generated_cmake_file: Path) -> None:
        cmake_file = CMakeFile(generated_cmake_file)
        cmake_file.add_element(CMakeComment("My Extension generated file"))
        linker_output_file_basename = self.spl_config.linker_output_file_basename
        cmake_file.add_element(
            CMakeCustomCommand(
                output=f"${{CMAKE_CURRENT_BINARY_DIR}}/{linker_output_file_basename}.zip",
                command="python ${PROJECT_SOURCE_DIR}/modules/spl_extension_zip/src/my_extension.py --run --project_root_dir ${CMAKE_SOURCE_DIR} --variant ${VARIANT} --build_kit ${BUILD_KIT}",
                depends=[
                    f"${{CMAKE_CURRENT_BINARY_DIR}}/{linker_output_file_basename}.exe",
                    "${PROJECT_SOURCE_DIR}/modules/spl_extension_zip/src/my_extension.py",
                ],
                comment=f"Generating {linker_output_file_basename}.zip",
            )
        )
        cmake_file.add_element(
            CMakeCustomTarget(
                "artifact",
                depends=[
                    f"${{CMAKE_CURRENT_BINARY_DIR}}/{linker_output_file_basename}.zip"
                ],
                all=True,
            )
        )
        cmake_file.to_file()

    def run(self):
        """Archive the linker generated binary and the readme.txt file into a zip file"""
        # get the path to the linker generated binary
        linker_artifacts = LinkerArtifacts(
            self.spl_paths,
            self.spl_config.linker_output_file_basename,
            self.spl_config.linker_output_file_extensions,
        )

        binary_file = linker_artifacts.get_generated_file("exe")

        # create a zip file with the same name as the binary file
        zip_path = binary_file.with_suffix(".zip")
        with zipfile.ZipFile(zip_path, "w") as zip_file:
            for binary_file in linker_artifacts.get_generated_artifacts():
                zip_file.write(str(binary_file), binary_file.name)

        print(f"Created archive: {zip_path} ")


def create_parser():
    parser = argparse.ArgumentParser(
        description="Package binary and readme into zip and 7z files"
    )
    parser.add_argument(
        "--project_root_dir",
        type=Path,
        help="Project root directory",
        default=Path("."),
    )
    parser.add_argument(
        "--variant", type=Variant.from_string, help="Variant name, e.g. SUB/MAIN"
    )
    parser.add_argument("--build_kit", type=str, help="Build kit name, e.g. prod")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--run", action="store_true", help="Run the packer")
    group.add_argument("--generate", action="store_true", help="Generate a cmake file")
    return parser


def main():
    args = create_parser().parse_args()

    spl_paths = SplPaths(args.project_root_dir, args.variant, args.build_kit)
    spl_config = SplExtensionsConfig.from_json_file(
        KConfigArtifacts(spl_paths).kconfig_json
    )

    if args.generate:
        BinaryPacker(spl_paths, spl_config).generate()
    else:
        BinaryPacker(spl_paths, spl_config).run()


if __name__ == "__main__":
    main()
