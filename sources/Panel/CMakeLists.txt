cmake_minimum_required(VERSION 3.13)

project (Panel)

include_directories( src
                     src/libs/HAL
                     src/Hardware
                     src/libs/USBD
                     ../common/_VS_KEIL/CMSIS
                     ../common/_VS/std_includes
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                     ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                     ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                     ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    )

add_definitions(-DSTM32F429xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4018)    # 'xxx': signed/unsigned mismatch
add_compile_options(/wd4057)
add_compile_options(/wd4062)    # enumerator 'xxx' in switch of enum 'yyy' is not handled
add_compile_options(/wd4068)    # unknown pragma
add_compile_options(/wd4100)    # 'xxx': unreferenced formal parameter
add_compile_options(/wd4101)    # 'xxx': unreferenced local variable
add_compile_options(/wd4129)    # 'xxx': unrecognized character escape sequence
add_compile_options(/wd4189)    # 'xxx': local variable is initialized but not referenced
add_compile_options(/wd4211)    #  nonstandard extension used: redefined extern to static
add_compile_options(/wd4242)    # 'xxx': conversion from 'int' to 'short', possible loss of data
add_compile_options(/wd4244)    # 'xxx': conversion from 'int' to 'char', possible loss of data
add_compile_options(/wd4305)    # 'xxx': truncation from 'int' to 'char'
add_compile_options(/wd4310)    # cast truncates constant value
add_compile_options(/wd4365)
add_compile_options(/wd4459)    # declaration of 'Temp' hides global declaration
add_compile_options(/wd4505)
add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4571)    # Informational: catch(...) semantics changed since Visual C++ 7.1
add_compile_options(/wd4625)    # 'xxx': copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)
add_compile_options(/wd4668)    # 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4800)
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5026)    # 'xxx': move constructor was implicitly defined as deleted
add_compile_options(/wd5027)
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5204)
add_compile_options(/wd5219)


file(GLOB SRC          src/*.h src/*.cpp)
file(GLOB DISPLAY      src/Display/*.h src/Display/*.cpp)
file(GLOB DISPLAY_FONT src/Display/Font/*)
file(GLOB FREQMETER    src/FreqMeter/*.h src/FreqMeter/*.cpp)
file(GLOB HARDWARE     src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB KEYBOARD     src/Keyboard/*.h src/Keyboard/*.cpp)
file(GLOB LIBS_CMSIS   src/libs/CMSIS/*)
file(GLOB LIBS_HAL     src/libs/HAL/*)
file(GLOB LIBS_USBD    src/libs/USBD/*)
file(GLOB MENU         src/Menu/*.h src/Menu/*cpp)
file(GLOB MENU_PAGES   src/Menu/Pages/*)
file(GLOB SCPI         src/SPCI/*)
file(GLOB UTILS        src/Utils/*)


source_group(src            FILES ${SRC})
source_group(Display        FILES ${DISPLAY})
source_group(Display\\Font  FILES ${DISPLAY_FONT})
source_group(FreqMeter      FILES ${FREQMETER})
source_group(Hardware       FILES ${HARDWARE}})
source_group(Keyboard       FILES ${KEYBOARD})
source_group(libs\\CMSIS    FILES ${LIBS_CMSIS})
source_group(libs\\HAL      FILES ${LIBS_HAL})
source_group(libs\\USBD     FILES ${LIBS_USBD})
source_group(Menu           FILES ${MENU})
source_group(Menu\\Pages    FILES ${MENU_PAGES})
source_group(SCPI           FILES ${SCPI})
source_group(Utils          FILES ${UTILS})


set_source_files_properties(${LIBS_CMSIS}
                            PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(Panel ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${FREQMETER} ${HARDWARE} ${KEYBOARD} ${LIBS_CMSIS} ${LIBS_HAL} ${LIBS_USBD} ${MENU} ${MENU_PAGES} ${SCPI} ${UTILS})
