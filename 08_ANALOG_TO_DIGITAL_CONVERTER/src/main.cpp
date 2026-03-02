#include <avr/io.h>
#include <avr/interrupt.h>


void setup() {
DDRD |= (1<<PD6); //Make Pin 6 output for the LED.PIN 6 is for PWM

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
  ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));

  As, u can see the above code simply equate MUX3 to MUX0 as zero.
  THE analog pin used in this project is A0. THose MUXx bits are what define it.
  defing MUX3-MUX0 as zero, means we want to use A0 as the ADC pin.
 */ 
 ADCSRA |= (1<<ADEN);/*
 ACDSRA (ADC Control and Status Register A) is used to enable the ADC (THAT IS, IT TURNS ON THE ADC) IN THIS CODE
 */
 ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADIE);/*
 We made ADIE(ADC INTERRUPTS ENABLE) pin to be 1 to enable interrupts for ADC. 
 The interrupts is for when the ADC conversion is done (it takes some clock cycles)
 And we want to read the values automatically, so we just enabled the interrupts for it.

sO, THE ADCSRA registers needs a freq between 50-200 Khz to operates smoothly over a 10bit resolution.(The ADC is a 10bit resolution register. Higher resolution means better conversion)
SO, we used ADPPS2:0 to select the prescaler we need. 
In this case,we needed a prescaler of 124,and from the table in the datasheet, we need ADPS2:0 to be 1
 */

 TCCR0A |= (1<<WGM00) | (1<<WGM01) | (1<<COM0A1);/*
 Enale TIMER COUNTER CONTROL REGISTER A to turns on the type of PWM (or wave) which in this case is fast PWM.
 WE also use TCCR0A TO SHOW WHAT IS TO BE DONE when the timer hits the number in the OCCRO0A register.
 check project 7 to check on timers.
 */
 TCCR0B |= (1<<CS00) | (1<<CS01);/*
 TIMER COUNTER CONTROL REGISTER B is used to select the prescaler which in this case is 64.
 */


ADCSRA |= (1<<ADSC);//This is to start the conversion by enabling ADSC (ADC START CONVERSION). This does it just 1 time
 sei();//tHIS IS A GLOBAL INTERUPTS VAriable to enable the ADC interrupt.
}

ISR (ADC_vect)/*
ADC interrupt function
*/ {
int sensor = ADC; //We are reading the value of the conversion. The name "ADC" is a macro that reads out the value of the conversion
int converted_sensor = (sensor>>2);/*
When ever u  want to divide in bare-metal, do not use division symbol. it wastes clock cycles.
instead use the right shift operator. 
for every bit shifted to the right, it means division by 2^the number of time shifted.
in this case ours is 2^2=4. This is exponential.

the reason for the calculation is to convert the ADC value into the analog duty cycle equivalent.
remember,that our timers is 8 bits, and the it moves from 0-255
remember ADC converts analog to digital, now we want tpo convert digital to analog duty cycle
*/
OCR0A = converted_sensor;/*
Based on the voltage output from the ADC,we create a duty cycle.

since the highest voltage here is Vcc(5v), the ADC equivalent of 5v is 1024bit WHICH IS CONVERTED to 5v again.
if we feed 5v into OCR01,we are asking for a 100% duty cycle.
why the hassle of conversion cos, we are reading from a potentiometer, and whatever value we turns the potentiometer to, we want the LED to output light at that voltage.
so,i can turn the potentiometer to output 2.5V,the ADC will read it and convert it to 512 bits.
the goal is to give the LED 2.5v,so we divide the 512 bits by 4 to get 128.
remember 128 is half of 255 (i am talking about the 8 bits registers). 
This is a 50% duty cycle.
so,we send a 505 PWM to the led,and 50% of 5v is 2.5v, the LED is at 2.5v (this figuratively speaking. the led is not really at 2.5v. it's our eys deceiving us. check the project on pwm for more details)
*/
ADCSRA |= (1<<ADSC);/*
This code here is to create an infinite loop of starting thr ADC conversion.
u know the above line was written in setup function, and i said it runs 1 meaning only 1 conversion
After,the conversion is done, the interrupt is triggered,then it does all it works to make the LED light up in the equivalent
immediately the led lights up. it supposed to ends
But due to us writing "ADCSRA |= (1<<ADSC)" we are starting another conversion immediaterly
this creates an infinite loop of conversion.
*/
}

void loop() {

}


int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
  //the above code is to simply creates an infinite loop
}