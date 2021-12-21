#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "lcd_manager.h"
#include "pages.h"
#include "led_game.h"

/// I give up :)

const int waitTime = 2000;

//LED
const int dinPin = 3;
const int clockPin = 12;
const int loadPin = 4;
const int rows = 8;
const int cols = 8;
LedMap lc = LedMap(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

bool refreshLed;

int ledBrightness = 2;

//LCD
const int RS = 8;
const int enable = 10;
const int d4 = 7;
const int d5 = 11;
const int d6 = 0;
const int d7 = 1;

const int n = 8;

const int contrastPin = 5;
const int brightnessPin = 6;
LcdManager lcd(RS, enable, d4, d5, d6, d7, contrastPin, brightnessPin);

bool refreshLcd = false;

int lcdContrast = 92;
int lcdBrightness = 128;

bool aboutFirstTime = true;

//BUZZER
const int buzzerPin = 9;

// joystick
const int switchPin = 2;
const int xAxisPin = A0;
const int yAxisPin = A1;

const int joystickDown = 1;
const int joystickUp = 2;
const int joystickLeft = 4;
const int joystickRight = 8;

unsigned joystickSampleInterval = 1500;
unsigned long long lastJoystickSample = 0;

const int minThreshold = 200;
const int maxThreshold = 800;

bool joystickMoved = false;

const int joystickSampleFast = 100;
const int joystickSampleMedium = 300;
const int joystickSampleSlow = 1500;

// Joystick

void processJoystickInput(int joystickDirection);
void processJoystickInputMenu(int joystickDirection);
void processJoystickInputStartGame(int joystickDirection);
void processJoystickInputSettings(int joystickDirection);
void processJoystickInputHighscores(int joystickDirection);
void processJoystickInputPlay(int joystickDirection);
void processJoystickInputOver(int joystickDirection);
void processJoystickInputName(int joystickDirection);


// GameState
const int menuState = 0;
const int startGameState = 1;
const int highscoresState = 2;
const int settingsState = 3;
const int aboutState = 4;
const int playState = 5;
const int overState = 6;
const int nameState = 7;
const int scoreState = 8;

volatile int gameState = menuState;

unsigned long long gameOverTime = 0;

// Menu
const int eepromStart = 193;

int scrollPosition = 0;
int cursorPosition = 1;
int lvl = 1;

String highscoreNames[3] = {"AAA", "AAA", "AAA"};
int highscoreValues[3] = {0, 0, 0};

char dotName[3] = {'A', 'A', 'A'};
int letterPos = 0;
int racePlace = 0;

// About
unsigned long long aboutScrollTime = 0;
unsigned aboutScrollWait = 500;

// Game
const int randomBlanks = 6;

unsigned long score = 0;

// Score
unsigned long long lastScoreUpdate = 0;
const int scoreUpdateInterval = 500;
const int scoreMultiplayer = 8;

// Button

const int interruptCycle = 200;
volatile unsigned long long lastInterrupt = 0;

void handleInterrupt();
void processButton();
void processButtonMenu();
void processButtonStartGame();
void processButtonSettings();
void processButtonAbout();
void processButtonHighscores();
void processButtonPlayState();
void processButtonNameState();
void processButtonScoreState();




void processButton() {
  tone(buzzerPin, 400, 10);
  if (gameState == menuState) {
    processButtonMenu();
    return;
  }
  if (gameState == startGameState) {
    processButtonStartGame();
    return;
  }
  if (gameState == settingsState) {
    processButtonSettings();
    return;
  }
  if (gameState == aboutState) {
    processButtonAbout();
    return;
  }
  if (gameState == highscoresState) {
    processButtonHighscores();
    return;
  }
  if (gameState == playState) {
    processButtonPlayState();
    return;
  }
  if (gameState == nameState) {
    processButtonNameState();
    return;
  }
  if (gameState == scoreState) {
    processButtonScoreState();
    return;
  }
}

void processButtonMenu() {
  if (cursorPosition == startGameState) {
    gameState = startGameState;
    cursorPosition = lcd.startDefaultCursor;
    refreshLcd = true;
    return;
  }
  else if (cursorPosition == highscoresState) {
    gameState = highscoresState;
    refreshLcd = true;
    scrollPosition = lcd.startDefaultScroll;
    return;
  }
  else if (cursorPosition == settingsState) {
    gameState = settingsState;
    scrollPosition = lcd.startDefaultScroll;
    cursorPosition = lcd.startDefaultCursor;
    refreshLcd = true;
    joystickSampleInterval = joystickSampleFast;
    return;
  }
  else if (cursorPosition == aboutState) {
    gameState = aboutState;
    aboutFirstTime = true;
    refreshLcd = true;
    return;
  }
}

void processButtonStartGame() {
  if (cursorPosition == 0) {
    gameState = menuState;
    scrollPosition = lcd.startDefaultScroll;
    cursorPosition = lcd.startDefaultCursor;
    refreshLcd = true;
    return;
  }
  if (cursorPosition == 1) {
    gameState = playState;
    joystickSampleInterval = joystickSampleFast;
    refreshLcd = true;
    refreshLed = true;
    lc.reset();
    score = 0;
    return;
  }
}

void processButtonSettings() {
  if (cursorPosition == 0) {
    gameState = menuState;
    scrollPosition = lcd.startDefaultScroll;
    cursorPosition = lcd.startDefaultCursor;
    joystickSampleInterval = joystickSampleSlow;
    refreshLcd = true;
    return;
  }
  if (cursorPosition == 4) {
    for (int i = 0; i < 3; i++) {
      EEPROM.update(eepromStart + (7 * i), 0);
      EEPROM.update(eepromStart + (7 * i) + 1, 0);
      EEPROM.update(eepromStart + (7 * i) + 2, 0);
      EEPROM.update(eepromStart + (7 * i) + 3, 0);

      EEPROM.update(eepromStart + (7 * i) + 4, 'A');
      EEPROM.update(eepromStart + (7 * i) + 5, 'A');
      EEPROM.update(eepromStart + (7 * i) + 6, 'A');

      highscoreNames[i] = "AAA";
      highscoreValues[i] = 0;
    }
    return;
  }
}

void processButtonAbout() {
  gameState = menuState;
  scrollPosition = lcd.startDefaultScroll;
  cursorPosition = lcd.startDefaultCursor;
  refreshLcd = true;
  return;
}

void processButtonHighscores() {
  gameState = menuState;
  scrollPosition = lcd.startDefaultScroll;
  cursorPosition = lcd.startDefaultCursor;
  refreshLcd = true;
  return;
}

void processButtonPlayState() {
  lc.jump();
  refreshLcd = true;
  return;
}

void processButtonNameState() {
  gameState = menuState;
  scrollPosition = lcd.startDefaultScroll;
  cursorPosition = lcd.startDefaultCursor;
  refreshLcd = true;
  joystickSampleInterval = joystickSampleSlow;

  for (int i = 2; i > racePlace; i--) {
    highscoreNames[i] = highscoreNames[i - 1];
    highscoreValues[i] = highscoreValues[i - 1];
  }
  highscoreNames[racePlace] = dotName;
  highscoreValues[racePlace] = score;

  for (int i = 0; i < 3; i++) {
    EEPROM.update(eepromStart + (7 * i), highscoreValues[i] >> 24);
    EEPROM.update(eepromStart + (7 * i) + 1, highscoreValues[i] >> 16);
    EEPROM.update(eepromStart + (7 * i) + 2, highscoreValues[i] >> 8);
    EEPROM.update(eepromStart + (7 * i) + 3, highscoreValues[i]);

    EEPROM.update(eepromStart + (7 * i) + 4, highscoreNames[i][0]);
    EEPROM.update(eepromStart + (7 * i) + 5, highscoreNames[i][1]);
    EEPROM.update(eepromStart + (7 * i) + 6, highscoreNames[i][2]);
  }
  return;
}

void processButtonScoreState() {
  gameState = menuState;
  scrollPosition = lcd.startDefaultScroll;
  cursorPosition = lcd.startDefaultCursor;
  refreshLcd = true;
  joystickSampleInterval = joystickSampleSlow;
  return;
}

void handleInterrupt() {
  unsigned long long curr = millis();
  if (curr - lastInterrupt > interruptCycle) {
    lastInterrupt = curr;
    processButton();
  }
}

void setup()
{

  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP);

  lc.fillScreen();

  attachInterrupt(digitalPinToInterrupt(switchPin), handleInterrupt, FALLING);

  for (int i = 0; i < 3; i++) {
    highscoreValues[i] = (EEPROM.read(eepromStart + (7 * i)) << 24) + (EEPROM.read(eepromStart + (7 * i) + 1) << 16) + (EEPROM.read(eepromStart + (7 * i) + 2) << 8) + EEPROM.read(eepromStart + (7 * i) + 3);
    dotName[0] = EEPROM.read(eepromStart + (7 * i) + 4);
    dotName[1] = EEPROM.read(eepromStart + (7 * i) + 5);
    dotName[2] = EEPROM.read(eepromStart + (7 * i) + 6);
    highscoreNames[i] = dotName;
  }

  pinMode(buzzerPin, OUTPUT);
  tone(buzzerPin, 200, 10);

  lcd.printSplash();
  delay(waitTime);

  lcd.printMenu(scrollPosition, cursorPosition);
  gameState = menuState;
}

void processJoystickInput(int joystickDirection) {
  if (gameState == menuState) {
    processJoystickInputMenu(joystickDirection);
    return;
  }
  else if (gameState == startGameState) {
    processJoystickInputStartGame(joystickDirection);
    return;
  }
  else if (gameState == settingsState) {
    processJoystickInputSettings(joystickDirection);
    return;
  }
  else if (gameState == highscoresState) {
    processJoystickInputHighscores(joystickDirection);
    return;
  }
  else if (gameState == playState) {
    processJoystickInputPlay(joystickDirection);
    return;
  }
  else if (gameState == overState) {
    processJoystickInputOver();
    return;
  }
  else if (gameState == nameState) {
    processJoystickInputName(joystickDirection);
    return;
  }
}

void processJoystickInputMenu(int joystickDirection) {
  if ((joystickDirection & joystickDown) == joystickDown) {
    if (cursorPosition < menuLength - 1)
      cursorPosition = cursorPosition + 1;
    if (scrollPosition < cursorPosition - 1)
      scrollPosition = cursorPosition - 1;
    refreshLcd = true;
  }
  if ((joystickDirection & joystickUp) == joystickUp) {
    if (cursorPosition > 1)
      cursorPosition = cursorPosition - 1;
    else
      scrollPosition = lcd.startDefaultScroll;
    if (scrollPosition > cursorPosition)
      scrollPosition = cursorPosition;
    refreshLcd = true;
  }
}

void processJoystickInputStartGame(int joystickDirection) {
  if ((joystickDirection & joystickDown) == joystickDown) {
    cursorPosition = lcd.startMaxCursor;
    refreshLcd = true;
  }
  if ((joystickDirection & joystickUp) == joystickUp) {
    cursorPosition = lcd.startMinCursor;
    refreshLcd = true;
  }
  if (cursorPosition == lcd.startMaxCursor && (joystickDirection & joystickLeft) == joystickLeft) {
    lvl = lvl - 1;
    refreshLcd = true;
  }
  if (cursorPosition == lcd.startMaxCursor && (joystickDirection & joystickRight) == joystickRight) {
    lvl = lvl + 1;
    refreshLcd = true;
  }
  if (lvl < minLvl)
    lvl = maxLvl;
  if (lvl > maxLvl)
    lvl = minLvl;
}

void processJoystickInputSettings(int joystickDirection) {
  if ((joystickDirection & joystickDown) == joystickDown) {
    if (cursorPosition < settingsLength - 1)
      cursorPosition = cursorPosition + 1;
    if (scrollPosition < cursorPosition - 1)
      scrollPosition = cursorPosition - 1;
    refreshLcd = true;
  }
  if ((joystickDirection & joystickUp) == joystickUp) {
    if (cursorPosition > 0)
      cursorPosition = cursorPosition - 1;
    if (scrollPosition > cursorPosition)
      scrollPosition = cursorPosition;
    refreshLcd = true;
  }
  if (cursorPosition == lcd.lcdBrightnessCursor && (joystickDirection & joystickLeft) == joystickLeft) {
    lcdBrightness = lcdBrightness - lcd.brightnessStep;
    refreshLcd = true;
  }
  if (cursorPosition == 2 && (joystickDirection & joystickLeft) == joystickLeft) {
    ledBrightness = ledBrightness - 1;
    refreshLcd = true;
  }
  if (cursorPosition == lcd.lcdContrastCursor && (joystickDirection & joystickLeft) == joystickLeft) {
    lcdContrast = lcdContrast - 1;
    refreshLcd = true;
  }
  if (cursorPosition == lcd.lcdBrightnessCursor && (joystickDirection & joystickRight) == joystickRight) {
    lcdBrightness = lcdBrightness + lcd.brightnessStep;
    refreshLcd = true;
  }
  if (cursorPosition == 2 && (joystickDirection & joystickRight) == joystickRight) {
    ledBrightness = ledBrightness + 1;
    refreshLcd = true;
  }
  if (cursorPosition == lcd.lcdContrastCursor && (joystickDirection & joystickRight) == joystickRight) {
    lcdContrast = lcdContrast + 1;
    refreshLcd = true;
  }
  if (lcdBrightness < lcd.brightnessMin)
    lcdBrightness = lcd.brightnessMax;
  if (lcdBrightness > lcd.brightnessMax)
    lcdBrightness = lcd.brightnessMin;
  if (ledBrightness < 0)
    ledBrightness = ledMax;
  if (ledBrightness > ledMax)
    ledBrightness = 0;
  if (lcdContrast < lcd.contrastMin)
    lcdContrast = lcd.contrastMax;
  if (lcdContrast > lcd.contrastMax)
    lcdContrast = lcd.contrastMin;

  lcd.setBrightness(lcdBrightness);
  lcd.setContrast(lcdContrast);
  lc.setBrightness(ledBrightness);
}

void processJoystickInputHighscores(int joystickDirection) {
  if ((joystickDirection & joystickDown) == joystickDown) {
    if (scrollPosition < lcd.highscoresMaxScroll)
      scrollPosition = scrollPosition + 1;
    refreshLcd = true;
  }
  if ((joystickDirection & joystickUp) == joystickUp) {
    if (scrollPosition > lcd.startDefaultScroll)
      scrollPosition = scrollPosition - 1;
    refreshLcd = true;
  }
}

void processJoystickInputPlay(int joystickDirection) {
  if ((joystickDirection & joystickDown) == joystickDown) {
    lc.moveDown();
    refreshLed = true;
  }
  if ((joystickDirection & joystickUp) == joystickUp) {
    lc.moveUp();
    refreshLed = true;
  }
  if ((joystickDirection & joystickLeft) == joystickLeft) {
    lc.moveLeft();
    refreshLed = true;
  }
  if ((joystickDirection & joystickRight) == joystickRight) {
    lc.moveRight();
    refreshLed = true;
  }
}

void processJoystickInputOver() {
  unsigned long long tim = millis();
  if (tim - gameOverTime < waitTime)
    return;
  if (score > highscoreValues[0]) {
    racePlace = 0;
    gameState = nameState;
  }
  else if (score > highscoreValues[1]) {
    racePlace = 1;
    gameState = nameState;
  }
  else if (score > highscoreValues[2]) {
    racePlace = 2;
    gameState = nameState;
  }
  else
    gameState = scoreState;

  dotName[0] = 'A';
  dotName[1] = 'A';
  dotName[2] = 'A';
  letterPos = 0;
  refreshLcd = true;
  joystickSampleInterval = joystickSampleMedium;
}

void processJoystickInputName(int joystickDirection) {
  if ((joystickDirection & joystickDown) == joystickDown) {
    dotName[letterPos] = dotName[letterPos] - 1;
    if (dotName[letterPos] < 'A')
      dotName[letterPos] = 'Z';
    refreshLcd = true;
  }
  if ((joystickDirection & joystickUp) == joystickUp) {
    dotName[letterPos] = dotName[letterPos] + 1;
    if (dotName[letterPos] > 'Z')
      dotName[letterPos] = 'A';
    refreshLcd = true;
  }
  if ((joystickDirection & joystickLeft) == joystickLeft) {
    letterPos = letterPos - 1;
    if (letterPos < 0)
      letterPos = lcd.letterPosMax;
    refreshLcd = true;
  }
  if ((joystickDirection & joystickRight) == joystickRight) {
    letterPos = letterPos + 1;
    if (letterPos > lcd.letterPosMax)
      letterPos = 0;
    refreshLcd = true;
  }
}

void handleScore() { // and scroll game and add obstacles
  if (gameState != playState)
    return;

  if (lc.itCollides()) {
    gameState = overState;
    gameOverTime = millis();
  }

  unsigned long long tim = millis();
  if (tim - lastScoreUpdate < scoreUpdateInterval)
    return;
  lastScoreUpdate = tim;
  score = score + scoreMultiplayer *  lvl;
  refreshLcd = true;
  lc.clockMap();
  int x[6] = {0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 6; i++) {
    int aux = random(randomBlanks * lvl + lvl) - randomBlanks + 1;
    if (aux < 0)
      aux = 0;
    x[i] = aux;
  }
  lc.addObstacleLine(x);
  refreshLed = true;
}


void handleLcd() {
  if (!refreshLcd)
    return;
  refreshLcd = false;
  if (gameState == menuState) {
    lcd.printMenu(scrollPosition, cursorPosition);
    return;
  }
  else if (gameState == startGameState) {
    lcd.printStartGame(cursorPosition, lvl);
    return;
  }
  else if (gameState == settingsState) {
    lcd.printSettings(scrollPosition, cursorPosition, lcdBrightness, ledBrightness, lcdContrast);
    return;
  }
  else if (gameState == aboutState) {
    unsigned long long tim = millis();
    refreshLcd = true;
    if (tim - aboutScrollTime < aboutScrollWait)
      return;
    aboutScrollTime = tim;
    lcd.printAbout(aboutFirstTime);
    aboutFirstTime = false;
    return;
  }
  else if (gameState == highscoresState) {
    lcd.printHighscores(scrollPosition, highscoreNames, highscoreValues);
    return;
  }
  else if (gameState == playState) {
    lcd.printGame(lvl, 1, score);
    return;
  }
  else if (gameState == overState) {
    lcd.printGameOver();
    return;
  }
  else if (gameState == nameState) {
    lcd.printName(score, racePlace + 1, letterPos, dotName);
    return;
  }

  else if (gameState == scoreState) {
    lcd.printScore(score);
    return;
  }
}

void handleLed() {
  if (!refreshLed)
    return;
  refreshLed = false;
  lc.redrawMap();
}

int getJoystickDirection() {
  unsigned long long sampleTime = millis();

  int xValue = analogRead(xAxisPin);
  int yValue = analogRead(yAxisPin);
  bool sampled = false;
  int joystickDirection = 0;

  if (xValue < minThreshold) {
    sampled = true;
    joystickDirection = joystickDirection + joystickDown;
  }

  if (xValue > maxThreshold) {
    sampled = true;
    joystickDirection = joystickDirection + joystickUp;
  }

  if (yValue < minThreshold) {
    sampled = true;
    joystickDirection = joystickDirection + joystickLeft;
  }

  if (yValue > maxThreshold) {
    sampled = true;
    joystickDirection = joystickDirection + joystickRight;
  }

  if (joystickMoved && sampled && sampleTime - lastJoystickSample < joystickSampleInterval)
    return 0;

  if (sampled) {
    joystickMoved = true;
    lastJoystickSample = sampleTime;
  }
  else {
    joystickMoved = false;
  }

  return joystickDirection;
}


void loop() {
  int joystickPos = getJoystickDirection();
  processJoystickInput(joystickPos);
  handleScore();
  handleLcd();
  handleLed();
}
