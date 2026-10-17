# Digital-Dice-using-PIC18F4550


A simple digital dice built on the PIC18F4550 microcontroller. Press a push button and the program rolls a pseudo-random number from **1 to 6**, shown on a **7-segment display**. Randomness comes from a software **6-bit LFSR** (Linear Feedback Shift Register).

## Features
- Push-button triggered dice roll (RA2, active LOW)
- 6-bit LFSR pseudo-random number generator (polynomial x^6 + x + 1)
- Direct 7-segment drive from PORTB (common anode)
- Software debounce and wait-for-release, so one press gives one roll
- Display holds the last rolled number until the next press

## Hardware
| Component | PIC18F4550 Pin | Notes |
|---|---|---|
| Push button | RA2 (PORTA) | Active LOW, needs an external pull-up (about 10 kΩ to VDD) |
| 7-segment display (a to g, dp) | PORTB (RB0 to RB7) | Common anode, one current-limiting resistor per segment (220 Ω to 330 Ω) |

### 7-segment codes (common anode)
| Digit | Code |
|---|---|
| 1 | 0xF9 |
| 2 | 0xA4 |
| 3 | 0xB0 |
| 4 | 0x99 |
| 5 | 0x92 |
| 6 | 0x82 |

## How It Works
1. `ADCON1 = 0x0F` sets all pins as digital; `TRISB = 0x00` makes PORTB output; `TRISA = 0xFF` makes PORTA input.
2. The main loop polls RA2.
3. When the button is pressed (RA2 = 0), `lfsr6()` returns a number from 1 to 6.
4. The matching segment code is written to `LATB`.
5. `delay(100)` provides debounce and display hold, then the program waits for the button to be released.

## Build and Run
1. Open **MPLAB X IDE** and create a new project for the **PIC18F4550** (XC8 compiler).
2. Add `src/main.c` to the project.
3. Set the configuration bits (oscillator, watchdog off, etc.) for your board.
4. Build and program the device.

> The `delay()` duration depends on your oscillator frequency and compiler optimisation. Adjust the `time` argument if needed.

## Known Limitations
- The LFSR starts from the same seed (`0xA9`) every power-up, so the roll sequence is the same after each reset.
- A 6-bit LFSR repeats after at most 63 rolls, and `% 6` makes some faces very slightly more likely.
- Polling and busy-wait delay are not power efficient.
- No debounce delay after button release.
- `LATB` is not initialised, so the display may show a random pattern until the first press.

## Possible Improvements
- Seed the LFSR from Timer0 for different results on every power-up
- Add a debounce delay after release
- Initialise `LATB = 0xFF` to blank the display at start
- Use an external interrupt (INT0) instead of polling
- Add a second dice or a rolling animation

## Project Structure
```
pic18f4550-digital-dice/
├── src/
│   └── main.c
├── README.md
└── .gitignore
```

## License
MIT
