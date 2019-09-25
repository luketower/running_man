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

struct character_state
{
    bool IsRunning;
    bool IsJumping;
};

global character_state CharacterState;
global game_window GameWindow;
global int UpdateCount = 0;

void CharacterJump(SDL_Rect *TextureRect)
{
    TextureRect->y = TextureRect->h * 2;
    TextureRect->x = 0;

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
    else
    {
        TextureRect->x = TextureRect->w * 1;
    }

}

void CharacterRun(SDL_Rect *TextureRect)
{
    TextureRect->y = TextureRect->h;

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

void CharacterIdle(SDL_Rect *TextureRect)
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
    CharacterState.IsRunning = false;
    CharacterState.IsJumping = false;
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
    SDL_RendererFlip TextureFlip = SDL_FLIP_NONE;
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

    // NOTE(luke): Not sure why this TextureRect.w /= 7 doesn't work.
    // Eyeballing the results landed at 50, but it'd be nice to get
    // this value programmatically.
    TextureRect.w = 50;
    TextureRect.h /= 16;

    u32 FrameStart;
    float ElapsedTime;

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
                    case SDLK_UP:
                        CharacterState.IsJumping = true;
                        break;
                    case SDLK_RIGHT:
                        CharacterState.IsRunning = true;
                        TextureFlip = SDL_FLIP_NONE;
                        break;
                    case SDLK_LEFT:
                        CharacterState.IsRunning = true;
                        TextureFlip = SDL_FLIP_HORIZONTAL;
                        break;
                    default:
                        break;
                }
            }

            if (Event.type == SDL_KEYUP)
            {
                switch(KeyCode)
                {
                    case SDLK_RIGHT:
                    case SDLK_LEFT:
                        CharacterState.IsRunning = false;
                        TextureRect.x = 0;
                        TextureRect.y = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        if (CharacterState.IsJumping)
        {
            if (UpdateCount > 100)
            {
                CharacterState.IsJumping = false;
            }
            else
            {
                CharacterJump(&TextureRect);
            }
        }
        else if (CharacterState.IsRunning)
        {
            CharacterRun(&TextureRect);
        }
        else
        {
            CharacterIdle(&TextureRect);
        }

        SDL_assert(SpriteSheet);
        SDL_RenderClear(Renderer);
        SDL_RenderCopyEx(Renderer, SpriteSheet, &TextureRect, &WindowRect, 0, NULL, TextureFlip);
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
