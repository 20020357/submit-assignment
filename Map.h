// Thư mục này chứa các tên hàm của thư mục Map.cpp

#pragma once
#include "Minesweeper.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

void Map (Minesweeper& test, bool &replay);
void refreshScreen (SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& mouse, Minesweeper& test, int& squares, bool &aLive);
void youWin (SDL_Rect picture, SDL_Rect screen, Minesweeper test);
void youLose (SDL_Rect picture, SDL_Rect screen, Minesweeper test);
void showBomb (SDL_Rect picture, SDL_Rect screen, SDL_Renderer* renderer, SDL_Texture* texture, Minesweeper test);
void replayGame (SDL_Rect picture, SDL_Rect screen, Minesweeper test, bool &replay);
