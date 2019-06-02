#pragma once

// TFTLCD settings
//
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Calibrate values
#define TS_MINX 95
#define TS_MINY 116
#define TS_MAXX 955
#define TS_MAXY 961

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Buttons settings and types
//

#define PADDING_X     10   // X padding
#define PADDING_Y     10   // Y padding

#define BUTTON_WIDTH  105  // Button width
#define BUTTON_HEIGHT 65   // Button height
#define BUTTON_RADUIS 5    // Button radius

#define BUTTONS_COUNT 8    // Number of buttons

typedef struct {
	char title[33];
	char image[33];
	char macro[33];
} Button;
