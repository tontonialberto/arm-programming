#include <MKL25Z4.h>

// I2C1 SDA, SCL pins on PTE0, PTE1
const static uint32_t PIN_SDA = 0;
const static uint32_t PIN_SCL = 1;

const static uint8_t ADDR_MPU6050 = 0x68;
const static uint8_t REG_WHOAMI = 0x75;

const static uint8_t I2C_READ = 0x1;
const static uint8_t I2C_WRITE = 0x0;

// BUSY: Another master is using the channel
// NACK: Slave didn't respond to the last byte
// ARB_LOST: Another master started transmission.
enum I2C_WriteResult {
	SUCCESS = 0, NACK, ARB_LOST, BUSY
};

typedef enum I2C_WriteResult I2C_WriteResult;

I2C_WriteResult I2C_SendAddress(
	I2C_Type *i2c,
	uint8_t address,
	uint8_t mode // 0x1 = read, 0x0 = write
);

I2C_WriteResult I2C_Write(
	I2C_Type *i2c,
	uint8_t address,
	uint8_t *data,
	uint32_t len,
	uint8_t restart);

I2C_WriteResult I2C_Read(I2C_Type *i2c,
		uint8_t address, uint8_t *out);

static uint8_t buffer[20];
static uint8_t readByte;
static volatile I2C_WriteResult i2cresult;

int main() {
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// pins on MUX=6 (that is, I2C1)
	// Enable pull-up resistors
	/*
	PORTE->PCR[PIN_SDA] &= ~PORT_PCR_MUX_MASK 
		& ~PORT_PCR_PS_MASK 
		& ~PORT_PCR_PE_MASK;
	*/
	PORTE->PCR[PIN_SDA] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	/*
	PORTE->PCR[PIN_SCL] &= ~PORT_PCR_MUX_MASK 
		& ~PORT_PCR_PS_MASK 
		& ~PORT_PCR_PE_MASK;
	*/
	PORTE->PCR[PIN_SCL] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	
	// Disable I2C module
	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	
	// Setting I2C1 baud rate at approx 200 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be approx 120 => ICR = 0x1b
	I2C1->F = I2C_F_MULT(0) | I2C_F_ICR(0x1b);
	
	// Enable I2C0, interrupts disabled
	I2C1->C1 |= I2C_C1_IICEN_MASK;
	
	while(1) {
		buffer[0] = REG_WHOAMI;
		i2cresult = I2C_Write(I2C1, ADDR_MPU6050, buffer, 1, 0);
		
		i2cresult = I2C_Read(I2C1, ADDR_MPU6050, &readByte);
	}
}

I2C_WriteResult I2C_SendAddress(
		I2C_Type *i2c,
		uint8_t address,
		uint8_t mode) {
	
	I2C_WriteResult result = SUCCESS;
	mode = (uint8_t)(mode & 0x1); // Ensure that only the LSB is considered
			
	if(i2c->S & I2C_S_BUSY_MASK) {
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

I2C_WriteResult I2C_Write(
		I2C_Type *i2c,
		uint8_t address,
		uint8_t *data,
		uint32_t len,
		uint8_t restart) {
	
	I2C_WriteResult result = I2C_SendAddress(i2c, address, I2C_WRITE);
	
	if(SUCCESS == result) {
		// Transmit the first data byte
		i2c->D = data[0];
		
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
		if(i2c->S & I2C_S_RXAK_MASK) {
			result = NACK;
		}
		
		// Issue a stop condition
		i2c->C1 &= ~I2C_C1_MST_MASK & ~I2C_C1_TX_MASK;
	}
		
	return result;
}
		
I2C_WriteResult I2C_Read(I2C_Type *i2c,
		uint8_t address, uint8_t *out) {
	
	I2C_WriteResult result = I2C_SendAddress(i2c, address, I2C_READ);

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
