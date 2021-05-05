#include "Minesweeper.h"

// Hàm khởi tạo lựa chọn chế độ khi chơi game
void Minesweeper:: options(int &n)
{
    cout << "Write your option which you want to play: (Width x Height x Mines)" << endl
         << "1. Easy: 9 x 9 x 10" << endl
         << "2. Normal: 16 x 16 x 40" << endl
         << "3. Hard: 30 x 16 x 99" << endl
         << "4. Autoplay" << endl;
    cin >> n;
    switch(n)
    {
        case 1: width = height = 9, mines = 10;
                break;
        case 2: width = height = 16, mines = 40;
                break;
        case 3: width = 30, height = 16, mines = 99;
                break;
        case 4: width = height = 9, mines = 10;
                break;
        default: cout << "Invalid. Writing again, please !" << endl;
                break;
    }
}
/* Hàm khởi tạo map: 1 map số, 1 map kí tự, 1 map nhấn chuột
   Mặc định map số toàn 0, map kí tự toàn * và map nhấn chuột toàn 1 */
void Minesweeper:: creating_map ()
{
	display.resize(width);
	data.resize(width);
	button.resize(width);
	for (int i = 0; i < width; i++)
	{
		display[i].resize(height);
		data[i].resize(height);
		button[i].resize(height);
		for (int j = 0; j < height; j++)
		{
			display[i][j] = '*';
			data[i][j] = 0;
			button[i][j] = 1;
		}
	}
}
// Hàm khởi tạo bom
void Minesweeper:: processing_bomb ()
{
	// Creating Bomb !
	srand(time(NULL));
	int x = 0, y = 0;
	for (int i = 0; i < mines; i++)
	{
		do
		{
			x = rand() % width;
			y = rand() % height;
		}
		while (data[x][y] == BOMB);
		data[x][y] = BOMB;
	}

	// Counting bomb around area have coordinate (i, j) !
	// Cập nhật map số
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if (data[i][j] != BOMB)
				for (int a = i-1; a <= i+1; a++)
					if (0 <= a && a < width)
						for (int b = j-1; b <= j+1; b++)
							if (0 <= b && b < height && data[a][b] == BOMB) data[i][j]++;
}
// Hàm mở ô được chọn
void Minesweeper:: open (int x, int y, int& squares, bool& aLive, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& picture, SDL_Rect& screen)
{
	if (data[x][y] == BOMB)
	{
		// Cập nhật map kí tự
		display[x][y] = 'X';

		//Khởi tạo và chạy âm thanh nổ bom
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Chunk* click_bomb = Mix_LoadWAV("Bomb.wav");
		Mix_PlayChannel(-1, click_bomb, 0);

		// Hiện bom ở ô vừa nhấn ra màn hình
		picture.x = data[x][y]*32; picture.y = 0; picture.h = 32; picture.w = 32;
		screen.x = x*50; screen.y = y*50; screen.h = 50; screen.w = 50;
		print(renderer, texture, picture, screen);

		// Gán biến còn sống là sai
		aLive = false;
	}
	else extend (x, y, squares, renderer, texture, picture, screen);
}
// Hàm mở rộng nếu là số khác 0 thì cập nhật map kí tự và map nhấn chuột ngược lại thì loang cho đến khi gặp số
void Minesweeper:: extend (int x, int y, int& squares, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& picture, SDL_Rect& screen)
{
	if (data[x][y] != 0)
	{
		// Cập nhật map kí tự và map nhấn chuột
		display[x][y] = data[x][y] + 48;
		if (button[x][y] == 1) countSquares(squares); // Giảm biến đếm ô đi 1
		button[x][y] = 0;

		// Khởi tạo và chạy âm thanh khi nhấn phải ô là số khác 0
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Chunk* click_on_number = Mix_LoadWAV("Click on number.wav");
		Mix_PlayChannel(-1, click_on_number, 0);

		// Hiện số ở ô vừa nhấn ra màn hình
		picture.x = data[x][y]*32; picture.y = 0; picture.h = 32; picture.w = 32;
		screen.x = x*50; screen.y = y*50; screen.h = 50; screen.w = 50;
		print(renderer, texture, picture, screen);
	}
	else
	{
		// Cập nhật map kí tự và map nhấn chuột
		display[x][y] = ' ';
		if (button[x][y] == 1) countSquares(squares);
		button[x][y] = 0;

		// Khởi tạo và chạy âm thanh khi nhấn phải ô là số 0
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Chunk* click_on_zero = Mix_LoadWAV("Click on zero.wav");
		Mix_PlayChannel(-1, click_on_zero, 0);

		// Hiện ô vừa được mở ra màn hình
		picture.x = data[x][y]*32; picture.y = 0; picture.h = 32; picture.w = 32;
		screen.x = x*50; screen.y = y*50; screen.h = 50; screen.w = 50;
		print(renderer, texture, picture, screen);

		// Đệ quy mở những ô chưa được mở cho đến khi gặp ô là số
		for (int i = -1; i < 2; i++)
			if (0 <= x+i && x+i < width)
				for (int j = -1; j < 2; j++)
					if (0 <= j+y && j+y < height)
						if (display[x+i][y+j] == '*') extend(x+i, y+j, squares, renderer, texture, picture, screen);

	}
}
// Hàm in ra màn hình
void Minesweeper:: print (SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& picture, SDL_Rect& screen)
{
	SDL_RenderCopy(renderer, texture, &picture, &screen);
    SDL_RenderPresent(renderer);
}
// Hàm đếm số ô còn lại
int Minesweeper:: countSquares (int& squares)
{
    squares--;
    return squares;
}

// Hàm tự chơi
void Minesweeper:: autoPlay (int &mine, int &square, SDL_Event &e, SDL_Renderer* renderer, SDL_Texture* texture)
{
    bool aLive = true;            // Biến tạo ra chỉ để khớp với hàm open ngoài ra không còn mục đích nào khác
    SDL_Rect picture, screen;

    while (true)
    {
        SDL_Delay(500);
        if (SDL_WaitEvent(&e) == 0) continue;
        if (e.type == SDL_QUIT || mine == square) break;

        //Lấy toạ độ ngẫu nhiên và mở nó
        int x = rand() % width, y = rand() % height;
        if (button[x][y] != 1) continue;
        if (data[x][y] == BOMB)
        {
            /* Điều kiện bên dưới giúp tăng độ khách quan cho game, nếu không có game vẫn chạy được bình thường nhưng sẽ mất khách quan
               Ví dụ: chưa mở những ô xung quanh nhưng vẫn biết đấy là bom và cắm cờ :)) */
            if (square > (width*height)/2) continue;
            else
            {
                picture.x = 352; picture.y = 0; picture.h = 32; picture.w = 32;
                screen.x = x*50; screen.y = y*50; screen.h = 50; screen.w = 50;
                print(renderer, texture, picture, screen);

                mine--;
                countSquares(square);
                button[x][y] = 0;
            }
        }
        else open(x, y, square, aLive, renderer, texture, picture, screen);
    }
}
