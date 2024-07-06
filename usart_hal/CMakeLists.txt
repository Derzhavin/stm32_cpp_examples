cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchains/toolchain-arm-none-eabi.cmake)

project(usart_hal VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE STM32_HAL_DRIVERS "${CMAKE_SOURCE_DIR}/stm32f4xx_hal_driver/*.c")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(LINKER_FILE ${CMAKE_SOURCE_DIR}/stm32f446re.ld)

add_executable(${PROJECT_NAME}.elf 
    main.cpp 
    # ${STM32_HAL_DRIVERS}
    CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    stm32f4xx_hal_driver/Inc
    stm32f4xx_hal_driver/Inc/Legacy
    CMSIS/Device/ST/STM32F4xx/Include/
    CMSIS/Core/Include/
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DSTM32 
    -DSTM32F4 
    -DSTM32F446xx
    -DARMCM4_FP.h     
    -DDEBUG   
)

target_compile_options(${PROJECT_NAME} PRIVATE
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

target_link_options(${PROJECT_NAME} PRIVATE
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
