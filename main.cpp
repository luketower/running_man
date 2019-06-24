#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define global static

typedef uint8_t u8;
typedef uint32_t u32;

struct game_window
{
    int Height;
    int Width;
    int Pitch;
    int BytesPerPixel;
};

enum keyboard_press
{
    KeyboardPress_Undefined,
    KeyboardPress_Up,
    KeyboardPress_Down,
    KeyboardPress_Right,
    KeyboardPress_Space
};

global game_window GameWindow;
global int UpdateCount = 0;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    bool Running = true;
    GameWindow.Height = 600;
    GameWindow.Width = 800;
    GameWindow.BytesPerPixel = 4;
    GameWindow.Pitch = GameWindow.Width * GameWindow.BytesPerPixel;

    SDL_Window *Window = SDL_CreateWindow("Running Man",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          GameWindow.Width,
                                          GameWindow.Height,
                                          SDL_WINDOW_SHOWN);
    SDL_assert(Window);

    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, 0, SDL_RENDERER_ACCELERATED);
    SDL_assert(Renderer);

    SDL_Texture *Texture;
    SDL_Rect DestRect = {
        .h = 64,
        .w = 64,
        .x = (GameWindow.Width/2)-32,
        .y = (GameWindow.Height/2)-32
    };


    u32 FrameStart;
    float ElapsedTime;
    keyboard_press KeyboardPress = KeyboardPress_Undefined;


    while(Running)
    {
        FrameStart = SDL_GetTicks();
        SDL_Event Event;

        while (SDL_PollEvent(&Event))
        {

            if(Event.type == SDL_QUIT)
            {
                Running = false;
                return 0;
            }

            SDL_Keycode KeyCode = Event.key.keysym.sym;

            if (Event.type == SDL_KEYDOWN)
            {
                switch(KeyCode)
                {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        Running = false;
                        break;
                    case SDLK_RIGHT:
                        KeyboardPress = KeyboardPress_Right;
                        break;
                    case SDLK_UP:
                        KeyboardPress = KeyboardPress_Up;
                        break;
                    case SDLK_DOWN:
                        KeyboardPress = KeyboardPress_Down;
                        break;
                    case SDLK_SPACE:
                        KeyboardPress = KeyboardPress_Space;
                    default:
                        KeyboardPress = KeyboardPress_Undefined;
                        break;
                }
            }

            if (Event.type == SDL_KEYUP)
            {
                switch(KeyCode)
                {
                    case SDLK_RIGHT:
                        KeyboardPress = KeyboardPress_Undefined;
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_Surface *TmpSurface;

        if (KeyboardPress == KeyboardPress_Right)
        {
            if (UpdateCount < 25)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-00.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount  < 50)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-01.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount < 75)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-02.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount < 100)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-03.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount < 125)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-04.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount < 150)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-05.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-run-00.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
        }
        else
        {
            if (UpdateCount < 25)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-idle-00.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount  < 50)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-idle-01.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount < 75)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-idle-02.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else if (UpdateCount < 100)
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-idle-03.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
            else
            {
                TmpSurface = IMG_Load("assets/individual_sprites/adventurer-idle-00.png");
                Texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
            }
        }

        SDL_FreeSurface(TmpSurface);
        SDL_assert(Texture);
        SDL_RenderClear(Renderer);
        SDL_RenderCopy(Renderer, Texture, NULL, &DestRect);
        SDL_RenderPresent(Renderer);

        if (UpdateCount < 149)
        {
            UpdateCount++;
        }
        else
        {
            UpdateCount = 0;
        }

        ElapsedTime = (float)(SDL_GetTicks() - FrameStart);

        if(ElapsedTime < 6)
        {
            SDL_Delay(6 - (u32)(ElapsedTime));
            ElapsedTime = (float)(SDL_GetTicks() - FrameStart);
        }
    }

    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyTexture(Texture);
    SDL_Quit();

    return 0;
}
