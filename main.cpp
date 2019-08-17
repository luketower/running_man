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
    KeyboardPress_Left,
    KeyboardPress_Space
};

global game_window GameWindow;
global int UpdateCount = 0;

void CharacterRun(SDL_Rect * TextureRect, int UpdateCount, keyboard_press KeyboardPress)
{
    TextureRect->y = 37;

    if (UpdateCount < 25)
    {
        TextureRect->x = TextureRect->w * 1;
    }
    else if (UpdateCount  < 50)
    {
        TextureRect->x = TextureRect->w * 2;
    }
    else if (UpdateCount < 75)
    {
        TextureRect->x = TextureRect->w * 3;
    }
    else if (UpdateCount < 100)
    {
        TextureRect->x = TextureRect->w * 4;
    }
    else if (UpdateCount < 125)
    {
        TextureRect->x = TextureRect->w * 5;
    }
    else if (UpdateCount < 150)
    {
        TextureRect->x = TextureRect->w * 6;
    }
    else
    {
        TextureRect->x = 50;
    }
}

void StandIdle(SDL_Rect *TextureRect, int UpdateCount)
{
    if (UpdateCount < 25)
    {
        TextureRect->x = 0;
    }
    else if (UpdateCount  < 50)
    {
        TextureRect->x = TextureRect->w * 1;
    }
    else if (UpdateCount < 75)
    {
        TextureRect->x = TextureRect->w * 2;
    }
    else if (UpdateCount < 100)
    {
        TextureRect->x = TextureRect->w * 3;
    }
    else
    {
        TextureRect->x = 0;
    }
}

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

    SDL_Texture *SpriteSheet = NULL;
    SDL_Surface *TmpSurface = IMG_Load("assets/adventurer-v1.5-Sheet.png");
    SpriteSheet = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
    SDL_FreeSurface(TmpSurface);

    SDL_Rect WindowRect = {
        .h = 64,
        .w = 64,
        .x = (GameWindow.Width/2)-32,
        .y = (GameWindow.Height/2)-32
    };

    SDL_Rect TextureRect = {
        .x = 0,
        .y = 0
    };

    SDL_QueryTexture(SpriteSheet, NULL, NULL, &TextureRect.w, &TextureRect.h);

    TextureRect.w = 50; // Not sure why this number works.
    TextureRect.h = 37;

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
                    case SDLK_LEFT:
                        KeyboardPress = KeyboardPress_Left;
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
                    case SDLK_LEFT:
                        KeyboardPress = KeyboardPress_Undefined;
                        TextureRect.x = 0;
                        TextureRect.y = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        if (KeyboardPress == KeyboardPress_Right)
        {
            CharacterRun(&TextureRect, UpdateCount, KeyboardPress);
        }
        else
        {
            StandIdle(&TextureRect, UpdateCount);
        }

        SDL_assert(SpriteSheet);
        SDL_RenderClear(Renderer);
        SDL_RenderCopy(Renderer, SpriteSheet, &TextureRect, &WindowRect);
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
    SDL_DestroyTexture(SpriteSheet);
    SDL_Quit();

    return 0;
}
