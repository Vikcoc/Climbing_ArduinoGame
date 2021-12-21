#include "lcd_manager.h"
#include "pages.h"

const byte lrLine = 2;
const byte rlLine = 0;
const byte heart = 1;
const byte upArrow = 3;
const byte downArrow = 4;

byte rlLineSprite[8] = {
  B00001,
  B00010,
  B00110,
  B00100,
  B00100,
  B01000,
  B10000,
};

byte heartSprite[8] = {
  B00000,
  B00000,
  B01010,
  B11011,
  B11111,
  B01110,
  B00100,
};

byte lrLineSprite[8] = {
  B10000,
  B01000,
  B01100,
  B00100,
  B00100,
  B00010,
  B00001,
};

byte upArrowSprite[8] = {
  B00100,
  B01110,
  B11111,
  B10101,
  B10101,
  B00100,
  B00100,
  B00100,
};

byte downArrowSprite[8] = {
  B00100,
  B00100,
  B00100,
  B10101,
  B10101,
  B11111,
  B01110,
  B00100,
};

const int contrastValue = 92;
const int brightness = 128;
const int displayWidth = 16;
const int displayHeight = 2;

LcdManager::LcdManager(int RS, int enable, int d4, int d5, int d6, int d7, int contrastPin, int brightnessPin)
  : lcd(RS, enable, d4, d5, d6, d7) {
  pinMode(contrastPin, OUTPUT);
  pinMode(brightnessPin, OUTPUT);
  analogWrite(contrastPin, contrastValue);
  analogWrite(brightnessPin, brightness);

  this -> brightnessPin = brightnessPin;
  this -> contrastPin = contrastPin;

  lcd.createChar(rlLine, rlLineSprite);
  lcd.createChar(heart, heartSprite);
  lcd.createChar(lrLine, lrLineSprite);
  lcd.createChar(upArrow, upArrowSprite);
  lcd.createChar(downArrow, downArrowSprite);


  lcd.begin(displayWidth, displayHeight);
}

void LcdManager::setBrightness(int val) {
  if (val > brightnessMax)
    val = brightnessMax;
  if (val < brightnessMin)
    val = brightnessMin;
  analogWrite(brightnessPin, val);
}

void LcdManager::setContrast(int val) {
  if (val > contrastMax)
    val = contrastMax;
  if (val < contrastMin)
    val = contrastMin;
  analogWrite(contrastPin, val);
}

void LcdManager::printSplash() {
  lcd.setCursor(welcomeTop.index, 0);
  lcd.print(welcomeTop.text);
  lcd.setCursor(0, 0);
  lcd.write(lrLine);
  lcd.setCursor(0, 1);
  lcd.write(rlLine);
  lcd.setCursor(displayWidth - 1, 0);
  lcd.write(rlLine);
  lcd.setCursor(displayWidth - 1, 1);
  lcd.write(lrLine);
  lcd.setCursor(welcomeBottom.index, 1);
  lcd.print(welcomeBottom.text);
}

const char focusCharacter = '-';

void LcdManager::debugPrint(int x, int y) {
  lcd.setCursor(0, 0);
  lcd.print(x);
  lcd.setCursor(0, 1);
  lcd.print(y);
}

void LcdManager::printMenu(int firstVisibleLine, int cursorLine) {
  if (firstVisibleLine < 0)
    firstVisibleLine = 0;

  if (firstVisibleLine == 0 && cursorLine <= startDefaultCursor)
    cursorLine = startDefaultCursor;

  if (firstVisibleLine > menuLength - displayHeight)
    firstVisibleLine = menuLength - displayHeight;

  if (cursorLine > menuLength)
    cursorLine = menuLength;

  if (cursorLine > firstVisibleLine + displayHeight - 1)
    cursorLine = firstVisibleLine + displayHeight - 1;

  if (cursorLine < firstVisibleLine)
    cursorLine = firstVisibleLine;

  lcd.clear();

  for (int i = 0; i < min(menuLength - firstVisibleLine, displayHeight); i++) {
    lcd.setCursor(menu[firstVisibleLine + i].index, i);
    lcd.print(menu[firstVisibleLine + i].text);
  }

  lcd.setCursor(menu[cursorLine].index - 1, cursorLine - firstVisibleLine);
  lcd.print(focusCharacter);

  if (firstVisibleLine > 0) {
    lcd.setCursor(displayWidth - 1, 0);
    lcd.write(upArrow);
  }

  if (firstVisibleLine < menuLength - displayHeight) {
    lcd.setCursor(displayWidth - 1, displayHeight);
    lcd.write(downArrow);
  }
}

void LcdManager::printStartGame(int cursorLine, int lvl) {
  lcd.clear();
  if (cursorLine < startMinCursor)
    cursorLine = startMinCursor;
  if (cursorLine > startDefaultCursor)
    cursorLine = startDefaultCursor;
  if (lvl < minLvl)
    lvl = minLvl;
  if (lvl > maxLvl)
    lvl = maxLvl;

  lcd.setCursor(startGameBack.index, 0);
  lcd.print(startGameBack.text);

  lcd.setCursor(startGameStart.index, 1);
  lcd.print(startGameStart.text);

  lcd.setCursor(startGameStart.index + 11, 1);
  lcd.print(lvl);

  if (cursorLine == 0)
    lcd.setCursor(startGameBack.index - 1, 0);
  else
    lcd.setCursor(startGameStart.index - 1, 1);
  lcd.print(focusCharacter);
}

void LcdManager::printSettings(int firstVisibleLine, int cursorLine, int lcdBright, int ledBright, int lcdContrast) {
  lcd.clear();
  if (cursorLine < 0)
    cursorLine = 0;
  if (cursorLine > settingsLength - 1)
    cursorLine = settingsLength - 1;
  if (firstVisibleLine < cursorLine - 1)
    firstVisibleLine = cursorLine - 1;
  if (firstVisibleLine > cursorLine)
    firstVisibleLine = cursorLine;

  if (firstVisibleLine == 0) {
    lcd.setCursor(settingsBack.index, 0);
    lcd.print(settingsBack.text);
    lcd.setCursor(settingsBrightness.index, 1);
    lcd.print(settingsBrightness.text);
    lcd.print(lcdBright);
    lcd.print(">");
  }
  else if (firstVisibleLine == 1) {
    lcd.setCursor(settingsBrightness.index, 0);
    lcd.print(settingsBrightness.text);
    lcd.print(lcdBright);
    lcd.print(">");
    lcd.setCursor(matrixBrightness.index, 1);
    lcd.print(matrixBrightness.text);
    lcd.print(ledBright);
    lcd.print(">");
  }
  else if (firstVisibleLine == 2) {
    lcd.setCursor(matrixBrightness.index, 0);
    lcd.print(matrixBrightness.text);
    lcd.print(ledBright);
    lcd.print(">");
    lcd.setCursor(settingsContrast.index, 1);
    lcd.print(settingsContrast.text);
    lcd.print(lcdContrast);
    lcd.print(">");
  }
  else if (firstVisibleLine == 3) {
    lcd.setCursor(settingsContrast.index, 0);
    lcd.print(settingsContrast.text);
    lcd.print(lcdContrast);
    lcd.print(">");
    lcd.setCursor(settingsReset.index, 1);
    lcd.print(settingsReset.text);
  }
  lcd.setCursor(1, cursorLine - firstVisibleLine);
  lcd.print(focusCharacter);
}

void LcdManager::printAbout(bool firstTime) {
  if (!firstTime) {
    lcd.scrollDisplayLeft();
    return;
  }
  lcd.clear();
  lcd.setCursor(aboutTop.index, 0);
  lcd.print(aboutTop.text);
  lcd.setCursor(aboutBottom.index, 1);
  lcd.print(aboutBottom.text);
}

void LcdManager::printHighscores(int firstVisibleLine, String names[3], int scores[3]) {
  lcd.clear();
  if (firstVisibleLine == 0) {
    lcd.setCursor(highscoresTop.index, 0);
    lcd.print(highscoresTop.text);
    lcd.setCursor(4, 1);
    lcd.print(names[0]);
    lcd.print("  ");
    lcd.print(scores[0]);
  }
  else if (firstVisibleLine == 1) {
    lcd.setCursor(4, 0);
    lcd.print(names[0]);
    lcd.print("  ");
    lcd.print(scores[0]);
    lcd.setCursor(4, 1);
    lcd.print(names[1]);
    lcd.print("  ");
    lcd.print(scores[1]);
  }
  else if (firstVisibleLine == 2) {
    lcd.setCursor(4, 0);
    lcd.print(names[1]);
    lcd.print("  ");
    lcd.print(scores[1]);
    lcd.setCursor(4, 1);
    lcd.print(names[2]);
    lcd.print("  ");
    lcd.print(scores[2]);
  }
}

void LcdManager::printGame(int lvl, int lives, int score) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(lvl);

  lcd.setCursor(2, 1);
  lcd.print("Score: ");
  lcd.print(score);

  lcd.setCursor(12, 0);
  for (int i = 0; i < lives; i++);
  lcd.write(heart);
}

void LcdManager::printGameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Id is what id is");
  lcd.setCursor(4, 1);
  lcd.print("You dead");
}

void LcdManager::printName(int score, int scorePosition, int letter, char dotName[3]) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YOU NO ");
  lcd.print(scorePosition);
  lcd.print("! ");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Name: ");

  for (int i = 0; i < 3; i++) {
    if (letter == i)
      lcd.print("<");
    lcd.print(dotName[i]);
    if (letter == i)
      lcd.print(">");
  }
}

void LcdManager::printScore(int score) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CONGRATULATIONS!");
  lcd.setCursor(2, 1);
  lcd.print("score: ");
  lcd.print(score);
}
