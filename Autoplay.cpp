#include "Autoplay.h"

void Autoplay (Minesweeper& test, bool &replay)
{
    // Khởi tạo cửa sổ game
    SDL_Window* window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, test.width*50, test.height*50, 0);
    SDL_Renderer* renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface = IMG_Load("Minesweeper.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect picture; SDL_Rect screen;
    picture.x = 320; picture.y = 0; picture.w = 32; picture.h = 32;

    // Khởi tạo và phát âm thanh đầu game
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* opening = Mix_LoadWAV("Opening.wav");
    Mix_PlayChannel(-1, opening, 0);

    // Đổ phần renderer ra màn hình
    for (int i = 0; i < test.width; i++)
        for (int j = 0; j < test.height; j++)
        {
            screen.x = i*50; screen.y = j*50; screen.h = 50; screen.w = 50;
            test.print(renderer, texture, picture, screen);
        }
    SDL_Delay(2600);
    // Khởi tạo sự kiện chuột
    SDL_Event e;
    // Tạo biến đếm bom và số ô còn lại
    int count_mine = test.mines, count_square = test.width*test.height;
    // Tạo biến win và biến đã đóng cửa sổ
    bool win = false, closed = false;
    while (win == false)
    {
        if (e.type == SDL_QUIT)
        {
            replay = false;
            closed = true;
            break;
        }
        // Nếu số ô còn lại = số bom thì win
        if (count_square == count_mine)
        {
            SDL_DestroyWindow(window);
            count_mine = test.mines;
            win = true;
            break;
        }
        test.autoPlay(count_mine, count_square, e, renderer, texture);
    }
    // Trong khi chưa đóng cửa sổ thì: Nếu win thì hiện cửa sổ win, hiện bom lần lượt và hiện nút replay
    while (closed == false)
    {
        if (win == true)
        {
            SDL_Delay(2000);
            autoShowWindowWin(picture, screen, test);

            replay = false;
            back_to_replayGame(picture, screen, test, replay);
            break;
        }
    }

}
// Hàm tạo cửa sổ khi win và hiện bom lần lượt

void autoShowWindowWin (SDL_Rect picture, SDL_Rect screen, Minesweeper test)
{
    // show full map
    SDL_Window* newWindow = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, test.width*50, test.height*50, 0);
    SDL_Renderer* newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* newSurface = IMG_Load("Minesweeper.bmp");
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(newRenderer, newSurface);

    // Khởi tạo và chạy âm thanh khi win
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* win = Mix_LoadWAV("Win game.wav");
    Mix_PlayChannel(-1, win, 0);

    for (int i = 0; i < test.width; i++)
        for (int j = 0; j < test.height; j++)
        {
            picture.x = (test.data[i][j])*32;
            if (test.data[i][j] == BOMB) picture.x = 320;
            picture.y = 0; picture.h = 32; picture.w = 32;
            screen.x = i*50; screen.y = j*50; screen.h = 50; screen.w = 50;
            test.print(newRenderer, newTexture, picture, screen);
        }
    for (int i = 0; i < test.width; i++)
        for (int j = 0; j < test.height; j++)
        {
            picture.x = (test.data[i][j])*32; picture.y = 0; picture.h = 32; picture.w = 32;
            screen.x = i*50; screen.y = j*50; screen.h = 50; screen.w = 50;
            test.print(newRenderer, newTexture, picture, screen);
            if (test.data[i][j] == BOMB) SDL_Delay(2600/test.mines);
        }
    SDL_Delay(1000);
    SDL_DestroyWindow(newWindow);
}
// Hàm khởi tạo nút replay
void back_to_replayGame (SDL_Rect picture, SDL_Rect screen, Minesweeper test, bool &replay)
{
    // Khởi tạo cửa sổ và nút replay
    SDL_Window* replay_game = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 595, 0);
    SDL_Renderer* renderer_replay = SDL_CreateRenderer(replay_game, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface_replay = IMG_Load("Replay.bmp");
    SDL_Texture* texture_replay = SDL_CreateTextureFromSurface(renderer_replay, surface_replay);
    picture.x = 0; picture.y = 0; picture.h = 137; picture.w = 270;
    screen.x = (1000-270)/2; screen.y = (595-137)/2; screen.h = 137; screen.w = 270;
    test.print(renderer_replay, texture_replay, picture, screen);

    // Khởi tạo sự kiện nhấn nút
    SDL_Event e;
    bool run = true;
    while (run == true)
    {
        SDL_Delay(10);
        if (SDL_WaitEvent(&e) == 0) continue;
        // Nếu nhấn vào nút x trên màn hình thì thoát vòng lặp
        if (e.type == SDL_QUIT) break;
        // Nếu nhấn chuột xuống thì: Nếu là chuột trái và nhấn vào nút thì mới replay ngược lại đều không replay
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
                if ((365 < e.button.x && e.button.x < 635) && (229 < e.button.y && e.button.y < 366))
                {
                    replay = true;
                    run = false;
                }
                else replay = false;
            else replay = false;
        }
    }
    // Nếu replay đúng thì hủy cửa sổ replay đi
    if (replay == true) SDL_DestroyWindow(replay_game);
}
