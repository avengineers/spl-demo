/**
 * @file
 */

#include <gtest/gtest.h>
using namespace testing;

extern "C" {
#include "component_a.h"
}

#include "mockup_src_examples_component_a.h"

/*!
* @rst
*
* .. test:: component_a_suite_1.TS_COMP_A_010
*    :id: TS_COMP_A_010
*    :tests: SWDD_COMP_A-010
*
* @endrst
*/
TEST(component_a_suite_1, TS_COMP_A_010)
{
    // Arrange
    CREATE_MOCK(mymock);
    EXPECT_CALL(mymock, ReadSomeData(_)).WillOnce(SetArgPointee<0>(13));
    ASSERT_EQ(13, CheckReadSomeData());
}

/*!
* @rst
*
* .. test:: component_a_suite_1.TS_COMP_A_011
*    :id: TS_COMP_A_011
*    :tests: SWDD_COMP_A-011
*
* @endrst
*/
TEST(component_a_suite_1, TS_COMP_A_011)
{
    CREATE_MOCK(mymock);
    // Expect that ReadSomeData is called with a pointer which points to a value that is equal to 13
    EXPECT_CALL(mymock, ReadSomeData(Pointee(13)));
    CheckWriteSomeData(13);
}

/*!
* @rst
*
* .. test:: component_a_suite_1.TS_COMP_A_020
*    :id: TS_COMP_A_020
*    :tests: SWDD_COMP_A-020
*
* @endrst
*/
TEST(component_a_suite_1, TS_COMP_A_020)
{
    CREATE_MOCK(mymock);
    // Variable gets updated because return status is zero
    EXPECT_CALL(mymock, ReadSomeDataAndReturn(_)).WillOnce(DoAll(SetArgPointee<0>(13), Return(0)));
    ASSERT_EQ(13, CheckReadSomeDataAndReturn());
    // Variable does not get updated because the return status is non-zero
    EXPECT_CALL(mymock, ReadSomeDataAndReturn(_)).WillOnce(DoAll(SetArgPointee<0>(13), Return(1)));
    ASSERT_EQ(0, CheckReadSomeDataAndReturn());
}

/*!
* @rst
*
* .. test:: component_a_suite_1.TS_COMP_A_030
*    :id: TS_COMP_A_030
*    :tests: SWDD_COMP_A-030
*
* @endrst
*/
TEST(component_a_suite_1, TS_COMP_A_030)
{
    CREATE_MOCK(mymock);
    // Variables get updated because return status is zero
    EXPECT_CALL(mymock, ReadMultipleDataAndReturn(_, _)).WillOnce(
        DoAll(SetArgPointee<0>(5),
            SetArgPointee<1>(8),
            Return(0)
        ));
    ASSERT_EQ(13, CheckReadMultipleDataAndReturn());
    // Variables do not get updated because the return status is non-zero
    EXPECT_CALL(mymock, ReadMultipleDataAndReturn(_, _)).WillOnce(
        DoAll(SetArgPointee<0>(5),
            SetArgPointee<1>(8),
            Return(1)
        ));
    ASSERT_EQ(0, CheckReadMultipleDataAndReturn());
}
