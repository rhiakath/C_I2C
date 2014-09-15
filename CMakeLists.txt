cmake_minimum_required(VERSION 2.8)
project ( C_I2C )

if ( ATMEGA )
message ( STATUS "Compiling I2C for ATMega" )
else ( ATMEGA )
message ( STATUS "Compiling I2C for Linux" )
endif ( ATMEGA )
add_library ( C_I2C I2CDevice.c I2CDevice.h )
