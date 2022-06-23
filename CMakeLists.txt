cmake_minimum_required(VERSION 3.13)

include (pico_sdk_import.cmake)
project (z80disk C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(SIMPLE_VGA_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()
add_executable(z80disk
     hw_config.c
     ide.c
     dictionary.c
     iniparser.c
#cpm
     cpmfs.c
     device_sdfs.c 
)
pico_generate_pio_header(z80disk  ${CMAKE_CURRENT_LIST_DIR}/z80io.pio)
target_sources(z80disk  PRIVATE z80disk.c)

add_subdirectory(FatFs_SPI build)


target_link_libraries(z80disk  PRIVATE
        pico_stdlib
        hardware_pio
	hardware_dma
	pico_multicore
	hardware_gpio
	FatFs_SPI

)

pico_enable_stdio_usb(z80disk  0)
pico_enable_stdio_uart(z80disk  1)
pico_add_extra_outputs(z80disk )




