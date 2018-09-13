/**
 * Ken Hughes
 * 
 * Test app for the MPU6050 class
 *
 */

#include <stdio.h>
#include <string.h>


#include "mpu6050.h"

int main()
{
    MPU6050 *mpu = new MPU6050();
    mpu->setDebug(true);
    
    mpu->reset();
    if (mpu->whoAmI())
    {
        printf("WhoAmI was okay\n");
        
        // i2c bypass enabled
        mpu->setBypassEnable(true);
        printf("Set and Get BypassEnable to true - %s\n", mpu->getBypassEnable() ? "SUCCESS" : "FAILED");
        mpu->setBypassEnable(false);
        printf("Set and Get BypassEnable to false - %s\n", !mpu->getBypassEnable() ? "SUCCESS" : "FAILED");

        // gyro ranges
        mpu->setFullScaleGyroRange(fullScaleGyroRange::FS_GYRO_250DEG_S);
        printf("Set and Get FullScaleGyroRange to 250deg/sec - %s\n", (mpu->getFullScaleGyroRange() == fullScaleGyroRange::FS_GYRO_250DEG_S) ? "SUCCESS" : "FAILED");
        mpu->setFullScaleGyroRange(fullScaleGyroRange::FS_GYRO_500DEG_S);
        printf("Set and Get FullScaleGyroRange to 500deg/sec - %s\n", (mpu->getFullScaleGyroRange() == fullScaleGyroRange::FS_GYRO_500DEG_S) ? "SUCCESS" : "FAILED");
        mpu->setFullScaleGyroRange(fullScaleGyroRange::FS_GYRO_1000DEG_S);
        printf("Set and Get FullScaleGyroRange to 1000deg/sec - %s\n", (mpu->getFullScaleGyroRange() == fullScaleGyroRange::FS_GYRO_1000DEG_S) ? "SUCCESS" : "FAILED");
        mpu->setFullScaleGyroRange(fullScaleGyroRange::FS_GYRO_2000DEG_S);
        printf("Set and Get FullScaleGyroRange to 2000deg/sec - %s\n", (mpu->getFullScaleGyroRange() == fullScaleGyroRange::FS_GYRO_2000DEG_S) ? "SUCCESS" : "FAILED");

        // accelerometer ranges
        mpu->setFullScaleAccRange(fullScaleAccRange::FS_ACCL_2G);
        printf("Set and Get FullScaleAccRange to 2G - %s\n", (mpu->getFullScaleAccRange() == fullScaleAccRange::FS_ACCL_2G) ? "SUCCESS" : "FAILED");
        mpu->setFullScaleAccRange(fullScaleAccRange::FS_ACCL_4G);
        printf("Set and Get FullScaleAccRange to 4G - %s\n", (mpu->getFullScaleAccRange() == fullScaleAccRange::FS_ACCL_4G) ? "SUCCESS" : "FAILED");
        mpu->setFullScaleAccRange(fullScaleAccRange::FS_ACCL_8G);
        printf("Set and Get FullScaleAccRange to 8G - %s\n", (mpu->getFullScaleAccRange() == fullScaleAccRange::FS_ACCL_8G) ? "SUCCESS" : "FAILED");
        mpu->setFullScaleAccRange(fullScaleAccRange::FS_ACCL_16G);
        printf("Set and Get FullScaleAccRange to 16G - %s\n", (mpu->getFullScaleAccRange() == fullScaleAccRange::FS_ACCL_16G) ? "SUCCESS" : "FAILED");

        return 1;
    }
    return 0;
}