#include "LED_matrix.h"
#include "Arduino.h"

LED_matrix::LED_matrix(int data, int latch, int clock): data_pin(data), latch_pin(latch), clock_pin(clock) {
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);
}

void LED_matrix::SetPixel(int x, int y, int val) {
    grid[y][x] = val;
}

void LED_matrix::Set(int grid[8][8]) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            this -> grid[y][x] = grid[y][x];
        }
    }
}

void LED_matrix::Draw() {
    // scan row by row, activating needed columsn
    for (int y = 0; y < 8; y++) {
        // activate row
        bitSet(reg_data[row_mappings[y] > 7], row_mappings[y] % 8);

        // activate columns
        for (int x = 0; x < 8; x++) {
            if (grid[y][x]) {
                bitClear(reg_data[col_mappings[x] > 7], col_mappings[x] % 8);
            } else {
                bitSet(reg_data[col_mappings[x] > 7], col_mappings[x] % 8);
            }
        }

        Latch();
        // persistence of vision
        delay(1);

        // deactivate row
        bitClear(reg_data[row_mappings[y] > 7], row_mappings[y] % 8);

        // deactivate columns
        for (int x = 0; x < 8; x++) {
            bitSet(reg_data[col_mappings[x] > 7], col_mappings[x] % 8);
        }
    }
}

void LED_matrix::Latch() {
    digitalWrite(latch_pin, LOW);
    shiftOut(data_pin, clock_pin, MSBFIRST, reg_data[0]);
    shiftOut(data_pin, clock_pin, MSBFIRST, reg_data[1]);
    digitalWrite(latch_pin, HIGH);
}
