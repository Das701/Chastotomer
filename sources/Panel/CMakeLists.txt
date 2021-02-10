cmake_minimum_required(VERSION 3.13)

project (Panel)

include_directories( src
                     src/Hardware/HAL
                     src/Hardware/USBD
                     tests
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
add_compile_options(/wd4623)
add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base
                                # class assignment operator is inaccessible or deleted
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd26812)


file(GLOB HAL                 ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                              ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)
file(GLOB SRC                 src/*.*)
file(GLOB CALCULATE           src/Calculate/*)
file(GLOB DISPLAY             src/Display/*.*)
file(GLOB DISPLAY_FONT        src/Display/Font/*)
file(GLOB HARDWARE            src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS      src/Hardware/CMSIS/*)
file(GLOB HARDWARE_HAL        src/Hardware/HAL/*)
file(GLOB HARDWARE_USBD       src/Hardware/USBD/*
                              ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
                              ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                              ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c)
file(GLOB KEYBOARD            src/Keyboard/*)
file(GLOB MENU                src/Menu/*.*)
file(GLOB MENU_PAGES          src/Menu/Pages/*.*)
file(GLOB MENU_PAGES_CHANNELS src/Menu/Pages/Channels/*)
file(GLOB MENU_PAGES_MODES    src/Menu/Pages/Modes/*)
file(GLOB SCPI                src/SCPI/*)
file(GLOB UTILS               src/Utils/*)
file(GLOB TESTS               tests/*)


source_group(HAL                   FILES ${HAL})
source_group(src                   FILES ${SRC})
source_group(Calculate             FILES ${CALCULATE})
source_group(Display               FILES ${DISPLAY})
source_group(Display\\Font         FILES ${DISPLAY_FONT})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware\\CMSIS       FILES ${HARDWARE_CMSIS})
source_group(Hardware\\HAL         FILES ${HARDWARE_HAL})
source_group(Hardware\\USBD        FILES ${HARDWARE_USBD})
source_group(Keyboard              FILES ${KEYBOARD})
source_group(Menu                  FILES ${MENU})
source_group(Menu\\Pages           FILES ${MENU_PAGES})
source_group(Menu\\Pages\\Channels FILES ${MENU_PAGES_CHANNELS})
source_group(Menu\\Pages\\Modes    FILES ${MENU_PAGES_MODES})
source_group(SCPI                  FILES ${SCPI})
source_group(Utils                 FILES ${UTILS})
source_group(Tests                 FILES ${TESTS})

set_source_files_properties(${HARDWARE_CMSIS} ${HAL}
                            ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
                            ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                            ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
                            PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(Panel ${HAL} ${SRC} ${CALCULATE} ${DISPLAY} ${DISPLAY_FONT} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_HAL}
                  ${HARDWARE_USBD} ${KEYBOARD} ${MENU} ${MENU_PAGES} ${MENU_PAGES_CHANNELS} ${MENU_PAGES_MODES} ${SCPI}
                  ${UTILS} ${TESTS})
