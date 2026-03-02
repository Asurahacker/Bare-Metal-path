#include <avr/io.h>
#include <avr/interrupt.h>


void setup() {
DDRD |= (1<<PD6); //Make Pin 6 output for the LED

 ADMUX |= (1<<REFS0);/*
 The ADMUX (ADC Multiplexer Selection Register) is used to select the lowest and highest voltage. 
                    The max obtainable voltage is Vcc
  Also, there is an invisble code written here,which wasn't explicitly written, but the unwritten code is what helps us to choose which ADc pin we wnat.
  in this case, we want PIN A0 (note that there are 6 ADC pins on the nano)
  ANy of these ^ pins can be used for ADC.
  We are supposed to define what Pins we are using for the projec using the MUX table in the arduino datasheet
  But here it's not written cos, all MUX Pins are to be written to zero
  Due to the fact that on a default,arduino configure registers to ) automatically
  we did not need to define it.

  even if we define it, it's not an issue. The code is below:
  ADMUX |= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));

  As, u can see the above code simply equate MUX3 to MUX0 as zero.
  THE analog pin used in this project is A0. THose MUXx bits are what define it.
  defing MUX3-MUX0 as zero, means we want to use A0 as the ADC pin.
 */ 
 ADCSRA |= (1<<ADEN);/*
 ACDSRA (ADC Control and Status Register A) is used to enable the ADC (THAT IS, IT TURNS ON THE ADC) IN THIS CODE
 */
 ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADIE);/*
 
 */

 TCCR0A |= (1<<WGM00) | (1<<WGM01) | (1<<COM0A1);
 TCCR0B |= (1<<CS00) | (1<<CS01);


ADCSRA |= (1<<ADSC);
 sei();
}

ISR (ADC_vect) {
int sensor = ADC;
int converted_sensor = (sensor>>2);
OCR0A = converted_sensor;
ADCSRA |= (1<<ADSC);
}

void loop() {

}


int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}