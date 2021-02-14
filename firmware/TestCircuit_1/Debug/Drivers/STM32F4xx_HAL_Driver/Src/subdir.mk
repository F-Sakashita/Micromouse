################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c 

C_DEPS += \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.d \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.d 

OBJS += \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DLSI_VALUE=32000' -DARM_MATH_CM4 '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' -DSTM32F405xx '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' '-DHSE_VALUE=25000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Core/Inc -I../Core/Peripheral -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

