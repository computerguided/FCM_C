################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Component.c \
../src/Device.c \
../src/FCM_demo.c \
../src/Logical.c \
../src/MessageQueue.c \
../src/StateMachineEngine.c \
../src/StateTransitionTable.c \
../src/Timer.c \
../src/Utils.c 

OBJS += \
./src/Component.o \
./src/Device.o \
./src/FCM_demo.o \
./src/Logical.o \
./src/MessageQueue.o \
./src/StateMachineEngine.o \
./src/StateTransitionTable.o \
./src/Timer.o \
./src/Utils.o 

C_DEPS += \
./src/Component.d \
./src/Device.d \
./src/FCM_demo.d \
./src/Logical.d \
./src/MessageQueue.d \
./src/StateMachineEngine.d \
./src/StateTransitionTable.d \
./src/Timer.d \
./src/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


