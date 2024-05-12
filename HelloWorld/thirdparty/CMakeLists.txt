cmake_minimum_required(VERSION 3.15.3)

project(thirdparty)

# Begin CMSIS 5 library for Cortex-M
add_library(cmsis_5_core_m INTERFACE)
add_library(thirdparty::cmsis_5_core_m ALIAS cmsis_5_core_m)

target_include_directories(cmsis_5_core_m INTERFACE
  CMSIS_5/CMSIS/Core/Include
  CMSIS_5/Device/ARM/ARMCM4/Include
)

target_compile_definitions(cmsis_5_core_m INTERFACE
  "__PROGRAM_START"
)
# End CMSIS 5 library for Cortex-M