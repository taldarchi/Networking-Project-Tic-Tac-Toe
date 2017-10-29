################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/ClientCLI.cpp \
../src/FileInter.cpp \
../src/Game.cpp \
../src/MTCPListener.cpp \
../src/MThread.cpp \
../src/Socket.cpp \
../src/TCPSocket.cpp \
../src/UDPMessenger.cpp \
../src/UDPSocket.cpp 

OBJS += \
./src/Client.o \
./src/ClientCLI.o \
./src/FileInter.o \
./src/Game.o \
./src/MTCPListener.o \
./src/MThread.o \
./src/Socket.o \
./src/TCPSocket.o \
./src/UDPMessenger.o \
./src/UDPSocket.o 

CPP_DEPS += \
./src/Client.d \
./src/ClientCLI.d \
./src/FileInter.d \
./src/Game.d \
./src/MTCPListener.d \
./src/MThread.d \
./src/Socket.d \
./src/TCPSocket.d \
./src/UDPMessenger.d \
./src/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ubuntu/Desktop/WorkSpace/ClientProject/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


