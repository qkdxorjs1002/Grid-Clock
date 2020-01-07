#include <SoftwareSerial.h>

#define _ROW_PIXEL_SIZE 10
#define _COL_PIXEL_SIZE 6

SoftwareSerial ESP01(A4, 7);

const int8_t pin_map_x[_COL_PIXEL_SIZE] = {A1, A2, A3, A5, 13, A0};
const int8_t pin_map_y[_ROW_PIXEL_SIZE] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12};

const int8_t digit_led[10][5][3] = {
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {0, 0, 1},
        {0, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
    }
};

void setup()
{
    Serial.begin(115200);
	ESP01.begin(115200);
    for (int8_t xdx = 0; xdx < _COL_PIXEL_SIZE; xdx++)
    {
        pinMode(pin_map_x[xdx], OUTPUT);
        digitalWrite(pin_map_x[xdx], LOW);
    }
    for (int8_t ydx = 0; ydx < _ROW_PIXEL_SIZE; ydx++)
    {
        pinMode(pin_map_y[ydx], OUTPUT);
        digitalWrite(pin_map_y[ydx], HIGH);
    }
}

unsigned long pre_internal_time = 0;
unsigned long external_time = 0;
void loop()
{
    unsigned long time = -1;
    if (ESP01.available())
    {
        time = atol(ESP01.readStringUntil('\n').c_str());
        Serial.println(time);
    }

    if (time >= 0)
    {
        external_time = time;
        pre_internal_time = (millis() / 1000);
    }
    matrix_set(external_time + (millis() / 1000) - pre_internal_time);
    matrix_draw();
}

int8_t matrix_status[_ROW_PIXEL_SIZE][_COL_PIXEL_SIZE] = {{0, }, };
void matrix_set(uint32_t _time) {
    int8_t time[4] = {
        _time / 36000,
        (_time / 3600) % 10,
        (_time % 3600) / 600,
        (_time % 600) / 60,
    };
    
    for (int8_t ydx = 0; ydx < 5; ydx++) {
        for (int8_t xdx = 0; xdx < 3; xdx++) {
            matrix_status[ydx][xdx] = digit_led[time[0]][ydx][xdx];
            matrix_status[ydx][xdx + 3] = digit_led[time[1]][ydx][xdx];
            matrix_status[ydx + 5][xdx] = digit_led[time[2]][ydx][xdx];
            matrix_status[ydx + 5][xdx + 3] = digit_led[time[3]][ydx][xdx];
        }
    }
}

void matrix_draw() {
    for (int8_t ydx = 0; ydx < _ROW_PIXEL_SIZE; ydx++) {
        for (int8_t xdx = 0; xdx < _COL_PIXEL_SIZE; xdx++) {
            if (matrix_status[ydx][xdx]) {
                digitalWrite(pin_map_x[xdx], HIGH);
            }
        }
        digitalWrite(pin_map_y[ydx], LOW);
        delay(1);
        for (int8_t xdx = 0; xdx < _COL_PIXEL_SIZE; xdx++) {
            digitalWrite(pin_map_x[xdx], LOW);
        }
        digitalWrite(pin_map_y[ydx], HIGH);
    }
}
