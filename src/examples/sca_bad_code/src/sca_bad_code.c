/**
 * @file sca_bad_code.c
 * @brief Module to check output of the SCA tool.
 */

#include "sca_bad_code.h"
#include <stdio.h>
#include <stdlib.h>

void memory_leak() {
    int* ptr = (int*)malloc(sizeof(int) * 10);
    // Memory is allocated but not freed
}

void null_pointer_dereference() {
    int* ptr = NULL;
    *ptr = 10; // Dereferencing a null pointer
}

void buffer_overflow() {
    int arr[10];
    for (int i = 0; i <= 10; i++) {
        arr[i] = i; // Writing out of bounds
    }
}

int main_bad() {
    memory_leak();
    null_pointer_dereference();
    buffer_overflow();
    return 0;
}
