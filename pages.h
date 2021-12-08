#ifndef PAGES_H
#define PAGES_H
#include <Arduino.h>

struct LineData{
  int index;
  String text;
  LineData(int characterBegin, String text);
};

extern const LineData welcomeTop;
extern const LineData welcomeBottom;

extern const LineData menuMenu;
extern const LineData menuStartGame;
extern const LineData menuHighscores;
extern const LineData menuSettings;
extern const LineData menuAbout;


extern const int menuLength;
extern const LineData menu[5];


extern const LineData startGameBack;
extern const LineData startGameStart;
extern const int maxLvl;

extern const LineData settingsBack;
extern const LineData settingsBrightness;
extern const LineData matrixBrightness;
extern const LineData settingsContrast;

extern const int settingsLength;
extern const int lcdMax;
extern const int ledMax;

extern const LineData aboutTop;
extern const LineData aboutBottom;

extern const char letters[26];

extern const LineData highscoresTop;

#endif
