cmake_minimum_required(VERSION 3.0)

project (Panel)

include_directories(
                    ../../common/_VS/CMSIS/
                    )
                    
add_definitions(-DSTM32F429xx -DWXUSINGDLL -DUSE_SDL2 -DGUI)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

source_group(src            REGULAR_EXPRESSION ../../Panel/src/*.*)
source_group(Display        REGULAR_EXPRESSION ../../Panel/src/Display/*.*)
source_group(Display\\Font  REGULAR_EXPRESSION ../../Panel/src/Display/Font/*.*)
source_group(Hardware       REGULAR_EXPRESSION ../../Panel/src/Hardware/*.*)
source_group(Keyboard       REGULAR_EXPRESSION ../../Panel/src/Keyboard/*.*)
source_group(libs\\CMSIS    REGULAR_EXPRESSION ../../Panel/src/libs/CMSIS/*.*)
source_group(libs\\HAL      REGULAR_EXPRESSION ../../Panel/src/libs/HAL/*.*)
source_group(Menu           REGULAR_EXPRESSION ../../Panel/src/Menu/*.*)
source_group(Menu\\Pages    REGULAR_EXPRESSION ../../Panel/src/Menu/Pages/*.*)
source_group(Utils          REGULAR_EXPRESSION ../../Panel/src/Utils/*.*)

file(GLOB SOURCES   ../../Panel/src/*.*
                    ../../Panel/src/Display/*.*
                    ../../Panel/src/Display/Font/*.*
                    ../../Panel/src/Hardware/*.*
                    ../../Panel/src/Keyboard/*.*
                    ../../Panel/src/libs/CMSIS/*.*
                    ../../Panel/src/libs/HAL/*.*
                    ../../Panel/src/Menu/*.*
                    ../../Panel/src/Menu/Pages/*.*
                    ../../Panel/src/Utils/*.*
                    )
                    
add_executable(Panel ${SOURCES})

target_link_libraries(Panel "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxbase31ud.lib")
target_link_libraries(Panel "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxmsw31ud_core.lib")

target_link_libraries(Panel "..\\..\\..\\ThirdParty\\SDL2\\SDL2-2.0.10\\generated\\Debug\\SDL2-staticd.lib")

target_link_libraries(Panel WinMM.lib)
target_link_libraries(Panel version.lib)
target_link_libraries(Panel Imm32.Lib)
target_link_libraries(Panel Setupapi.lib)
