#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H
#include <Arduino.h>
#include <LiquidCrystal.h>

extern byte rlLineSprite[8];

extern byte heartSprite[8];

extern byte lrLineSprite[8];

class LcdManager {
    LiquidCrystal lcd;
    int brightnessPin;
    int contrastPin;

  public:
    LcdManager(int RS, int enable, int d4, int d5, int d6, int d7, int contrastPin, int brightnessPin);

    void debugPrint(int x, int y = 0);

    void setBrightness(int val);
    void setContrast(int val);

    void printSplash();
    void printMenu(int firstVisibleLine, int cursorLine);
    void printStartGame(int cursorLine, int lvl);
    void printSettings(int firstVisibleLine, int cursorLine, int lcdBright, int ledBright, int lcdContrast);
    void printAbout();
    void printHighscores(int firstVisibleLine, String names[3], int scores[3]);
    void printGame(int lvl, int lives, int score);
    void printGameOver();
    void printName(int score, int scorePosition, int letter, char dotName[3]);
    void printScore(int score);
};

#endif
