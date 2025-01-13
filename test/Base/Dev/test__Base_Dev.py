from pathlib import Path
import pytest
from spl_core.test_utils.base_variant_test_runner import BaseVariantTestRunner


class Test_Base__Dev(BaseVariantTestRunner):
    @property
    def component_paths(self):
        return [
            Path("src/examples/component_a"),
        ]

    @property
    def expected_build_artifacts(self):
        return []

    @pytest.mark.build
    def test_build(self):
        pass
