################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/delay.c \
../src/ds1302.c \
../src/irpin.c \
../src/main.c \
../src/oled.c \
../src/stepper.c \
../src/sys.c \
../src/syscalls.c \
../src/system_stm32f10x.c \
../src/timer.c \
../src/uart.c 

OBJS += \
./src/delay.o \
./src/ds1302.o \
./src/irpin.o \
./src/main.o \
./src/oled.o \
./src/stepper.o \
./src/sys.o \
./src/syscalls.o \
./src/system_stm32f10x.o \
./src/timer.o \
./src/uart.o 

C_DEPS += \
./src/delay.d \
./src/ds1302.d \
./src/irpin.d \
./src/main.d \
./src/oled.d \
./src/stepper.d \
./src/sys.d \
./src/syscalls.d \
./src/system_stm32f10x.d \
./src/timer.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103ZETx -DDEBUG -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER -I"D:/workspace/AC6/Q19_CAR_STANDBY_F103ZET6_AC6/StdPeriph_Driver/inc" -I"D:/workspace/AC6/Q19_CAR_STANDBY_F103ZET6_AC6/inc" -I"D:/workspace/AC6/Q19_CAR_STANDBY_F103ZET6_AC6/CMSIS/device" -I"D:/workspace/AC6/Q19_CAR_STANDBY_F103ZET6_AC6/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


