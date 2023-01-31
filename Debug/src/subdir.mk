################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Cryptographer.cpp \
../src/DataBaseLoader.cpp \
../src/Logger.cpp \
../src/TCPServer.cpp \
../src/TCPSetverProj.cpp 

CPP_DEPS += \
./src/Cryptographer.d \
./src/DataBaseLoader.d \
./src/Logger.d \
./src/TCPServer.d \
./src/TCPSetverProj.d 

OBJS += \
./src/Cryptographer.o \
./src/DataBaseLoader.o \
./src/Logger.o \
./src/TCPServer.o \
./src/TCPSetverProj.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Cryptographer.d ./src/Cryptographer.o ./src/DataBaseLoader.d ./src/DataBaseLoader.o ./src/Logger.d ./src/Logger.o ./src/TCPServer.d ./src/TCPServer.o ./src/TCPSetverProj.d ./src/TCPSetverProj.o

.PHONY: clean-src

