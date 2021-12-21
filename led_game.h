#ifndef LED_GAME_H
#define LED_GAME_H
#include "LedControl.h"

class LedMap {
  public:

    LedControl led;
    bool dotLeft = true;
    int dotY = 7;

    int jumpingStep = 0;

    const int emptyLine = 0b010001000100;
    int maps[8] = {
      emptyLine,
      emptyLine,
      emptyLine,
      emptyLine,
      emptyLine,
      emptyLine,
      emptyLine,
      emptyLine
    };

    byte bonus[3] = {0, 0, 0};

    const int obstacleLines[6] = {0, 2, 4, 6, 8, 10};
    const int bonusLines[3] = {3, 7, 11};


    int leftCorner = 1;
    const int width = 12;

    void scrollLeft();
    void scrollRight();
    byte getLine(int line);


  public:
    LedMap(int dinPin, int clockPin, int loadPin, bool driver);
    void fillScreen();
    void moveLeft();
    void moveRight();
    void jump();
    void moveUp();
    void moveDown();
    void clockMap();
    void redrawMap();

    void addObstacleLine(int dropObstacles[6]);
    //    void addBonusLine(int dropBonus[3]);
    bool itCollides();
    //    bool itCollects();

    void reset();
    void setBrightness(int brightness);

//    static const int letterPosMax = 2;
};


#endif
