cmake_minimum_required(VERSION 3.13)

project (Panel)

include_directories( src
                     src/libs/HAL
                     src/Hardware
                     ../common/_VS_KEIL/CMSIS
                     ../common/_VS/std_includes
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                     ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    )

add_definitions(-DSTM32F429xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4626)
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5027)
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

source_group(src            REGULAR_EXPRESSION src/*.*)
source_group(Display        REGULAR_EXPRESSION src/Display/*.*)
source_group(Display\\Font  REGULAR_EXPRESSION src/Display/Font/*.*)
source_group(Hardware       REGULAR_EXPRESSION src/Hardware/*.*)
source_group(Keyboard       REGULAR_EXPRESSION src/Keyboard/*.*)
source_group(libs\\CMSIS    REGULAR_EXPRESSION src/libs/CMSIS/*.*)
source_group(libs\\HAL      REGULAR_EXPRESSION src/libs/HAL/*.*)
source_group(Menu           REGULAR_EXPRESSION src/Menu/*.*)
source_group(Menu\\Pages    REGULAR_EXPRESSION src/Menu/Pages/*.*)
source_group(Utils          REGULAR_EXPRESSION src/Utils/*.*)

file(GLOB SOURCES   src/*
                    src/Display/*
                    src/Display/Font/*
                    src/Hardware/*
                    src/Keyboard/*
                    src/Interface/*
                    src/libs/CMSIS/startup_stm32f429xx.s
                    src/libs/HAL/*
                    src/Menu/*
                    src/Menu/Pages/*
                    src/Utils/*
                    )

add_library(Panel ${SOURCES})
