################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Peripheral/SystickTimer.c 

CPP_SRCS += \
../Core/Peripheral/Button.cpp \
../Core/Peripheral/ICM_20602.cpp 

C_DEPS += \
./Core/Peripheral/SystickTimer.d 

OBJS += \
./Core/Peripheral/Button.o \
./Core/Peripheral/ICM_20602.o \
./Core/Peripheral/SystickTimer.o 

CPP_DEPS += \
./Core/Peripheral/Button.d \
./Core/Peripheral/ICM_20602.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Peripheral/Button.o: ../Core/Peripheral/Button.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' -DSTM32F446xx '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DARM_MATH_CM4 -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Peripheral/Button.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Core/Peripheral/ICM_20602.o: ../Core/Peripheral/ICM_20602.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' -DSTM32F446xx '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DARM_MATH_CM4 -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Peripheral/ICM_20602.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Core/Peripheral/SystickTimer.o: ../Core/Peripheral/SystickTimer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' -DSTM32F446xx '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DARM_MATH_CM4 -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Peripheral/SystickTimer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

