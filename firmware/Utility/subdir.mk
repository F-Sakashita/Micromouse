################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Utility/Calculation.cpp \
../Utility/DebugConsole.cpp 

OBJS += \
./Utility/Calculation.o \
./Utility/DebugConsole.o 

CPP_DEPS += \
./Utility/Calculation.d \
./Utility/DebugConsole.d 


# Each subdirectory must supply rules for building sources it contributes
Utility/Calculation.o: ../Utility/Calculation.cpp Utility/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../../Source/Application -I../../Source/Middle -I../../Source/HAL -I../../Source/HAL/HardwareDriver -I../../Source/HAL/Peripheral -I../../Source/Utility -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Utility/Calculation.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Utility/DebugConsole.o: ../Utility/DebugConsole.cpp Utility/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../../Source/Application -I../../Source/Middle -I../../Source/HAL -I../../Source/HAL/HardwareDriver -I../../Source/HAL/Peripheral -I../../Source/Utility -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Utility/DebugConsole.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

