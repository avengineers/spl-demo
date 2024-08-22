#include <gtest/gtest.h>
using namespace testing;

extern "C" {
#include "check_abort.h"
}

#include "mockup_src_examples_check_abort.h"

// Just provide a simple test case for the function CheckAbort
// to reach 100% MC/DC coverage.
TEST(check_abort, test_mcdc)
{
    CREATE_MOCK(mymock);
    ASSERT_EQ(FALSE, CheckAbort(FALSE, FALSE, FALSE));
    ASSERT_EQ(FALSE, CheckAbort(FALSE, TRUE, FALSE));
    ASSERT_EQ(TRUE, CheckAbort(FALSE, TRUE, TRUE));
    ASSERT_EQ(TRUE, CheckAbort(TRUE, FALSE, FALSE));
}
