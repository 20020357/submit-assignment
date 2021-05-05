#include "Map.h"

void Map (Minesweeper& test, bool &replay)
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

    // Khởi tạo sự kiện chuột
    SDL_Event e;
    SDL_Rect mouse;
    mouse.x = 400; mouse.y = 300;
    mouse.w = 16; mouse.h = 12;

    // Tạo biến đếm bom và số ô còn lại
    int count = test.mines, count_square = test.width*test.height;
    // Tạo biến win và biến đã đóng cửa sổ
    bool win = false, closed = false;
    while (win == false)
    {
        SDL_Delay(10);
        if (SDL_WaitEvent(&e) == 0) continue;
        // Nếu nhấn nút x đỏ trên cửa sổ thì gán biến đã đóng cửa sổ là đúng, biến chơi lại là sai, và thoát vòng lặp
        if (e.type == SDL_QUIT)
        {
            replay = false;
            closed = true;
            break;
        }
        // Nếu số ô còn lại = số bom thì win
        if (count_square == count)
        {
            SDL_DestroyWindow(window);
            count = test.mines;
            win = true;
        }
        // Mỗi lần nhấn chuột thì
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            mouse.x = e.button.x;
            mouse.y = e.button.y;
            // Nếu là chuột trái thì làm mới màn hình
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                refreshScreen(renderer, texture, mouse, test, count_square, replay);
            }
            // Nếu là chuột phải và ô chưa mở thì cắm cờ (Tránh trường hợp ô đã mở rồi lại cắm được cờ)
            else if (e.button.button ==  SDL_BUTTON_RIGHT && test.button[mouse.x/50][mouse.y/50] == 1)
                {
                    picture.x = 352; picture.y = 0; picture.h = 32; picture.w = 32;
                    screen.x = (mouse.x/50)*50; screen.y = (mouse.y/50)*50; screen.h = 50; screen.w = 50;
                    test.print(renderer, texture, picture, screen);

                    // Nếu ô cắm cờ là bom thì giảm biến đếm ô đi 1, giảm số bom đi một
                    if (test.data[mouse.x/50][mouse.y/50] == BOMB)
                    {
                        count--;
                        if (test.button[mouse.x/50][mouse.y/50] == 1) test.countSquares(count_square);
                        test.button[mouse.x/50][mouse.y/50] = 0;    //Gán = 0: xác định là ô đã được mở ngược lại = 1 là chưa mở
                    }
                }
        }
        if (replay == false) break;
    }
    /* Trong khi chưa đóng cửa sổ thì: Nếu win thì hiện cửa sổ win, hiện bom lần lượt và hiện nút replay
                                       Nếu lose thì show bom những ô là bom, hiện cửa sổ lose và hiện nút replay */
    while (closed == false)
    {
        if (win == true)
        {
            SDL_Delay(1000);
            youWin(picture, screen, test);

            replay = false;
            replayGame(picture, screen, test, replay);
            break;
        }
        else
        {
            showBomb(picture, screen, renderer, texture, test);
            SDL_DestroyWindow(window);
            youLose(picture, screen, test);

            replayGame(picture, screen, test, replay);
            break;
        }
    }

}
// Hàm làm mới màn hình
void refreshScreen (SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect& mouse, Minesweeper& test, int& squares, bool &replay)
{
    // Khởi tạo biến còn sống hay không
    bool aLive = true;

    SDL_Rect picture, screen;
    // Hiện lên màn hình ô được mở
    test.open(mouse.x/50, mouse.y/50, squares, aLive, renderer, texture, picture, screen);

    // Nếu không còn sống thì gán replay sai
    if (aLive == false) replay = false;
}
// Hàm tạo cửa sổ khi thắng và hiện bom lần lượt
void youWin (SDL_Rect picture, SDL_Rect screen, Minesweeper test)
{
    // show picture: You Win to screen
    SDL_Window* window_win = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 626, 417, 0);
    SDL_Renderer* renderer_win = SDL_CreateRenderer(window_win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface_win = IMG_Load("You Win.bmp");
    SDL_Texture* texture_win = SDL_CreateTextureFromSurface(renderer_win, surface_win);
    picture.x = 0; picture.y = 0; picture.h = 417; picture.w = 626;
    screen.x = 0; screen.y = 0; screen.h = 417; screen.w = 626;
    test.print(renderer_win, texture_win, picture, screen);
    SDL_Delay(1000);
    SDL_DestroyWindow(window_win);

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
// Hàm tạo cửa sổ khi thua
void youLose (SDL_Rect picture, SDL_Rect screen, Minesweeper test)
{
    // show picture: You Lose to screen
    SDL_Window* window_lose = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 626, 391, 0);
    SDL_Renderer* renderer_lose = SDL_CreateRenderer(window_lose, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface_lose = IMG_Load("You Lose.bmp");
    SDL_Texture* texture_lose = SDL_CreateTextureFromSurface(renderer_lose, surface_lose);
    picture.x = 0; picture.y = 0; picture.h = 391; picture.w = 626;
    screen.x = 0; screen.y = 0; screen.h = 391; screen.w = 626;
    test.print(renderer_lose, texture_lose, picture, screen);
    SDL_Delay(1000);
    SDL_DestroyWindow(window_lose);

}
// Hàm show bom khi thua
void showBomb (SDL_Rect picture, SDL_Rect screen, SDL_Renderer* renderer, SDL_Texture* texture, Minesweeper test)
{
    for (int i = 0; i < test.width; i++)
        for (int j = 0; j < test.height; j++)
            if (test.data[i][j] == BOMB)
            {
                picture.x = 288; picture.y = 0; picture.h = 32; picture.w = 32;
                screen.x = i*50; screen.y = j*50; screen.h = 50; screen.w = 50;
                test.print(renderer, texture, picture, screen);
            }
    SDL_Delay(1100);
}
// Hàm khởi tạo nút replay
void replayGame (SDL_Rect picture, SDL_Rect screen, Minesweeper test, bool &replay)
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
