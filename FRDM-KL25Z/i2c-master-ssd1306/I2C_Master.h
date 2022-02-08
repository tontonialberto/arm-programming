#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <MKL25Z4.h>

const static uint8_t I2C_READ = 0x1;
const static uint8_t I2C_WRITE = 0x0;

// BUSY: Another master is using the channel
// NACK: Slave didn't respond to the last byte
// ARB_LOST: Another master started transmission.
enum I2C_Result {
	SUCCESS = 0, NACK, ARB_LOST, BUSY
};

typedef enum I2C_Result I2C_Result;

I2C_Result I2C_Write(
	I2C_Type *i2c,
	uint8_t address,
	uint8_t *data,
	uint32_t len,
	uint8_t restart);

I2C_Result I2C_Read(I2C_Type *i2c,
		uint8_t address, uint8_t *out);

/*
	Perform read burst from the I2C slave,
	which basically means:
	1) Write the control byte to the address, issue a restart condition
	2) Switch to read mode, and read up to len bytes from the slave
	
WARNING: for single-byte read, this doesn't work! Use I2C_Read() instead.
*/
I2C_Result I2C_ReadBurst(
	I2C_Type *i2c,
	uint8_t address,
	uint8_t control,
	uint8_t *data,
	uint32_t len);

static I2C_Result I2C_SendAddress(
	I2C_Type *i2c,
	uint8_t address,
	uint8_t mode // 0x1 = read, 0x0 = write
);

I2C_Result I2C_Write(
		I2C_Type *i2c,
		uint8_t address,
		uint8_t *data,
		uint32_t len,
		uint8_t restart) {
	
	I2C_Result result = I2C_SendAddress(i2c, address, I2C_WRITE);
	
	for(uint32_t i=0; i<len; i++) {
		if(SUCCESS == result) {
			// Transmit the next data byte
			i2c->D = data[i];
			
			// Wait completion of data transmission
			while(! (i2c->S & I2C_S_IICIF_MASK) );
			i2c->S |= I2C_S_IICIF_MASK;
			
			// Check lost of arbitration
			if(i2c->S & I2C_S_ARBL_MASK) {
				i2c->S |= I2C_S_ARBL_MASK; // Clear ARBL flag
				result = ARB_LOST;
			}
		}
		
		if(SUCCESS == result) {
			// Check ack bit from slave
			if(i2c->S & I2C_S_RXAK_MASK)
				result = NACK;
		}
	}
		
	if(restart) {
		// Issue a repeated start condition
		i2c->C1 |= I2C_C1_RSTA_MASK;
	}
	else {
		// Issue a stop condition
		i2c->C1 &= ~I2C_C1_MST_MASK & ~I2C_C1_TX_MASK;
	}
		
	return result;
}
		
I2C_Result I2C_Read(I2C_Type *i2c,
		uint8_t address, uint8_t *out) {
	
	I2C_Result result = I2C_SendAddress(i2c, address, I2C_READ);

	if(SUCCESS == result) {
		
		// Set I2C master on receiver mode
		i2c->C1 &= ~I2C_C1_TX_MASK;
		
		// Send NACK on received byte
		i2c->C1 |= I2C_C1_TXAK_MASK;
	
		// Start receiving by accessing the IO register.
		// If you don't do this, receiving does not start!
		*out = i2c->D;
		
		// Wait until the slave sends a byte
		while(! (i2c->S & I2C_S_IICIF_MASK) );
		i2c->S |= I2C_S_IICIF_MASK;
		
		// Exit from rx mode before reading the byte
		i2c->C1 |= I2C_C1_TX_MASK;
		
		*out = i2c->D;
	
		// Issue stop condition
		i2c->C1 &= ~I2C_C1_MST_MASK & ~I2C_C1_TX_MASK;
	}
			
	return result;
}
		
I2C_Result I2C_ReadBurst(
		I2C_Type *i2c,
		uint8_t address,
		uint8_t control,
		uint8_t *data,
		uint32_t len) {
	
	uint8_t dummy[1];
	I2C_Result result;
			
	if(len == 0) return SUCCESS; // No data to read, don't even start I2C communication
			
	dummy[0] = control; // Using array to comply with I2C_Write interface
	result = I2C_Write(i2c, address, dummy, 1, 1); 
			
	if(SUCCESS == result) {
		result = I2C_SendAddress(i2c, address, I2C_READ);
	}
			
	if(SUCCESS == result) {
		// Set I2C master on receiver mode
		i2c->C1 &= ~I2C_C1_TX_MASK;
		
		// Send ACK on next byte
		i2c->C1 &= ~I2C_C1_TXAK_MASK;
		
		// If there's only 1 byte, send NACK on receiving
		if(len == 1) {
			i2c->C1 |= I2C_C1_TXAK_MASK;
		}
		
		// Start receiving by accessing the IO register.
		// If you don't do this, receiving does not start!
		dummy[0] = i2c->D;
		
		for(uint32_t i=0; i<len; i++) {
			
			// Wait until the slave sends a byte
			while(! (i2c->S & I2C_S_IICIF_MASK) );
			i2c->S |= I2C_S_IICIF_MASK;
			
			if(i == len - 1) {
				// Exit from rx mode before reading the last byte
				i2c->C1 |= I2C_C1_TX_MASK;
			}
			
			data[i] = i2c->D;
			
			// Penultimate byte
			if(i == len - 2) {
				// Send NACK on next byte
				i2c->C1 |= I2C_C1_TXAK_MASK;
			}
		}
	
		// Issue stop condition
		i2c->C1 &= ~I2C_C1_MST_MASK & ~I2C_C1_TX_MASK;
	}
	
	return result;
}

static I2C_Result I2C_SendAddress(
		I2C_Type *i2c,
		uint8_t address,
		uint8_t mode) {
	
	I2C_Result result = SUCCESS;
	uint8_t isMaster;
	
	mode = (uint8_t)(mode & 0x1); // Ensure that only the LSB is considered		
	isMaster = I2C_C1_MST_MASK & i2c->C1;
	
	if( (!isMaster) && (i2c->S & I2C_S_BUSY_MASK) ) {
		result = BUSY;
	}
	
	if(SUCCESS == result) {
		// Master mode, transmit
		i2c->C1 |= I2C_C1_MST(1) | I2C_C1_TX(1);
		
		// Specify the slave address, and r/w mode
		i2c->D = (uint8_t)(address << 1) | mode;
		
		// Wait completion of address transmission
		while(! (i2c->S & I2C_S_IICIF_MASK) );
		i2c->S |= I2C_S_IICIF_MASK;
		
		// Check lost of arbitration
		if(i2c->S & I2C_S_ARBL_MASK) {
			i2c->S |= I2C_S_ARBL_MASK; // Clear ARBL flag
			result = ARB_LOST;
		}
	}
	
	if(SUCCESS == result) {
		// Check ack bit from slave (RXAK = 1 means NACK)
		if(i2c->S & I2C_S_RXAK_MASK) {
			result = NACK;
		}
	}
	
	return result;
}
		
#endif
