/**
 * Ken Hughes
 * 
 * C library for the MPU6050 gryo
 * Datasheet : https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 *
 */

#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>

#include "mpu6050.h"


void MPU6050::reset()
{
    unsigned char resetPacket[2] = {MPU6050_REG_PWR_MGMT_1, 0x80};
    writeRegister(resetPacket, 2);
}

bool MPU6050::whoAmI()
{
    if (readRegister8(MPU6050_REG_WHO_AM_I) == 0x68)
    {
        return true;
    }
    return false;
}


void MPU6050::writeRegister(unsigned char* data, int bytes)
{
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) 
    {
        printf("Error opening I2C channel\n");
    }
    else 
    {
        if (ioctl(i2cHandle, I2C_SLAVE, MPU6050_ADDR) < 0) 
        {
            printf("Error at ioctl\n");
        }
        else 
        {
            write(i2cHandle, data, bytes);
        }
        close(*deviceName);
    }
}

void MPU6050::readRegister(unsigned char* buffer, int bytes)
{
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) 
    {
        printf("Error opening I2C channel\n");
    }
    else 
    {
        if (ioctl(i2cHandle, I2C_SLAVE, MPU6050_ADDR) < 0) 
        {
            printf("Error at ioctl\n");
        }
        else 
        {
            int bytesRead = read(i2cHandle, buffer, bytes);
            if (bytes != bytesRead) 
            {
                printf("Error reading from I2C, expected %i bytes, but got %i\n", bytes, bytesRead);
            }
        }
        close(*deviceName);
    }
}

int MPU6050::readRegister8(unsigned char registerAddr)
{
    unsigned char buffer[1] = {registerAddr};
    writeRegister(buffer, 1); // send the register we are interested in
    readRegister(buffer, 1); // get the value from it
    return (int)buffer[0];
}

int MPU6050::readRegister16(unsigned char registerAddr)
{
    unsigned char buffer[2] = {registerAddr, 0};
    writeRegister(buffer, 1); // send the register we are interested in
    readRegister(buffer, 2); // get the value from it (HI byte) and the next (LO byte)
    int retVal = buffer[0] * 0xFF;
    retVal += buffer[1];
    return retVal;
}