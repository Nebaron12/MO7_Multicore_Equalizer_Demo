# Generated by the Vitis IDE. Do not modify.

cmake_minimum_required(VERSION 3.20)
project(Multicore_Demo_System
LANGUAGES
C CXX
)

# Including the cmake modules from the Vitis Install.
set(CMAKE_MODULE_PATH
"$ENV{XILINX_VITIS}/vitis-server/scripts/cmake"
${CMAKE_MODULE_PATH}
)
find_package(BaremetalHost REQUIRED)

# Default Target is hw_emu, can be overridden by passing cmake argument. Valid
# value is hw_emu or hw Example: -DVITIS_TARGET=hw_emu builds the project for
# target hw_emu
set(VITIS_TARGET
hw_emu
CACHE STRING
"Target for which the project should be built"
)
set_property(CACHE VITIS_TARGET
   PROPERTY
    STRINGS hw_emu hw
)

set(VITIS_PLATFORM_PATH
D:\Fontys\MO7\SID_Multicore_Demo\platform\export\platform\platform.xpfm
CACHE STRING
"Vitis Platform"
)

