#include "pages.h"

LineData::LineData(int index, String text) {
  this->index = index;
  this->text = text;
}

const LineData welcomeTop(3, "WELCOME TO");
const LineData welcomeBottom(4, "CLIMBING");

const LineData menuMenu(6, "MENU");
const LineData menuStartGame(3, "START GAME");
const LineData menuHighscores(3, "HIGHSCORES");
const LineData menuSettings(4, "SETTINGS");
const LineData menuAbout(3, "ABOUT GAME");

const int menuLength = 5;
const LineData menu[5] = {menuMenu, menuStartGame, menuHighscores, menuSettings, menuAbout};


const LineData startGameBack(2, "BACK");
const LineData startGameStart(2, "START LVL:< >");
const int maxLvl = 4;

const LineData settingsBack(2, "BACK");
const LineData settingsBrightness(2, "LCD SHN:<");
const LineData matrixBrightness(2, "LED SHN:<");
const LineData settingsContrast(2, "LCD POP:<");
const int settingsLength = 4;

const int lcdMax = 255;
const int ledMax = 15;

const LineData aboutTop(2, "AUTHOR AT");
const LineData aboutBottom(0, "https://github.com/Vikcoc");

const char leters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

const LineData highscoresTop(4, "TOP SCORES");
