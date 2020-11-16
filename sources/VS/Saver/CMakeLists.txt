cmake_minimum_required(VERSION 3.15)

project (Saver)

include_directories(src/
                    ../../common/_VS_KEIL/CMSIS/
                    ../../common/_VS_KEIL/HAL/
                    ../../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/
                    ../../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/
                    ../../Panel/src/
                    ../../Panel/src/Hardware/HAL/
                    ../../Panel/src/Hardware/
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DSTM32F429xx -DWXUSINGDLL -DGUI)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4211)    # nonstandard extension used: redefined extern to static
add_compile_options(/wd4355)    # 'this': used in base member initializer list
add_compile_options(/wd4514)    # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4571)    # Informational: catch(...) semantics changed since Visual C++ 7.1
add_compile_options(/wd4625)    # 'xxx': copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # 'xxx' : format string expected in argument Y is not a string literal
add_compile_options(/wd4820)    # 'xxx': 'y' bytes padding added after data member 'zzz'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe. Consider using yyy instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS
add_compile_options(/wd5026)    # 'xxx': move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # 'xxx': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5204)    # 'xxx': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
add_compile_options(/wd5219)
add_compile_options(/wd26821)
add_compile_options(/wd26812)

file(GLOB SRC                 ../../Panel/src/defines.h
                                          src/Application_win.*
                                          src/main_win.cpp)
file(GLOB DISPLAY             ../../Panel/src/Display/Colors.*
                                          src/Display/Display_win.*)
file(GLOB KEYBOARD            ../../Panel/src/Keyboard/Keyboard.h
                                          src/Keyboard/Keyboard_win.cpp)
file(GLOB GUI                             src/GUI/*)
file(GLOB UTILS               ../../Panel/src/Utils/Buffer.*
                              ../../Panel/src/Utils/Stack.*
                              ../../Panel/src/Utils/String.*
                              ../../Panel/src/Utils/StringUtils.*)

source_group(src               FILES ${SRC})
source_group(Display           FILES ${DISPLAY})
source_group(Keyboard          FILES ${KEYBOARD})
source_group(GUI               FILES ${GUI})
source_group(Utils             FILES ${UTILS})


add_executable(Saver ${SRC} ${DISPLAY} ${GUI} ${KEYBOARD} ${UTILS})

target_link_libraries(Saver ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(Saver ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(Saver WinMM.lib)
target_link_libraries(Saver version.lib)
target_link_libraries(Saver Imm32.Lib)
target_link_libraries(Saver Setupapi.lib)
