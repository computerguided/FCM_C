################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Command.c \
../src/CommandHandler.c \
../src/Component.c \
../src/Control.c \
../src/Controller.c \
../src/DemoDevice.c \
../src/Device.c \
../src/Display.c \
../src/FCM_demo.c \
../src/Interface.c \
../src/Logical.c \
../src/MessageQueue.c \
../src/StateMachineEngine.c \
../src/StateTransitionTable.c \
../src/Timer.c \
../src/TimerHandler.c \
../src/Utils.c 

OBJS += \
./src/Command.o \
./src/CommandHandler.o \
./src/Component.o \
./src/Control.o \
./src/Controller.o \
./src/DemoDevice.o \
./src/Device.o \
./src/Display.o \
./src/FCM_demo.o \
./src/Interface.o \
./src/Logical.o \
./src/MessageQueue.o \
./src/StateMachineEngine.o \
./src/StateTransitionTable.o \
./src/Timer.o \
./src/TimerHandler.o \
./src/Utils.o 

C_DEPS += \
./src/Command.d \
./src/CommandHandler.d \
./src/Component.d \
./src/Control.d \
./src/Controller.d \
./src/DemoDevice.d \
./src/Device.d \
./src/Display.d \
./src/FCM_demo.d \
./src/Interface.d \
./src/Logical.d \
./src/MessageQueue.d \
./src/StateMachineEngine.d \
./src/StateTransitionTable.d \
./src/Timer.d \
./src/TimerHandler.d \
./src/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


