
# ArduinoSnake

This was my Arkwright Engineering Scholarship project. Images are in the images/ directory. The completed project files are in this directory.

## Project Description

**Goal**

I aimed to create the game "snake" playable on a breadboard, displayed on an LED matrix and using an arduino as a microcontroller. The project was broken into three stages - getting the LED matrix to work (v1), final wiring (v2) and completed (v3).

**Development process**

The v1 wiring was clumsy as can be seen in Image 3 and the main goal was to get the LED matrix to display a smiling face. The .ino file for v1 can be seen in Image 2 - the code is inefficient however this was refactored in Image 7. Then in v2 the wiring was significantly cleaned up (see Images 4, 5 and 8), and in v3 the game itself was implemented in code (the matrix code was also put into an external class (see Image 6) to make the code more readable, and the wires were cut to the right length).I had not used shift registers and LED matrices before so had to research the respective datasheets for the specific parts I had and learn how they worked in depth.Two shift registers were daisy-chained in order to reduce the number of I/O pins needed on the arduino - the shift registers were initially separate but while reading the datasheet I discovered that they could be treated as a single component by feeding the output of one into the other, saving a further 3 pins.

**Successes and failures**
- The original goal of the project was achieved fully.
- I struggled to figure out which rows and columns corresponded to each matrix pin, and had to reverse engineer this by brute forcing individual pins then testing their order.
- Many code bugs were present, especially an issue with specific LEDs lighting spontaneously (see Image 1).
- The buttons can be uncomfortable to use.

**Engineering lessons learnt**
- Due to my constant refactoring of code / component rewiring in each stage, I have learnt the importance of breaking a project up into manageable stages and that there is always a more efficient way to do things.
- I have gained an in depth knowledge of how shift registers and LED matrices work, and have also enjoyed the problem solving of trying to find the cleanest way to wire the components.

## Current working state explanation

**LED Matrix + shift register**
- Each frame, the pins of the matrix which need to be turned on/off are encoded in the bits of a 16-bit integer​
- The bits of this integer are then shifted out through the daisy-chained shift registers, so one bit is sent to each output register – each of which is wired to a matrix pin
- The latch pin is set low, serialized data is then sent through the data pin and then the latch pin is set high which flushes the registers​
- Each matrix pin turns a row or column high/low - to turn on an LED the row pin is set high and the column pin is set low​
- A persistence of vision illusion is used - we loop through each row, setting the column pins high/low​ with a small delay so it seems like a constant image to the human eye

**Snake**
- A classic snake game was implemented on top of the LED matrix code
- The longer the snake, the faster it moves ( creates increasing difficulty )
- The food pixel flashes at twice the movement speed so it is always distinguishable
- The player dies if they move off the screen or into their own body
- After death, the score is shown
