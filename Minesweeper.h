// Thư mục này chứa các tên hàm của thư mục Minesweeper.cpp

#pragma once
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_mixer.h>
#define BOMB 9

using namespace std;

struct Minesweeper
{
    int width, height, mines;
	vector <vector<char>> display;
	vector <vector<int>> data;
	vector<vector<int>> button;

    void options (int &n);
	void creating_map ();
	void processing_bomb ();
	void open (int x, int y, int& squares, bool& aLive, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& picture, SDL_Rect& screen);
	void extend (int x, int y, int& squares, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& picture, SDL_Rect& screen);
	void print (SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& picture, SDL_Rect& screen);
	int countSquares (int& squares);
	void autoPlay (int &mine, int &square, SDL_Event &e, SDL_Renderer* renderer, SDL_Texture* texture);
};
