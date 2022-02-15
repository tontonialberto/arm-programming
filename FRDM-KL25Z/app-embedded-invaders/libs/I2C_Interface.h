#ifndef __ALBERTO_TONTONI_I2C_INTERFACE__
#define __ALBERTO_TONTONI_I2C_INTERFACE__
enum I2C_Result {
	I2C_RESULT_SUCCESS = 0, I2C_RESULT_NACK, I2C_RESULT_ARB_LOST, I2C_RESULT_BUSY
};

typedef enum I2C_Result I2C_Result;
#endif
