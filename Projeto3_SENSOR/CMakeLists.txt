cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(temperatura_rp2040 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(temperatura_rp2040
    main.c
)

target_link_libraries(temperatura_rp2040
    pico_stdlib
    hardware_adc
)

# Habilita stdio sobre USB, desabilita UART
pico_enable_stdio_uart(temperatura_rp2040 0)
pico_enable_stdio_usb(temperatura_rp2040 1)

pico_add_extra_outputs(temperatura_rp2040)