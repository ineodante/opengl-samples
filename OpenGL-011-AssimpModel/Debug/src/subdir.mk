################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Mesh.cpp \
../src/Shader.cpp \
../src/Window.cpp \
../src/main.cpp 

OBJS += \
./src/Mesh.o \
./src/Shader.o \
./src/Window.o \
./src/main.o 

CPP_DEPS += \
./src/Mesh.d \
./src/Shader.d \
./src/Window.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/naveen/opengl/distonly -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


