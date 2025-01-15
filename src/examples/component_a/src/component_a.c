/**
 * @file component_a.c
 */

#include "component_a.h"

int CheckReadSomeData() {
    int var = 0;
    ReadSomeData(&var);
    return var;
}

void CheckWriteSomeData(int var) {
    ReadSomeData(&var);
}

int CheckReadSomeDataAndReturn() {
    int var = 0;
    unsigned char ret = ReadSomeDataAndReturn(&var);
    // var is not updated unless the return value is success (zero)
    if (ret != 0) {
        return 0;
    }
    else {
        return var;
    }
}

int CheckReadMultipleDataAndReturn() {
    int var1 = 0;
    int var2 = 0;
    unsigned char ret = ReadMultipleDataAndReturn(&var1, &var2);
    // var is not updated unless the return value is success (zero)
    if (ret != 0) {
        return 0;
    }
    else {
        return var1 + var2;
    }
}

void CheckReadDataStructure(MyDataType* output) {
    ReadDataStructure(output);
}

void CheckReadDataStructureArray(MyDataType* output) {
    ReadDataStructureArray(output);
}

extern void CalculateSquare(void) {
    int input, output;
    Read_MyInput(&input);
    output = input * input;
    Write_MyInput(&output);
}
