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
    if (mpu->whoAmI())
    {
        printf("WhoAmI was okay\n");
        if (mpu->getBypassEnable())
        {
            printf("Bypass is ENABLED\n");
        }
        else
        {
            printf("Bypass is DISABLED\n");
        }
        printf("Toggling...\n");
        mpu->setBypassEnable(mpu->getBypassEnable());
        return 1;
    }
    return 0;
}