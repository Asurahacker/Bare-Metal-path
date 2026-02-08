#include <Arduino.h> //Still depending on this guy. refer to project 1 to know why we use him.

void setup() {
DDRD |= 0b11111100; 
/*
So, the goal of the 2 lines of code in which i sandwich this comment between is to simply make some group of bits in a register equals to 1(meaning turning them to OUTPUT)
The code "DDRD |= 0b11111100; " is to simply make pin D2 to pin D7 of an arduino an output.
pin D2 to pin D7 are bits presents in register DDP (Data DIrection Register D)
SAME THING applies to the code below "DDRB |= 0b00000011;", only difference is that, it is in Data DIrection Register B(DDRB).

we use the keyword "0b" before 0b11111100 or 0b00000011, to let the arduino or compiler knows we are dealing with binary values.
as we know,the registers in arduino are 8 bits. so the binary value digits are 8 in number.
         0b                   1111              1100
          |                     |                 |
   (Binary definer)     (Pin 4 to 7 are high)  (pin 2-3 are high)
so, we set both DDRD and DDRB to OUTPUT MODE.
*/
DDRB |= 0b00000011;// read the above

}

void loop () {

  for (int i=2;i<8;i++)
  /*
  To use a "for loop" to loop from Pin 2 to Pin 8, and increment itself by 1

  To understand how for loops works, check Google/Youtube/AI for better teachers

  */
  {
    PORTD |= (1<<i);
    /*
    what ever values i is (the first value for i is 2), a bitwise operation will be peformed on PORTD.
    check project 1 for understanding of the logic behind this.
    */
    delay(80);//delay for 80 milli seconds
    PORTD &= ~(1<<i); //we turned off the LED we just turned on after 80 milli seconds. check project 1 for the logic
   
    
  }
  for (int i=0;i<2;i++) {
    PORTB |= (1<<i);
    delay(80);
    PORTB &= ~(1<<i);
   
    /*
    All that is in this for loop, is the same as above only that it is for PORTB (pin 0 and pin 1)
    */

  }

}