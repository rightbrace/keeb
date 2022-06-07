#include <Arduino.h>

#define LAYOUT_US_ENGLISH
#include "HID-Project.h"

#define DOWN LOW
#define UP HIGH

int cols[] = {6, 5, 4, 3, 2, 0, 1};
int rows[] = {20, 19, 18, 15, 14, 16, 10, 9, 8, 7};


KeyboardKeycode NONE = KeyboardKeycode(0);
KeyboardKeycode F1 = KEY_VOLUME_DOWN;
KeyboardKeycode F2 = KEY_VOLUME_UP;
KeyboardKeycode F3 = KeyboardKeycode(0);
KeyboardKeycode F4 = KeyboardKeycode(0);
KeyboardKeycode F5 = KeyboardKeycode(0);
KeyboardKeycode F6 = KeyboardKeycode(0);
KeyboardKeycode F7 = KeyboardKeycode(0);
KeyboardKeycode F8 = KeyboardKeycode(0);
KeyboardKeycode F9 = KeyboardKeycode(0);

KeyboardKeycode keymap[10][7] = {
  {KEY_TILDE, KEY_2, F1, F3, KEY_ESC, KEY_0, KEY_EQUAL},
  {KEY_TAB, KEY_W, KEY_4, KEY_6, F8, KEY_P, KEY_RIGHT_BRACE},
  {KEY_CAPS_LOCK, KEY_S, KEY_R, KEY_Y, F7, KEY_L, KEY_QUOTE},
  {KEY_LEFT_SHIFT, KEY_X, KEY_F, KEY_H, F6, KEY_COMMA, KEY_SLASH},
  {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_V, KEY_N, F5, KEY_LEFT_ARROW, KEY_UP_ARROW},
  {KEY_1, KEY_3, F2, F4, KEY_8, KEY_MINUS, KEY_BACKSPACE},
  {KEY_Q, KEY_E, KEY_5, KEY_7, KEY_I, KEY_LEFT_BRACE, KEY_BACKSLASH},
  {KEY_A, KEY_D, KEY_T, KEY_U, KEY_K, KEY_SEMICOLON, KEY_RETURN},
  {KEY_Z, KEY_C, KEY_G, KEY_J, KEY_O, KEY_PERIOD, KEY_LEFT_SHIFT},
  {KEY_LEFT_GUI, KEY_SPACE, KEY_B, KEY_M, KEY_9, KEY_DOWN_ARROW, KEY_RIGHT_ARROW}
};

int keystates [10][7] = {0};
int debounceCycles = 12;

const int statusLight = 21;
void setup() {
  for (int row = 0; row < 10; row++) {
    pinMode(rows[row], INPUT_PULLUP);
  }

  for (int col = 0; col < 7; col++) {
    pinMode(cols[col], OUTPUT);
  }

  pinMode(statusLight, OUTPUT);

  Keyboard.begin();

   for (int i = 0; i < 3; i++) {
      digitalWrite(statusLight, HIGH);
      delay(100);
      digitalWrite(statusLight, LOW);
      delay(100);
   }

  digitalWrite(statusLight, LOW);

}

int statusTimer = 0;

void loop() {

  if(statusTimer > 0) {
      digitalWrite(statusLight, HIGH);
      statusTimer --;
      if (statusTimer == 0) {
        digitalWrite(statusLight, LOW);
      }
  }
  

  for (int col = 0; col < 7; col ++) {

      int colPin = cols[col];
      digitalWrite(colPin, HIGH);

      for (int row = 0; row < 10; row ++) {

        int rowPin = rows[row];
        char key = keymap[row][col];
        if (key) {          
          int value = digitalRead(rowPin);

          if (value == DOWN) {
            if (keystates[row][col] == 0) {
              keystates[row][col] = debounceCycles;
            } else if (keystates[row][col] > 1) {
              if (keystates[row][col] == 2) {
                Keyboard.press(keymap[row][col]);
                statusTimer = 100;
              }
              keystates[row][col] --;
            }
          } else {
            if (keystates[row][col] == 1) {
              Keyboard.release(keymap[row][col]);
              keystates[row][col] = 0;
            }
          }
        }
    
      }

      digitalWrite(colPin, LOW);
  }
}
