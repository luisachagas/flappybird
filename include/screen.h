#include <stdio.h>

#ifndef SCREEN_H
#define SCREEN_H

#define ESC "\033"
#define CLEARSCREEN "[2J"
#define HOMECURSOR "[f"
#define SHOWCURSOR "[?25h"
#define HIDECURSOR "[?25l"
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

#define MINX           1      // min screen horizontal pos
#define MINY           1      // min screen vertical pos
#define MAXX           80     // max screen horizontal pos
#define MAXY           24     // max screen vertical pos

// Cores ANSI
typedef enum {
  DEFAULT_COLOR = 39,
  BLACK = 30,
  RED = 31,
  GREEN = 30,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
  LIGHTGRAY = 37,
  DARKGRAY = 39,
} screenColor;

void screenInit();
void screenDestroy();
void screenUpdate();

// Funções auxiliares
static inline void screenHomeCursor();
static inline void screenShowCursor();
static inline void screenHideCursor();
static inline void screenClear();
static inline void screenGotoxy(int x, int y);
static inline void screenSetColor(screenColor fg, screenColor bg);


void drawGround();

static inline void screenHomeCursor() { printf("%s%s", ESC, HOMECURSOR); }

static inline void screenShowCursor() { printf("%s%s", ESC, SHOWCURSOR); }

static inline void screenHideCursor() { printf("%s%s", ESC, HIDECURSOR); }

static inline void screenClear() {
  screenHomeCursor();
  printf("%s%s", ESC, CLEARSCREEN); 
}

static inline void screenGotoxy(int x, int y) {
  x = (x < 0 ? 0 : x >= SCREEN_WIDTH ? SCREEN_WIDTH - 1 : x);
  y = (y < 0 ? 0 : y > SCREEN_HEIGHT ? SCREEN_HEIGHT : y);

  printf("%s[%d;%dH", ESC, y, x);
}

void screenSetColor(screenColor fg, screenColor bg) {
  printf("%s[38;5;%dm%s[48;5;%dm", ESC, fg, ESC, bg);
}

#endif // SCREEN_H