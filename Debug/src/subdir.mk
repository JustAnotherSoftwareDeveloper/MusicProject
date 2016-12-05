################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CS229File.cpp \
../src/CS229IO.cpp \
../src/HelpDialog.cpp \
../src/MusicFile.cpp \
../src/MusicIO.cpp \
../src/MusicProject.cpp \
../src/SoundCat.cpp \
../src/SoundGen.cpp \
../src/SoundMix.cpp \
../src/soundInfo.cpp 

OBJS += \
./src/CS229File.o \
./src/CS229IO.o \
./src/HelpDialog.o \
./src/MusicFile.o \
./src/MusicIO.o \
./src/MusicProject.o \
./src/SoundCat.o \
./src/SoundGen.o \
./src/SoundMix.o \
./src/soundInfo.o 

CPP_DEPS += \
./src/CS229File.d \
./src/CS229IO.d \
./src/HelpDialog.d \
./src/MusicFile.d \
./src/MusicIO.d \
./src/MusicProject.d \
./src/SoundCat.d \
./src/SoundGen.d \
./src/SoundMix.d \
./src/soundInfo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


