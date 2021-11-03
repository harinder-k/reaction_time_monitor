# reaction_time_monitor

This project uses an Arduino Uno with buttons and an LCD to have a user test their reaction time.

## Required

### Hardware

- Arduino Uno
- LCD 1602
- 2 Buttons
- Potentionmeter
- 2 10K Resistors

### Software

- Arduino IDE

## Circuit

1. Connect one terminal of each button to 5V and the other to a resistor leading to GND. One of these buttons will be the Start/Reset button (1) while the other (2) will be used to record reactions
2. Connect the output of (1) to Digital Pin 2 and (2) to Digital Pin 3.
3. Connect LCD 1602 with Arduino in the following manner:
https://content.instructables.com/ORIG/FC7/0BX0/JSCG2QWZ/FC70BX0JSCG2QWZ.jpg?auto=webp&frame=1&width=855&height=1024&fit=bounds&md=93d664f0b899a3c393dbf9752cbb94c5

## Instructions

1. After setting up circuit, upload code to board.
2. Follow instructions on LED (If nothing visible, adjust potentiometer), reacting to LED L on the board
