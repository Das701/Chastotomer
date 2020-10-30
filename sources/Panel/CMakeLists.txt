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

add_definitions(-DSTM32F401xE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4211)    #  nonstandard extension used: redefined extern to static
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5204)    # 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
add_compile_options(/wd5219)    # implicit conversion from 'type1' to 'type2', possible loss of data


file(GLOB HAL          ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                       ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)
file(GLOB SRC          src/*.h src/*.cpp)
file(GLOB DISPLAY      src/Display/*.h src/Display/*.cpp)
file(GLOB DISPLAY_FONT src/Display/Font/*)
file(GLOB FREQMETER    src/FreqMeter/*.h src/FreqMeter/*.cpp)
file(GLOB HARDWARE     src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB KEYBOARD     src/Keyboard/*.h src/Keyboard/*.cpp)
file(GLOB LIBS_CMSIS   src/libs/CMSIS/*)
file(GLOB LIBS_HAL     src/libs/HAL/*)
file(GLOB LIBS_USBD    src/libs/USBD/*
                       ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
                       ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                       ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c)
file(GLOB MENU         src/Menu/*.h src/Menu/*cpp)
file(GLOB MENU_PAGES   src/Menu/Pages/*)
file(GLOB SCPI         src/SPCI/*)
file(GLOB UTILS        src/Utils/*)


source_group(HAL            FILES ${HAL})
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


set_source_files_properties(${LIBS_CMSIS} ${HAL}
                            ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
                            ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                            ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
                            PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(Panel ${HAL} ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${FREQMETER} ${HARDWARE} ${KEYBOARD} ${LIBS_CMSIS} ${LIBS_HAL} ${LIBS_USBD} ${MENU} ${MENU_PAGES} ${SCPI} ${UTILS})
