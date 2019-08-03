# Make sure we use the correct vesion
cmake_minimum_required(VERSION 3.14)

# Add the "src" directory as include directory to allow including all "hal-xxx" modules using include.
include_directories(src)

# Add the common data to the project.
add_subdirectory(src/hal-common)
add_subdirectory(src/hal-feather-m0)

# Add the toolchain projects.
add_subdirectory(hal-toolchain/arduino-core)
add_subdirectory(hal-toolchain/arduino-link)

# Name the project.
project(delayed-serial)

# Set the upload port
set(UPLOAD_PORT "/dev/cu.usbmodem141401")

# Define the library to build.
add_library(delayed-serial STATIC src/Application.cpp src/Application.hpp src/main.cpp)
target_link_libraries(delayed-serial HAL-common HAL-feather-m0)

# Link the firmware
arduino_link(delayed-serial)
