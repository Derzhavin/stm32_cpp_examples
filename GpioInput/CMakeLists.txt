cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchains/toolchain-arm-none-eabi.cmake)

project(GpioInput)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/linkerScripts/stm32f446re.ld)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES 
    src/*.c
    src/*.cpp 
    src/*.s

    device/*.c
    device/*.cpp 
    device/*.s
)

add_subdirectory(thirdparty)

add_executable(${EXECUTABLE} ${SOURCES})

target_include_directories(${EXECUTABLE} PRIVATE
    device
)
target_link_libraries(${EXECUTABLE} PRIVATE
    thirdparty::cmsis_5_core_m
)

target_compile_definitions(${EXECUTABLE} PRIVATE
    -DSTM32 
    -DSTM32F4 
    -DSTM32F446xx
    -DARMCM4_FP.h     
    -DDEBUG   
)

target_compile_options(${EXECUTABLE} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -fdata-sections
    -ffunction-sections

    -Wall
    -O0
    -g3
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${LINKER_FILE}
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    --specs=nano.specs
    --specs=nosys.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -Xlinker -print-memory-usage -Xlinker
)