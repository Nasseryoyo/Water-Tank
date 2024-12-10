# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(WaterTank C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Find all source files in the Drivers folder and its subdirectories
file(GLOB_RECURSE DRIVERS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/Drivers/**/*.c")

# Add the main source file
set(SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/WaterTank.c"
    ${DRIVERS_SOURCES}
)

add_subdirectory(freertos)

# Add executable. Default name is the project name, version 0.1

add_executable(WaterTank ${SOURCES})
pico_set_program_name(WaterTank "WaterTank")
pico_set_program_version(WaterTank "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(WaterTank 1)
pico_enable_stdio_usb(WaterTank 1)

# Add the standard library to the build
target_link_libraries(WaterTank
        pico_stdlib freertos)

# Add the standard include files to the build
target_include_directories(WaterTank PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add your source files here

include_directories(${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include)


# Add any user requested libraries
target_link_libraries(WaterTank 
        pico_cyw43_arch_none
        )

pico_add_extra_outputs(WaterTank)

