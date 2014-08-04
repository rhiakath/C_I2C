#ifndef I2C_INCLUDED
#define I2C_INCLUDED

#include <stdint.h>

typedef struct
    {
#ifndef ATMEGA
    int FileDescriptor;
#endif
    uint8_t SlaveDeviceAddress;
    uint8_t BusAddress;
    } SI2C_Device;

uint8_t I2C_Initialize ( SI2C_Device *In_Device, uint8_t In_Bus );
uint8_t I2C_Shutdown ( SI2C_Device *In_Device );
uint8_t I2C_SetSlaveDeviceAddress ( SI2C_Device *In_Device, uint8_t In_SlaveDeviceAddress );
// uint8_t I2C_Start ( SI2C_Device *In_Device, uint8_t In_Mode );
// uint8_t I2C_Stop ( SI2C_Device *In_Device );
// uint8_t I2C_WriteData ( SI2C_Device *In_Device, uint8_t *In_Data, uint8_t In_Length );
// uint8_t I2C_WriteByte ( SI2C_Device *In_Device, uint8_t In_Data );
// uint8_t I2C_ReadData ( SI2C_Device *In_Device, uint8_t *Out_Data, uint8_t In_Length );
// uint8_t I2C_ReadByte ( SI2C_Device *In_Device, uint8_t *Out_Data );
// uint8_t I2C_ReadNACK ( SI2C_Device *In_Device );

uint8_t I2C_WriteByteToAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t In_Data );
uint8_t I2C_WriteDataToAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t *In_Data, uint8_t In_Length );
uint8_t I2C_ReadByteFromAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t *Out_Data );
uint8_t I2C_ReadDataFromAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t *Out_Data, uint8_t In_Length );

#endif
