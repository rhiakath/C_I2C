#include "I2CDevice.h"

#ifdef LOGGER_SUPPORT
#include <C_Logger/Logger.h>
#endif
#ifdef ATMEGA
#include <C_ATMega_TWI/ATMega_TWI.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#endif

void I2CDevice_Constructor ( SI2CDevice *In_Device )
    {
#ifdef ATMEGA
    TWIInit();
#else
    In_Device->BusFileDescriptor = -1;
#endif
    In_Device->BusAddress = 0;
    In_Device->DeviceAddress = 0;
    }

void I2CDevice_Destructor ( SI2CDevice *In_Device )
    {
    I2CDevice_CloseDevice ( In_Device );
    }

int8_t I2CDevice_OpenDevice ( SI2CDevice *In_Device, uint8_t In_I2CBus, uint8_t In_DeviceAddress )
    {
    I2CDevice_CloseDevice ( In_Device );

#ifndef ATMEGA
    char Temp[50];
    snprintf ( Temp, sizeof ( Temp ), "/dev/i2c-%d", In_I2CBus );
    errno = 0;
    if ( ( In_Device->BusFileDescriptor = open ( Temp, O_RDWR ) ) < 0 )
        {
        LOG_ERROR ( "Unable to open filename %s. \"%s\"",Temp, strerror ( errno ) );
        In_Device->BusFileDescriptor = -1;
        return -1;
        }
    if ( ioctl ( In_Device->BusFileDescriptor, I2C_SLAVE, In_DeviceAddress ) < 0 )
        {
        LOG_ERROR ( "Unable to do ioctl. \"%s\"", strerror ( errno ) );
        return -2;
        }
#endif
    In_Device->DeviceAddress = In_DeviceAddress;
    In_Device->BusAddress = In_I2CBus;
    return 0;
    }

int8_t I2CDevice_CloseDevice ( SI2CDevice *In_Device )
    {
#ifndef ATMEGA
    if ( In_Device->BusFileDescriptor != -1 )
        close ( In_Device->BusFileDescriptor );
    In_Device->BusFileDescriptor = -1;
#endif
    return 0;
    }

int8_t I2CDevice_SetSlaveDeviceAddress ( SI2CDevice *In_Device, uint8_t In_DeviceAddress )
    {
#ifndef ATMEGA
    if ( ioctl ( In_Device->BusFileDescriptor, I2C_SLAVE, In_DeviceAddress ) < 0 )
        {
        LOG_ERROR ( "Unable to do ioctl. \"%s\"", strerror ( errno ) );
        return -1;
        }
#endif
    In_Device->DeviceAddress = In_DeviceAddress;
    return 0;
    }

int8_t I2CDevice_GetSlaveDeviceAddress ( SI2CDevice *In_Device )
    {
    return In_Device->DeviceAddress;
    }
/*
int8_t I2CDevice_ProbeDevice ( SI2CDevice *In_Device )
    {
    return i2c_smbus_write_byte ( In_Device->BusFileDescriptor, In_Device->DeviceAddress );
    }*/
/*
int8_t I2CDevice_WriteByte ( SI2CDevice *In_Device, uint8_t In_Data )
    {
#ifdef ATMEGA
    return TWIWrite ( In_Data );
#else
    if ( i2c_smbus_write_byte ( In_Device->BusFileDescriptor, In_Data ) != 0 )
        {
        LOG_ERROR ( "Unable to write byte. \"%s\"", strerror ( errno ) );
        return -1;
        }
#endif
    return 0;
    }

int8_t I2CDevice_ReadByte ( SI2CDevice *In_Device, uint8_t *Out_Data )
    {
    int Result = i2c_smbus_read_byte ( In_Device->BusFileDescriptor );
    if ( Result < 0 )
        {
        LOG_ERROR ( "Unable to read byte. \"%s\"", strerror ( errno ) );
        return -1;
        }
    *Out_Data = Result & 0xFF;
    return 0;
    }*/

int8_t I2CDevice_WriteByteToAddress ( SI2CDevice *In_Device, uint8_t In_Address, uint8_t In_Data )
    {
#ifdef ATMEGA
    return I2CDevice_WriteDataToAddress ( In_Device, In_Address, &In_Data, 1 );
#else
    if ( i2c_smbus_write_byte_data ( In_Device->BusFileDescriptor, In_Address, In_Data ) != 0 )
        {
        LOG_ERROR ( "Unable to write byte. \"%s\"", strerror ( errno ) );
        return -1;
        }
    return 0;
#endif
    }

int8_t I2CDevice_ReadByteFromAddress ( SI2CDevice *In_Device, uint8_t In_Address, uint8_t *Out_Data )
    {
#ifdef ATMEGA
    return I2CDevice_ReadDataFromAddress ( In_Device, In_Address, Out_Data, 1 );
#else
    int Result = i2c_smbus_read_byte_data ( In_Device->BusFileDescriptor, In_Address );
    if ( Result < 0 )
        {
        LOG_ERROR ( "Unable to read byte. \"%s\"", strerror ( errno ) );
        return -1;
        }
    *Out_Data = Result & 0xFF;
    return 0;
#endif
    }/*

int8_t I2CDevice_WriteWordToAddress ( SI2CDevice *In_Device, uint8_t In_Address, uint16_t In_Data )
    {
    if ( i2c_smbus_write_word_data ( In_Device->BusFileDescriptor, In_Address, In_Data ) != 0 )
        {
        LOG_ERROR ( "Unable to write word. \"%s\"", strerror ( errno ) );
        return -1;
        }
    return 0;
    }

int8_t I2CDevice_ReadWordFromAddress ( SI2CDevice *In_Device, uint8_t In_Address, uint16_t* Out_Data )
    {
    int Result = i2c_smbus_read_word_data ( In_Device->BusFileDescriptor, In_Address );
    if ( Result < 0 )
        {
        LOG_ERROR ( "Unable to read word. \"%s\"", strerror ( errno ) );
        return -1;
        }
    *Out_Data = Result & 0xFFFF;
    return 0;
    }*/

int8_t I2CDevice_WriteDataToAddress ( SI2CDevice *In_Device, uint8_t In_Address, uint8_t *In_Data, uint8_t In_Length )
    {
#ifdef ATMEGA
    UNUSED ( In_Device );
    uint8_t Result = TWIStart ( In_Device->DeviceAddress, I2C_WRITE );
    if ( Result != 0 )
        return Result;
    Result = TWIWrite ( In_Address );
    if ( Result != 0 )
        return Result;
    uint8_t cont;
    for ( cont = 0; cont < In_Length; ++cont )
        {
        Result = TWIWrite ( In_Data [cont] );
        if ( Result )
            return Result;
        }
    Result = TWIStop ();
    if ( Result != 0 )
        return Result;
    return 0;
#else
    int Result = i2c_smbus_write_i2c_block_data ( In_Device->BusFileDescriptor, In_Address, In_Length, In_Data );
    if ( Result != 0 )
        {
        LOG_ERROR ( "Unable to write data. \"%s\"", strerror ( errno ) );
        return -1;
        }
    return 0;
#endif
    }

int8_t I2CDevice_ReadDataFromAddress ( SI2CDevice *In_Device, uint8_t In_Address, uint8_t *Out_Data, uint8_t In_Length )
    {
#ifdef ATMEGA
    UNUSED ( In_Device );
    uint8_t Result = TWIStart ( In_Device->DeviceAddress, I2C_WRITE );
    if ( Result != 0 )
        return Result;
    Result = TWIWrite ( In_Address );
    if ( Result != 0 )
        return Result;
    TWIStop ( );
    Result = TWIStart ( In_Device->DeviceAddress, I2C_READ );
    if ( Result != 0 )
        return Result;
    unsigned cont;
    for ( cont = 0; cont < In_Length; ++cont )
        {
        Result = TWIRead ();
        Out_Data[cont] = Result;
//     LOG_MSG ( "Reading %d/%d. %02X %s", cont + 1, In_Length, Result, TWCRToString() );
        }
    Result = TWIStop ();
    if ( Result != 0 )
        return Result;
    return 0;
#else
    int Result = i2c_smbus_read_i2c_block_data ( In_Device->BusFileDescriptor, In_Address, In_Length, Out_Data );
    if ( Result < 0 )
        {
        LOG_ERROR ( "Unable to read data. \"%s\"", strerror ( errno ) );
        return -1;
        }
    return 0;
#endif
    }
