from spl_core.test_utils.base_variant_test_runner import BaseVariantTestRunner

class Test_Base__Dev(BaseVariantTestRunner):

    @property
    def component_paths(self):
        return []

    @property
    def expected_build_artifacts(self):
        return []
    
    def test_build(self):
        pass
    
    def test_unittest(self) -> None:
        pass
    