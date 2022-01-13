#include <MKL25Z4.h>
#include <math.h>

// Connect external potentiometer, or KY-013 temperature sensor, on PTE22
const static uint32_t PIN_ANALOG = 22;

volatile static uint16_t adcValue = 0;

volatile static double volts = 0;

volatile static double celsiusTemperature = 0;

// ADC0_SE3 corresponds to the pin previously defined
const static uint32_t ADC_CHANNEL = 3;

// Conversion function, from ADC raw value to voltage in range [vMin, vMax]
// adcSteps: 2^Resolution
// vMin: Vref-
// vMax: Vref+
static double adcToVoltage(uint16_t value, uint32_t adcSteps, double vMax, double vMin) {
	return (vMax - vMin) * value / adcSteps;
}

// Conversion function, from ADC raw value to temperature in Celsius.
// Note that this conversion function is specific to the KY-013 thermistor.
static double adcToCelsius(uint16_t value, uint32_t adcSteps) {
	
	// Steinhart-Hart coefficients for the KY-013 thermistor.
	// I've just found them online...
	const double c1 = 0.001129148, 
		c2 = 0.000234125, 
		c3 = 0.0000000876741;
	
	// Convert ADC value to Ohm
	const double currentR = 10000.0 * ( ((double)adcSteps / (double)value) - 1.0 ); 
	const double logR = log(currentR);
	
	// Compute 1/T in Kelvin 
	const double invKelvin = c1 + (c2 * logR) + (c3 * logR * logR * logR);
	const double celsius = (1.0 / invKelvin) - 273.15;
	
	return celsius;
}

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[PIN_ANALOG] &= ~PORT_PCR_MUX_MASK;
	
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	// Activate software trigger
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
	
	// 16-bit conversion, clock source is bus clock, short sample time
	ADC0->CFG1 = ADC_CFG1_MODE(3);
	
	while(1) {
		// Select single-ended mode, ADC0_SE3 on PTE22, disable interrupts.
		// This also starts the conversion.
		ADC0->SC1[0] = ADC_SC1_ADCH(ADC_CHANNEL);
		
		// Wait conversion complete
		while(! (ADC0->SC1[0] & ADC_SC1_COCO_MASK));
		
		// Read value from ADC
		adcValue = (uint16_t)ADC0->R[0];
		
		// Convert the ADC value in [0 - 3.3V] range
		volts = adcToVoltage(adcValue, 65536, 3.3, 0);
		
		celsiusTemperature = adcToCelsius(adcValue, 65536);
	}
}
