cmake_minimum_required(VERSION 3.14)

project (Panel_GUI)

include_directories(
                    ../../common/_VS_KEIL/CMSIS/
                    ../../common/_VS_KEIL/HAL/
                    ../../Panel/src/
                    ../../Panel/src/libs/HAL/
                    ../../Panel/src/Hardware/
                    )
                    
add_definitions(-DSTM32F429xx -DWXUSINGDLL -DUSE_SDL2 -DGUI)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4062)    # enumerator 'xxx' in switch of enum 'yyy' is not handled
add_compile_options(/wd4211)    # nonstandard extension used: redefined extern to static
add_compile_options(/wd4514)    # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'xxx': 'y' bytes padding added after data member 'zzz'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe. Consider using yyy instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS
add_compile_options(/wd5027)    # 'xxx': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

source_group(src            FILES ../../Panel/src/main.cpp
                                  ../../Panel/src/defines.h)
source_group(Display        FILES             src/Display/Display_win.cpp
                                  ../../Panel/src/Display/Colors.cpp
                                  ../../Panel/src/Display/Colors.h
                                  ../../Panel/src/Display/Display.cpp
                                  ../../Panel/src/Display/Display.h
                                  ../../Panel/src/Display/Primitives.cpp
                                  ../../Panel/src/Display/Primitives.h
                                  ../../Panel/src/Display/Text.cpp
                                  ../../Panel/src/Display/Text.h)
source_group(Display\\Font  REGULAR_EXPRESSION ../../Panel/src/Display/Font/*)
source_group(Hardware       REGULAR_EXPRESSION ../../Panel/src/Hardware/*)
source_group(Menu           REGULAR_EXPRESSION ../../Panel/src/Menu/*)
source_group(Menu\\Pages    REGULAR_EXPRESSION ../../Panel/src/Menu/Pages/*)
source_group(Utils          REGULAR_EXPRESSION ../../Panel/src/Utils/*)
source_group(libs\\HAL      FILES ../../common/_VS_KEIL/HAL/stm32f4xx_hal.cpp
                                  ../../common/_VS_KEIL/HAL/stm32f4xx_hal_rcc.cpp
                                  ../../common/_VS_KEIL/HAL/stm32f4xx_hal_rcc_ex.cpp)


file(GLOB SOURCES   ../../Panel/src/main.cpp
                    ../../Panel/src/defines.h
                                src/Display/Display_win.cpp
                    ../../Panel/src/Display/Colors.cpp
                    ../../Panel/src/Display/Colors.h
                    ../../Panel/src/Display/Display.cpp
                    ../../Panel/src/Display/Display.h
                    ../../Panel/src/Display/Primitives.cpp
                    ../../Panel/src/Display/Primitives.h
                    ../../Panel/src/Display/Text.cpp
                    ../../Panel/src/Display/Text.h
                    
                    ../../Panel/src/Display/Font/*
                    ../../Panel/src/Hardware/*.*
                    ../../Panel/src/Menu/*
                    ../../Panel/src/Menu/Pages/*
                    ../../Panel/src/Utils/*
                    
                    ../../common/_VS_KEIL/HAL/stm32f4xx_hal.cpp
                    ../../common/_VS_KEIL/HAL/stm32f4xx_hal_rcc.cpp
                    ../../common/_VS_KEIL/HAL/stm32f4xx_hal_rcc_ex.cpp
                    )
                    
add_executable(Panel_GUI ${SOURCES})

target_link_libraries(Panel_GUI "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxbase31ud.lib")
target_link_libraries(Panel_GUI "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxmsw31ud_core.lib")

target_link_libraries(Panel_GUI "..\\..\\..\\ThirdParty\\SDL2\\SDL2-2.0.10\\generated\\Debug\\SDL2-staticd.lib")

target_link_libraries(Panel_GUI WinMM.lib)
target_link_libraries(Panel_GUI version.lib)
target_link_libraries(Panel_GUI Imm32.Lib)
target_link_libraries(Panel_GUI Setupapi.lib)
