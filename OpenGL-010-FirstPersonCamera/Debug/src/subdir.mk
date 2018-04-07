################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ModelLoader.cpp \
../src/OpenGL-010-FirstPersonCamera.cpp \
../src/ShaderProgram.cpp 

OBJS += \
./src/ModelLoader.o \
./src/OpenGL-010-FirstPersonCamera.o \
./src/ShaderProgram.o 

CPP_DEPS += \
./src/ModelLoader.d \
./src/OpenGL-010-FirstPersonCamera.d \
./src/ShaderProgram.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/naveen/opengl/dist/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


