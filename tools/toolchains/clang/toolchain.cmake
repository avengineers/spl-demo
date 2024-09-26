set(CMAKE_C_COMPILER clang CACHE STRING "C Compiler")
set(CMAKE_CXX_COMPILER clang++ CACHE STRING "CXX Compiler")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} CACHE STRING "ASM Compiler")

# TODO: clarify why llvm-cov produces invalid gcov files (contain blank lines), releated GCOVR issue: https://github.com/gcovr/gcovr/issues/331
set(GCOVR_ADDITIONAL_OPTIONS --gcov-executable \"llvm-cov gcov\" --gcov-ignore-parse-errors --html-title \"Code Coverage Report \(tool suite: LLVM Clang\)\")
