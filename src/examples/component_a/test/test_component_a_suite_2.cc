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
* .. test:: component_a.TS_COMP_A_050
*    :id: TS_COMP_A_050
*    :tests: SWDD_COMP_A-050
*
* @endrst
*/
TEST(component_a_suite_2, TS_COMP_A_050) {
    CREATE_MOCK(mymock);

    MyDataType input = {
        .a = 13,
        .b = 'a'
    };

    EXPECT_CALL(mymock, ReadDataStructure(AllOf(
        Field(&MyDataType::a, input.a),
        Field(&MyDataType::b, input.b)
    )));

    CheckReadDataStructure(&input);
}

/*!
* @rst
*
* .. test:: component_a_suite_2.TS_COMP_A_060
*    :id: TS_COMP_A_060
*    :tests: SWDD_COMP_A-060
*
* @endrst
*/
TEST(component_a_suite_2, TS_COMP_A_060)
{
    CREATE_MOCK(mymock);

    MyDataType input[MY_DATA_ARRAY_SIZE] = {
        {.a = 11, .b = 'a'},
        {.a = 22, .b = 'b'},
        {.a = 33, .b = 'c'},
    };

    EXPECT_CALL(mymock, ReadDataStructureArray(_)).WillOnce(SetArrayArgument<0>(input, input + MY_DATA_ARRAY_SIZE));

    MyDataType result[MY_DATA_ARRAY_SIZE];
    CheckReadDataStructureArray(result);

    ASSERT_EQ(11, result[0].a);
    ASSERT_EQ('a', result[0].b);
    ASSERT_EQ(22, result[1].a);
    ASSERT_EQ('b', result[1].b);
    ASSERT_EQ(33, result[2].a);
    ASSERT_EQ('c', result[2].b);
}

/*!
* @rst
*
* .. test:: component_a_suite_2.TS_COMP_A_070
*    :id: TS_COMP_A_070
*    :tests: SWDD_COMP_A-070
*
* @endrst
*/
TEST(component_a_suite_2, TS_COMP_A_070)
{
    CREATE_MOCK(mymock);

    MyDataType input[MY_DATA_ARRAY_SIZE] = {
        {.a = 11, .b = 'a'},
        {.a = 22, .b = 'b'},
        {.a = 33, .b = 'c'},
    };

    // Check that the ReadDataStructureArray input array has the same values as the one in input
    EXPECT_CALL(mymock, ReadDataStructureArray(_))
        .WillOnce(Invoke([&input](const MyDataType* array) {
        for (size_t i = 0; i < MY_DATA_ARRAY_SIZE; ++i) {
            EXPECT_EQ(input[i].a, array[i].a) << "Check 'a' for index " << i + 1;
            EXPECT_EQ(input[i].b, array[i].b) << "Check 'b' for index " << i + 1;
        }}));

        CheckReadDataStructureArray(input);

}

typedef struct
{
    const char* description;
    // Inputs
    int input;
    // Outputs
    int output;
} TestDataCalculateSquare_t;

/*!
* @rst
*
* .. test:: component_a_suite_2.TS_COMP_A_080
*    :id: TS_COMP_A_080
*    :tests: SWDD_COMP_A-080
*
* @endrst
*/
TEST(component_a_suite_2, TS_COMP_A_080)
{
    CREATE_MOCK(mymock);

    std::vector<TestDataCalculateSquare_t> TestDataCalculateSquare = {
        {"Positive integer", 10, 100},
        {"Negative integer", -4, 16},
        {"Square zero", 0, 0}
    };

    for (const auto& param : TestDataCalculateSquare) {
        /*
        'SCOPED_TRACE' provides a way to add additional context to the output of test assertions.
        This is particularly useful in situations where tests are performed within a loop or
        across various data sets, and you want to identify which specific  iteration or data set
        caused a test failure.
        */
        SCOPED_TRACE(param.description);
        // Make Read_MyInput return param.input
        EXPECT_CALL(mymock, Read_MyInput(_)).WillOnce(SetArgPointee<0>(param.input));
        // Check that Write_MyInput was called with param.output value
        EXPECT_CALL(mymock, Write_MyInput(Pointee(param.output)));
        CalculateSquare();
    }
}
