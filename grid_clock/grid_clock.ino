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
        {0, 1, 1},
        {0, 0, 1},
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
    }};

void setup()
{
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
    ESP01.begin(115200);
}

void loop()
{
    matrix_set(get_time());
    matrix_draw();
}

String esp_serial_stream = "";
unsigned long internal_time = 0;
uint32_t online_time = 0;
uint32_t get_time()
{
    if (ESP01.available())
    {
        char byte_from_read = ESP01.read();
        if ((int)byte_from_read != -1)
        {
            esp_serial_stream += byte_from_read;
            if (byte_from_read == '\n')
            {
                int32_t time = atol(esp_serial_stream.c_str());
                if (time > 0)
                {
                    online_time = time;
                    internal_time = (millis() / 1000);
                }
                esp_serial_stream = "";
            }
        }
    }

    return (online_time + (millis() / 1000) - internal_time);
}

int8_t matrix_status[_ROW_PIXEL_SIZE][_COL_PIXEL_SIZE] = {
    {
        0,
    },
};
void matrix_set(uint32_t _time)
{
    int8_t time[4] = {
        _time / 36000,
        (_time / 3600) % 10,
        (_time % 3600) / 600,
        (_time % 600) / 60,
    };

    for (int8_t ydx = 0; ydx < 5; ydx++)
    {
        for (int8_t xdx = 0; xdx < 3; xdx++)
        {
            matrix_status[ydx][xdx] = digit_led[time[0]][ydx][xdx];
            matrix_status[ydx][xdx + 3] = digit_led[time[1]][ydx][xdx];
            matrix_status[ydx + 5][xdx] = digit_led[time[2]][ydx][xdx];
            matrix_status[ydx + 5][xdx + 3] = digit_led[time[3]][ydx][xdx];
        }
    }
}

void matrix_draw()
{
    for (int8_t ydx = 0; ydx < _ROW_PIXEL_SIZE; ydx++)
    {
        for (int8_t xdx = 0; xdx < _COL_PIXEL_SIZE; xdx++)
        {
            if (matrix_status[ydx][xdx])
            {
                digitalWrite(pin_map_x[xdx], HIGH);
            }
        }
        digitalWrite(pin_map_y[ydx], LOW);
        delay(1);
        for (int8_t xdx = 0; xdx < _COL_PIXEL_SIZE; xdx++)
        {
            digitalWrite(pin_map_x[xdx], LOW);
        }
        digitalWrite(pin_map_y[ydx], HIGH);
    }
}
