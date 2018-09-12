/**
 * Ken Hughes
 * 
 * C library for the MPU6050 gryo
 * Datasheet : https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 *
 */



#define MPU6050_ADDR                  0x68

#define MPU6050_REG_GYRO_CONF         0x1B
#define MPU6050_REG_ACC_CONF          0x1C

#define MPU6050_REG_INT_PIN_CONF      0x37

#define MPU6050_REG_ACC_XOUT_H        0x3B
#define MPU6050_REG_ACC_XOUT_L        0x3C
#define MPU6050_REG_ACC_YOUT_H        0x3D
#define MPU6050_REG_ACC_YOUT_L        0x3E
#define MPU6050_REG_ACC_ZOUT_H        0x3F
#define MPU6050_REG_ACC_ZOUT_L        0x40
#define MPU6050_REG_TEMP_OUT_H        0x41
#define MPU6050_REG_TEMP_OUT_L        0x42
#define MPU6050_REG_GYRO_XOUT_H       0x43
#define MPU6050_REG_GYRO_XOUT_L       0x44
#define MPU6050_REG_GYRO_YOUT_H       0x45
#define MPU6050_REG_GYRO_YOUT_L       0x46
#define MPU6050_REG_GYRO_ZOUT_H       0x47
#define MPU6050_REG_GYRO_ZOUT_L       0x48

#define MPU6050_REG_PWR_MGMT_1        0x6B

#define MPU6050_REG_WHO_AM_I          0x75


typedef enum
{
    FS_GYRO_250DEG_S  = 0x00,
    FS_GYRO_500DEG_S  = 0x01,
    FS_GYRO_1000DEG_S = 0x02,
    FS_GYRO_2000DEG_S = 0x03
} fullScaleGyroRange;

typedef enum
{
    FS_ACCL_2G  = 0x00,
    FS_ACCL_4G  = 0x01,
    FS_ACCL_8G  = 0x02,
    FS_ACCL_16G = 0x03
} fullScaleAcclRange;

typedef enum
{
    CLK_INT_8MHZ       = 0x00,
    CLK_PLL_GYRO_X_REF = 0x01,
    CLK_PLL_GYRO_y_REF = 0x02,
    CLK_PLL_GYRO_Z_REF = 0x03,
    CLK_EXT_32KHZ      = 0x04,
    CLK_EXT_19MHZ      = 0x05,
    CLK_STOP           = 0x07,
} clockSource;


class MPU6050
{
    public:
        // MPU6050();
        // ~MPU6050();

        void reset(void);
        
        void setBypassEnable(bool);
        bool getBypassEnable(void);
        
        // clockSource getClockSource(void);
        // void setClockSource(clockSource);
        
        // fullScaleAcclRange getFullScaleAcclRange(void);
        // void setFullScaleAcclRange(fullScaleAcclRange);
        
        // fullScaleGyroRange getFullScaleGyroRange(void);
        // void setFullScaleGyroRange(fullScaleGyroRange);
        
        bool whoAmI(void);
    
    private:
        void readRegister(unsigned char*, int);
        int readRegister8(unsigned char);
        int readRegister16(unsigned char);
        void writeRegister(unsigned char*, int);
        void writeRegister8(unsigned char, unsigned char);
};