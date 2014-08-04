#include "I2C.h"
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef LOGGER_SUPPORT
#include <C_Logger/Logger.h>
#include <errno.h>
#endif

#define I2C_WRITE 0
#define I2C_READ 1

uint8_t I2C_Initialize ( SI2C_Device *In_Device, uint8_t In_Bus )
    {
    char Temp[50];
    snprintf ( Temp, sizeof ( Temp ), "/dev/i2c-%d", In_Bus );
    if ( ( In_Device->FileDescriptor = open ( Temp, O_RDWR ) ) < 0 )
        {
#ifdef LOGGER_SUPPORT
        Logger_Error ( LOGGER_HERE_AND_NOW,  "Unable to open filename %s. \"%s\"",Temp, strerror ( errno ) );
#endif
        In_Device->FileDescriptor = -1;
        return 1;
        }
    return 0;
    }

uint8_t I2C_Shutdown ( SI2C_Device *In_Device )
    {
    if ( In_Device->FileDescriptor != -1 )
        {
        close ( In_Device->FileDescriptor );
        In_Device->FileDescriptor = -1;
        }
    return 0;
    }

uint8_t I2C_SetSlaveDeviceAddress ( SI2C_Device *In_Device, uint8_t In_SlaveDeviceAddress )
    {
    if ( ioctl ( In_Device->FileDescriptor, I2C_SLAVE, In_SlaveDeviceAddress ) < 0 )
        {
#ifdef LOGGER_SUPPORT
        Logger_Error ( LOGGER_HERE_AND_NOW,  "Unable to do ioctl. \"%s\"", strerror ( errno ) );
#endif
        return 1;
        }
    return 0;
    }
/*    
uint8_t I2C_Start ( SI2C_Device *In_Device, uint8_t In_Mode )
    {
    return 1;
    }
uint8_t I2C_Stop ( SI2C_Device *In_Device )
    {
    return 1;
    }
uint8_t I2C_WriteData ( SI2C_Device *In_Device, uint8_t *In_Data, uint8_t In_Length )
    {
    unsigned cont;
    for ( cont = 0; cont < In_Length; ++cont )
        {
        uint8_t Result = I2C_WriteByte ( In_Device, In_Data[cont] );
        if ( Result != 0 )
            return Result;
        }
    return 0;
    }
uint8_t I2C_WriteByte ( SI2C_Device *In_Device, uint8_t In_Data )
    {
    int Result = i2c_smbus_write_byte ( In_Device->FileDescriptor, In_Data );
    return ( Result == 0?0:1 );
    }
uint8_t I2C_ReadData ( SI2C_Device *In_Device, uint8_t *Out_Data, uint8_t In_Length )
    {
    unsigned cont;
    for ( cont = 0; cont < In_Length; ++cont )
        {
        uint8_t Result = I2C_ReadByte ( In_Device, Out_Data + cont );
        if ( Result != 0 )
            return Result;
        }
    return 0;
    }
uint8_t I2C_ReadByte ( SI2C_Device *In_Device, uint8_t *Out_Data )
    {
    int Result = i2c_smbus_read_byte ( In_Device->FileDescriptor );
    if ( Result < 0 )
        return 1;
    *Out_Data = Result & 0xFF;
    return 0;
    }
uint8_t I2C_ReadNACK ( SI2C_Device *In_Device )
    {
    return 1;
    }*/
uint8_t I2C_WriteByteToAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t In_Data )
    {
    int Result = i2c_smbus_write_byte_data ( In_Device->FileDescriptor, In_Address, In_Data );
    return ( Result == 0?0:1 );
    }
uint8_t I2C_WriteDataToAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t *In_Data, uint8_t In_Length )
    {
    int Result = i2c_smbus_write_block_data ( In_Device->FileDescriptor, In_Address, In_Length, In_Data );
    return ( Result == 0?0:1 );
    }
uint8_t I2C_ReadByteFromAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t *Out_Data )
    {
    int Result = i2c_smbus_read_byte_data ( In_Device->FileDescriptor, In_Address );
    if ( Result < 0 )
        return 1;
    *Out_Data = Result & 0xFF;
    return 0;
    }
uint8_t I2C_ReadDataFromAddress ( SI2C_Device *In_Device, uint8_t In_Address, uint8_t *Out_Data, uint8_t In_Length )
    {
    int Result = i2c_smbus_read_i2c_block_data ( In_Device->FileDescriptor, In_Address, In_Length, Out_Data );
    return ( Result == In_Length?0:1 );
    }

