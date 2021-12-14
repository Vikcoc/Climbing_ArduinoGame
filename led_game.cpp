#include "led_game.h"

LedMap::LedMap(int dinPin, int clockPin, int loadPin, bool driver): led(dinPin, clockPin, loadPin, driver)
{
  led.shutdown(0, false); // turn off power saving, enables display
  led.setIntensity(0, 2); // sets brightness (0~15 possible values)
  led.clearDisplay(0);// clear screen
}

void LedMap::scrollLeft() {
  if (leftCorner < 1)
    leftCorner = width - 1;
  else
    leftCorner = leftCorner - 1;
}

void LedMap::scrollRight() {
  if (leftCorner > width - 2)
    leftCorner = 0;
  else
    leftCorner = leftCorner + 1;
}

byte LedMap::getLine(int line) {
  byte x = 0;
  if (width - leftCorner >= 8) {
    x += (maps[line] >> (width - leftCorner - 8));
  }
  else {
    x += maps[line];
    x = x << 8 - width + leftCorner;
    x += maps[line] >> width - (8 - width + leftCorner);
  }

  return x;
}

void LedMap::fillScreen() {
  led.setRow(0, 0, -1);
  led.setRow(0, 1, -1);
  led.setRow(0, 2, -1);
  led.setRow(0, 3, -1);
  led.setRow(0, 4, -1);
  led.setRow(0, 5, -1);
  led.setRow(0, 6, -1);
  led.setRow(0, 7, -1);
}

void LedMap::setBrightness(int brightness) {
  led.setIntensity(0, brightness);
}

void LedMap::moveLeft() {
  if (jumpingStep != 0 || dotLeft)
    return;
  dotLeft = true;
  scrollLeft();
}

void LedMap::moveRight() {
  if (jumpingStep != 0 || !dotLeft)
    return;
  dotLeft = false;
  scrollRight();
}

void LedMap::moveUp() {
  if (dotY > 0)
    dotY -= 1;
}

void LedMap::moveDown() {
  if (dotY < 7)
    dotY += 1;
}

void LedMap::jump() {
  if (jumpingStep == 0)
    jumpingStep = 1;
}

void LedMap::clockMap() {
  if (jumpingStep > 0 && dotLeft) {
    jumpingStep++;
    scrollLeft();
    if (jumpingStep > 3) {
      jumpingStep = 0;
      dotLeft = false;
    }
  }
  else if (jumpingStep > 0 && !dotLeft) {
    jumpingStep++;
    scrollRight();
    if (jumpingStep > 3) {
      jumpingStep = 0;
      dotLeft = true;
    }
  }

  for (int i = 7; i > 0; i--)
    maps[i] = maps[i - 1];
  maps[0] = emptyLine;

  for (int i = 0; i < 3; i++)
    bonus[i] = bonus[i] >> 1;
}

void LedMap::redrawMap() {
  for (int i = 0; i < 8; i++)
    led.setRow(0, i, getLine(i));

  led.setLed(0, dotY, dotLeft ? 3 : 4, HIGH);
}

void LedMap::reset() {
  for (int i = 0; i < 8; i++)
    maps[i] = emptyLine;

  for (int i = 0; i < 3; i++)
    bonus[i] = 0;

  dotY = 7;
  dotLeft = true;
  leftCorner = 1;
}

void LedMap::addObstacleLine(int dropObstacles[6]) {
  for (int i = 0; i < 6; i++)
  {
    if (dropObstacles[i] == 1)
      maps[0] = maps[0] | (1 << (width - obstacleLines[i] + 1));
    else if (dropObstacles[i] == 2) {
      if (i & 1)
        maps[0] = maps[0] & ( -1 ^ (1 << (width - obstacleLines[i])));
      else
        maps[0] = maps[0] & ( -1 ^ (1 << (width - obstacleLines[i]) - 2));
    }
    else if (dropObstacles[i] == 3) {
      if (i & 1)
        maps[0] = maps[0] | (3 << (width - obstacleLines[i] + 1));
      else
        maps[0] = maps[0] | (3 << (width - obstacleLines[i]));
    }
    else if (dropObstacles[i] == 4) {
      if (i & 1)
        maps[0] = maps[0] | (3 << (width - obstacleLines[i] + 1));
      else
        maps[0] = maps[0] | (3 << (width - obstacleLines[i]));

      maps[1] = maps[1] | (1 << (width - obstacleLines[i] + 1));
    }
  }
}

//void LedMap::addBonusLine(int dropBonus[3]) {
//  for (int i = 0; i < 3; i++) {
//      bonus[i] = dropBonus[i] << 8;
//      maps[0] = maps[0] | (dropBonus[i] << (width - bonusLines[i] - 1));
//  }
//}

bool LedMap::itCollides() {
  byte line = getLine(dotY);
  int x = line & (1 <<  (dotLeft ? 4 : 3));
  if (x > 0)
    return true;

  x = !(line >> (dotLeft ? 3 : 4) & 1);

  if (x > 0 && jumpingStep == 0)
    return 1;

  return 0;
}
