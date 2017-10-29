################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConnectionManger.cpp \
../src/Dispatcher.cpp \
../src/FileInter.cpp \
../src/MTCPListener.cpp \
../src/MThread.cpp \
../src/Server.cpp \
../src/ServerCLI.cpp \
../src/Socket.cpp \
../src/TCPSocket.cpp \
../src/UDPSocket.cpp \
../src/User.cpp 

OBJS += \
./src/ConnectionManger.o \
./src/Dispatcher.o \
./src/FileInter.o \
./src/MTCPListener.o \
./src/MThread.o \
./src/Server.o \
./src/ServerCLI.o \
./src/Socket.o \
./src/TCPSocket.o \
./src/UDPSocket.o \
./src/User.o 

CPP_DEPS += \
./src/ConnectionManger.d \
./src/Dispatcher.d \
./src/FileInter.d \
./src/MTCPListener.d \
./src/MThread.d \
./src/Server.d \
./src/ServerCLI.d \
./src/Socket.d \
./src/TCPSocket.d \
./src/UDPSocket.d \
./src/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


