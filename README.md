# ArduinoSnake

(This was my arkwright interview presentation)

**Goal**: create a version of the game Snake, fully playable on a breadboard

**Achieved**: gained a deep understanding of shift registers + datasheet reading skills, satisfaction of a working final result

**Challenges**: reverse engineering components, arranging the wiring on the breadboard in an efficient way

## Overview

**LED Matrix + shift register**:
- Each frame, the pins of the matrix which need to be turned on/off are encoded in a 16-bit integer​
- Shifted out through the daisy-chained shift registers, so one bit is sent to each output – each of which is wired to a matrix pin as seen in the picture​
- The latch pin is set low, serialized data is then sent through the data pin and then the latch pin is set high which clears the registers and the data is output​
- Each matrix pin turns a row or column high/low - to turn on an LED the row pin is set high and the column pin is set low​

For each row, the needed column pins are set high/low​

**Snake**: A classic snake game was implemented on top of the LED matrix code – note the food pixel flashes at twice the movement speed so it is always distinguishable

## Creation Process
- The project was split up into manageable chunks: matrix, shift registers, snake​
- Hardest part was shift registers/matrix - have implemented snake before​
- Looked up the serial numbers of each component to find their datasheets​
- Had to reverse engineer which pins on the arduino mapped to which rows/columns on the physical LED matrix by using a debugging circuit and following wires
