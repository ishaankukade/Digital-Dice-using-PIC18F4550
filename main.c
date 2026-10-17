/*
 * Digital Dice using PIC18F4550
 *
 * Press a push button (RA2, active LOW) to roll a dice.
 * A 6-bit LFSR generates a pseudo-random number (1-6),
 * which is shown on a common-anode 7-segment display on PORTB.
 */

#include <pic18f4550.h>

void delay(unsigned int time)
{
    unsigned int i, j;
    for(i = 0; i < time; i++)
        for(j = 0; j < 5000; j++);
}

// 6-bit LFSR-based random number generator (returns 1-6)
unsigned char lfsr6()
{
    static unsigned char lfsr = 0xA9; // nonzero seed value
    unsigned char feedback = ((lfsr >> 0) ^ (lfsr >> 1)) & 1; // polynomial x^6+x+1
    lfsr = (lfsr >> 1) | (feedback << 5);
    return (lfsr % 6) + 1;
}

void main(void)
{
    ADCON1 = 0x0F;      // Set all pins digital
    TRISB = 0x00;       // PORTB output for 7-seg
    TRISA = 0xFF;       // PORTA as input, RA2 is button
    LATA = 0x03;        // Optional init

    // 7-seg codes for digits 1-6 (common anode assumed)
    char c[] = {0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82};
    unsigned char number;

    while(1)
    {
        if(PORTAbits.RA2 == 0)  // Button pressed active low
        {
            number = lfsr6();          // Generate random number 1 to 6
            LATB = c[number - 1];      // Display digit on 7-seg

            delay(100);                // Display hold and debounce
            while(PORTAbits.RA2 == 0); // Wait for release
        }
    }
}
