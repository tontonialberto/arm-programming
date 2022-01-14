#include <MKL25Z4.h>

// I2C0 SDA, SCL pins on PTE0, PTE1
const static uint32_t PIN_SDA = 0;
const static uint32_t PIN_SCL = 1;

const static uint8_t ADDR_MPU6050 = 0x68;
const static uint8_t REG_WHOAMI = 0x75;

// BUSY: Another master is using the channel
// NACK: Slave didn't respond to the last byte
// ARB_LOST: Another master started transmission.
enum I2C_WriteResult {
	SUCCESS = 0, NACK, ARB_LOST, BUSY
};

typedef enum I2C_WriteResult I2C_WriteResult;

I2C_WriteResult I2C_Write(
	I2C_Type *i2c,
	uint8_t address,
	uint8_t *data,
	uint32_t len);

static uint8_t buffer[20];
static volatile I2C_WriteResult i2cresult;

int main() {
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// I2C0 pins on MUX=6 (that is, I2C0)
	// Enable pull-up resistors
	PORTE->PCR[PIN_SDA] &= ~PORT_PCR_MUX_MASK 
		& ~PORT_PCR_PS_MASK 
		& ~PORT_PCR_PE_MASK;
	PORTE->PCR[PIN_SDA] |= PORT_PCR_MUX(6)
		| PORT_PCR_PE(1)
		| PORT_PCR_PS(1);
	PORTE->PCR[PIN_SCL] &= ~PORT_PCR_MUX_MASK 
		& ~PORT_PCR_PS_MASK 
		& ~PORT_PCR_PE_MASK;
	PORTE->PCR[PIN_SCL] |= PORT_PCR_MUX(6)
		| PORT_PCR_PE(1)
		| PORT_PCR_PS(1);
	
	// Disable I2C0
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	
	// Setting I2C0 baud rate at 500 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be 48 => ICR = 0xd
	I2C0->F = I2C_F_MULT(0) | I2C_F_ICR(0xd);
	
	// Enable I2C0, interrupts disabled
	I2C0->C1 |= I2C_C1_IICEN_MASK;
	
	while(1) {
		buffer[0] = REG_WHOAMI;
		i2cresult = I2C_Write(I2C0, ADDR_MPU6050, buffer, 1);
		
		// I2C_Read(I2C0, 0x68, buffer, 1);
		// Read buffer[0]
	}
}

I2C_WriteResult I2C_Write(
		I2C_Type *i2c,
		uint8_t address,
		uint8_t *data,
		uint32_t len) {
	
	I2C_WriteResult result = SUCCESS;
			
	if(i2c->S & I2C_S_BUSY_MASK) {
		result = BUSY;
	}
	
	if(SUCCESS == result) {
		// Master mode, transmit
		i2c->C1 |= I2C_C1_MST(1) | I2C_C1_TX(1);
		
		// Specify the slave address, write mode
		i2c->D = (uint8_t)(address << 1) | 0x0;
		
		// Wait completion of address transmission
		while(! (i2c->S & I2C_S_TCF_MASK) );
		
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
	
	if(SUCCESS == result) {
		// Transmit the first data byte
		i2c->D = data[0];
		
		// Wait completion of data transmission
		while(! (i2c->S & I2C_S_TCF_MASK) );
		
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
