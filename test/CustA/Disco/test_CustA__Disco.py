import os.path
from utils import spl_build


class Test_CustA__Disco:
    @classmethod
    def setup_class(cls):
        cls.variant = "CustA/Disco"
        cls.artifacts_collection = ArtifactsCollection()

    def test_unit_tests(self):
        """Unit tests execution shall be successful."""
        assert 0 == spl_build(self.variant, "test", "unittests")

        """Coverage report shall be created"""
        assert os.path.isfile(f"build/{self.variant}/test/reports/coverage/index.html")

    def test_build(self):
        """build wrapper shall build target and related outputs."""
        assert 0 == spl_build(self.variant, "prod", "all")

        """executable shall exist and collected for the bom."""
        self.artifacts_collection.collect(f"build/{self.variant}/prod/spled.exe")

        """bom shall be created"""
        assert os.path.isfile(f"build/{self.variant}/prod/bom.json")

    def test_reports(self):
        """Reports generation shall be successful."""
        assert 0 == spl_build(self.variant, "test", "reports")

        """SWE.4 reports shall be created"""
        report_types = ["html", "coverage"]
        modules = [
            "keyboard_interface",
            "light_controller",
            "main_control_knob",
            "power_signal_processing",
            "spled",
        ]
        for report_type in report_types:
            for module in modules:
                assert os.path.isfile(
                    f"build/{self.variant}/test/src/{module}/reports/{report_type}/index.html"
                )
