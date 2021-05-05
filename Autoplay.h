// Thư mục này chứa các tến hàm của file Autoplay.cpp

#pragma once
#include "Minesweeper.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

void Autoplay (Minesweeper& test, bool &replay);
void autoShowWindowWin (SDL_Rect picture, SDL_Rect screen, Minesweeper test);
void back_to_replayGame (SDL_Rect picture, SDL_Rect screen, Minesweeper test, bool &replay);

