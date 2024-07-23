cmake_minimum_required(VERSION 3.24)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled1 main.c preprocessor.c main.h macro.h macro.c firstPass.c parsing.h cmdList.h validation.c validation.h labels.h parsing.c firstPass.h cmdList.c labels.c codeImage.c codeImage.h datalmage.c datalmage.h encoding.c encoding.h testing.c testing.h secondPass.c secondPass.h output.c output.h strOps.c strOps.h preprocessor.h)
