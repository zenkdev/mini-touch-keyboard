// Visual Micro is in vMicro>General>Tutorial Mode
//
/*
	Name:       mini_touch_keyboard.ino
	Created:	29.05.2019 21:39:08
	Author:     DESKTOP-PROG\dotnetjedi
*/

// Define User Types below here or use a .h file
//
#define VERSION "0.0.1"

#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>

#include "mini-touch-keyboard.h"

uint32_t loop_timer = 0; // 0.1 sec loop timer
byte loop_count = 0;     // loop counter

// Buttons
Button buttons[BUTTONS_COUNT] = {
    {
        "Format document", // title
        "test.bmp",        // image
        "shift+alt+f\n",     // keystroke
    },
    {
        "Plus", // title
        "test.bmp",        // image
        "add\n",     // keystroke
    },
    {
        "Multi command", // title
        "test.bmp",        // image
        "home:control+divide:down\n",     // keystroke
    },
    {
        "Format document", // title
        "test.bmp",        // image
        "shift+alt+f\n",     // keystroke
    },
    {
        "Format document", // title
        "test.bmp",        // image
        "shift+alt+f\n",     // keystroke
    },
    {
        "Format document", // title
        "test.bmp",        // image
        "shift+alt+f\n",     // keystroke
    },
    {
        "Format document", // title
        "test.bmp",        // image
        "shift+alt+f\n",     // keystroke
    },
    {
        "Format document", // title
        "test.bmp",        // image
        "shift+alt+f\n",     // keystroke
    }};

int16_t pressed_button = -1; // current pressed button

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// TFTLCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Define Function Prototypes that use User Types below here or use a .h file
//

// Define Functions below here or use other .ino or cpp files
//

// Button handler
void button_handler()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // Serial.print("X = ");
  // Serial.print(p.x);
  // Serial.print("\tY = ");
  // Serial.print(p.y);
  // Serial.print("\tPressure = ");
  // Serial.println(p.z);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {

    // scale from 0->1023 to tft.width

    // *** SPFD5408 change -- Begin
    // Bug in in original code
    //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    // *** SPFD5408 change -- End
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    // Serial.print("(");
    // Serial.print(p.x);
    // Serial.print(", ");
    // Serial.print(p.y);
    // Serial.println(")");

    int16_t x, y;

    for (byte i = 0; i < BUTTONS_COUNT; i++)
    {
      x = PADDING_X + (i % 2 * (BUTTON_WIDTH + PADDING_X));
      y = PADDING_Y + (i / 2 * (BUTTON_HEIGHT + PADDING_Y));

      if (p.x >= x && p.x <= x + BUTTON_WIDTH && p.y >= y && p.y <= y + BUTTON_HEIGHT)
      {
        if (pressed_button != i)
        {
          drawButtonBorder(pressed_button, WHITE);

          pressed_button = i;
          drawButtonBorder(pressed_button, BLUE);

          Serial.print(buttons[pressed_button].keystroke);
        }
        break;
      }
    }
  }
}

// Read commands from serial
void read_serial()
{
  if (Serial.available())
  {
    String str = Serial.readString();

    if (strcmp(str.c_str(), "DONE"))
    {
      if (pressed_button != -1)
      {
        drawButtonBorder(pressed_button, WHITE);
        pressed_button = -1;
      }
    }
  }
}

void every_second()
{
  // drawButtons();
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
  Serial.begin(115200);
  // Serial.println("Mini touch keyboard v" VERSION " started.");

  tft.reset();

  tft.begin(0x9341); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial

  // Border
  drawBorder();

  // Initial screen
  tft.setCursor(55, 50);
  tft.setTextSize(3);
  tft.setTextColor(RED);
  tft.println("SPFD5408");
  tft.setCursor(55, 150);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.println("Mini touch");
  tft.setCursor(35, 170);
  tft.println("keyboard v" VERSION);

  tft.setCursor(80, 250);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.println("Touch to proceed");

  // Wait touch
  waitOneTouch();

  pinMode(13, OUTPUT);

  tft.fillScreen(BLACK);

  drawButtons();
}

// Add the main program code into the continuous loop() function
void loop()
{
  if (millis() > loop_timer)
  {
    // every 0.1 second
    loop_timer = millis() + 100;
    loop_count++;

    button_handler();

    read_serial();

    // every 0.2 second
    if (!(loop_count % 2))
    {
    }

    if (loop_count == 10)
    {
      // every second
      loop_count = 0;
      every_second();
    }
  }
}

// SPFD5408 implementation
//

// Wait one touch
TSPoint waitOneTouch()
{
  // wait 1 touch to exit function
  TSPoint p;

  do
  {
    p = ts.getPoint();

    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);

  } while ((p.z < MINPRESSURE) || (p.z > MAXPRESSURE));

  return p;
}

// Draw border
void drawBorder()
{
  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
}

// Draw buttons
void drawButtons()
{
  int16_t x, y;

  for (byte i = 0; i < BUTTONS_COUNT; i++)
  {
    x = PADDING_X + (i % 2 * (BUTTON_WIDTH + PADDING_X));
    y = PADDING_Y + (i / 2 * (BUTTON_HEIGHT + PADDING_Y));
    tft.fillRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_RADUIS, TFTLCD_WHITE);

    tft.setCursor(x + PADDING_X, y + PADDING_Y);
    tft.print(buttons[i].title);
  }
}

// Draw button border
void drawButtonBorder(uint16_t idx, uint16_t color)
{
  int16_t x = PADDING_X + (idx % 2 * (BUTTON_WIDTH + PADDING_X));
  int16_t y = PADDING_Y + (idx / 2 * (BUTTON_HEIGHT + PADDING_Y));
  tft.drawRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_RADUIS, color);
  tft.drawRoundRect(x + 1, y + 1, BUTTON_WIDTH - 2, BUTTON_HEIGHT - 2, BUTTON_RADUIS, color);
  tft.drawRoundRect(x + 2, y + 2, BUTTON_WIDTH - 4, BUTTON_HEIGHT - 4, BUTTON_RADUIS, color);
}
