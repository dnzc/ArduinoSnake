#ifndef LED_MATRIX_H
#define LED_MATRIX_H

class LED_matrix {
    public:
        LED_matrix(int data, int latch, int clock);
        void SetPixel(int x, int y, int val);
        void Set(int grid[8][8]);
        void Draw();
    private:
        void Latch();
        int data_pin, latch_pin, clock_pin;
        int grid[8][8];
        // data for the two shift registers
        int reg_data[2] = {0, 0};
        // Q_A to Q_H of first register labelled 0-7, Q_A to Q_H of second labelled 8-15
        // index n is row/col n, val at index is corresponding Q_n
        int row_mappings[8] = {0, 1, 11, 6, 8, 12 ,13, 10};
        int col_mappings[8] = {2, 15, 14, 5, 9, 7, 4, 3};
};

#endif
