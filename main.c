#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DISP_DDR		DDRB				///< DDR for Seven Segment Display..
#define DISP_PORT		PORTB				///< Port for Seven Segment Display.

void PORT_INIT(void);
void INT_INIT(void);

volatile uint8_t direction = 0;

/*!
 *	@brief ISR for INT0 external interrupt; change direction of flow of the counter. 
 */

ISR(INT0_vect){
	if(!(direction)){
		direction = 1;
	}
	else{
		direction = 0;
	}
}

int main(void){
	PORT_INIT();
	INT_INIT();
	
	uint8_t ssd[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	///< Array to hold values of segments corresponding to each digit.
	uint8_t value = 0;
	
	sei();									///< Enable global interrupts.
	
	while(1){
		if(!(direction)){
			DISP_PORT = ssd[value];
			value++;
			if(value > 9){
				value = 0;
			}
			_delay_ms(100);
		}
		else{
			value--;
			DISP_PORT = ssd[value];
			if(value <= 0){
				value = 10;
			}
			_delay_ms(100);
		}
	}
}

/*!
 *	@brief Initialize Ports.
 */

void PORT_INIT(void){
	DISP_DDR = 0xFF;
	DDRD &= ~(1<<PD2);						///< Set INT0 pin to input.
}

/*!
 *	@brief Initialize External Interrupt 0 at INT0.
 */

void INT_INIT(void){
	MCUCR |= (1<<ISC00)|(1<<ISC01);		///< A rising edge at INT0 generates an interrupt request.
	GICR |= (1<<INT0);						///< Enable INT0 external interrupt request.
}