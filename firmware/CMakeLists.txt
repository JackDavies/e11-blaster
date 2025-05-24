cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# project
add_executable(output
    main.c ammo.c laser.c sound.c rpi-pico-utils/src/button.c rpi-pico-utils/src/bargraph.c audiolib/audio.c 
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(output pico_stdlib pico_multicore hardware_dma hardware_pwm)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(output)

#SET(CMAKE_C_FLAGS "-O0")
