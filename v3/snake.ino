#include "LED_matrix.h"

// data pin 13, latch pin 12, clock pin 11
LED_matrix screen(13, 12, 11);

// left, down, right, up
int button_pins[4] = {
    A2,
    A3,
    A4,
    A5
};
// buttons are pulled up so 0 = pressed, 1 = not pressed
int button_states[4] = {
    1,
    1,
    1,
    1
};

// 0 left, 1 down, 2 right, 3 up, 4 static
int dir = 4;
// dx[dir] and dy[dir] are x,y changes
int dx[5] = {
    -1,
    0,
    1,
    0,
    0
};
int dy[5] = {
    0,
    1,
    0,
    -1,
    0
};

// x, y
int food_pos[2] = {
    5,
    5
};

// stores coords (x,y) as y*8+x
int snake[64];
int snake_len = 1;

// store an input queue to register every button press
int input_queue_len = 0;
int input_queue[20];

// game speed
double tickstep = 20;
int tick = 0;

bool dead = false;

void setup() {
    int grid[8][8] = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    screen.Set(grid);
    snake[0] = 42;

    for (int i = 0; i < 4; i++) {
        pinMode(button_pins[i], INPUT_PULLUP);
    }

    Serial.begin(9600);
}

void loop() {

    if (dead) {
        screen.Draw();
        return;
    }

    // process input
    for (int i = 0; i < 4; i++) {
        if (!digitalRead(button_pins[i]) && button_states[i]) {
            input_queue[input_queue_len] = i;
            input_queue_len++;
            break;
        }
    }
    for (int i = 0; i < 4; i++) {
        button_states[i] = digitalRead(button_pins[i]);
    }

    // timing
    tick++;
    if (tick >= tickstep) {
        screen.SetPixel(food_pos[0], food_pos[1], 0);
        if (logic_update()) return;
        tick = 0;
    } else if (tick >= tickstep / 2) {
        // food flashes at twice the update speed
        screen.SetPixel(food_pos[0], food_pos[1], 1);
    }

    screen.Draw();
}

int logic_update() {
    // process input
    if (input_queue_len > 0) {
        // if direction to change to isn't opposite current dir
        if (dir == 4 || (dir % 2 != input_queue[0] % 2)) {
            dir = input_queue[0];
        }
        input_queue_len--;
        for (int i = 0; i < input_queue_len; i++) {
            input_queue[i] = input_queue[i + 1];
        }
    }

    // check if dead
    int nextX = snake[0] % 8 + dx[dir];
    int nextY = snake[0] / 8 + dy[dir];
    if (nextX < 0 || nextY < 0 || nextX > 7 || nextY > 7) {
        die();
        return 1;
    }
    for (int i = 1; i < snake_len; i++) {
        if (snake[i] % 8 == nextX && (int)(snake[i] / 8) == nextY) {
            die();
            return 1;
        }
    }

    // check food
    bool grow = nextX == food_pos[0] && nextY == food_pos[1];
    if (grow) {
        snake_len++;
        // speed up game
        tickstep -= 0.5;
    }

    // update snake pos
    if (!grow) {
        screen.SetPixel(snake[snake_len - 1] % 8, (int)(snake[snake_len - 1] / 8), 0);
    }
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = nextY * 8 + nextX;
    screen.SetPixel(snake[0] % 8, (int)(snake[0] / 8), 1);

    // update food pos
    if (grow) {
        reposition_food();
    }

    return 0;

}

void reposition_food() {
    int valid_positions[64];
    int num_valid = 0;
    for (int i = 0; i < 64; i++) {
        // check if i is in the snake
        bool valid = true;
        for (int j = 0; j < snake_len; j++) {
            if (snake[j] == i) {
                valid = false;
                break;
            }
        }
        if (valid) {
            valid_positions[num_valid] = i;
            num_valid++;
        }
    }
    int chosen = valid_positions[rand() % num_valid];
    food_pos[0] = chosen % 8;
    food_pos[1] = chosen / 8;
}

void die() {
    int grid[8][8] = {
        {0,0,0,0,0,0,0,0},
        {0,1,0,0,0,0,1,0},
        {0,1,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,0},
        {0,1,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0},
    };
    screen.Set(grid);
    for (int i = 0; i < 100; i++)
        screen.Draw();

    // show score
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            screen.SetPixel(x, y, y * 8 + x < snake_len);
        }
    }
    screen.Draw();
    dead = true;

}
