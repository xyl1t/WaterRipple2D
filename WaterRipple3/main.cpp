//
//  main.cpp
//  WaterRipple
//
//  Created by Marat Isaw on 27.11.19.
//  Copyright © 2019 Marat Isaw. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "gfx.hpp"
#include "common.hpp"

int main(int argc, const char * argv[])
{
    srand((uint32_t)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    gfx_init(WIDTH, HEIGHT);
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Event event;
    
    window = SDL_CreateWindow("Water Ripple", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    surface = SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_ABGR32);
    
    int curx = 0;
    int cury = 0;

    
    float targetHeight = HEIGHT / 2;
    
    const int DIVIDER = 5;
    const int WATER_POINTS = WIDTH / DIVIDER + 1;
    float currentHeight[WATER_POINTS] = { };
    float vel[WATER_POINTS] = { };
    for (int i = 0; i < WATER_POINTS; i++)
    {
        currentHeight[i] = HEIGHT / 2;
        vel[i] = 0;
    }
    currentHeight[WATER_POINTS / 2] += 128;
    
    float lDeltas[WATER_POINTS] { };
    float rDeltas[WATER_POINTS] { };
    
    const float k = 0.025f; // k - spring constant
    const float d = 0.025f; // dampening
    const float s = 0.025f; // speard
    
    bool alive = true;
    bool testing = false;
    
    while(alive)
    {
        const Uint64 start = SDL_GetPerformanceCounter();
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                alive = false;
            }
            
            if(event.key.keysym.sym == SDLK_t)
            {
                testing = !testing;
            }
        }
        
        if(SDL_GetMouseState(&curx, &cury) & SDL_BUTTON_LEFT)
        {
            int index = curx / (DIVIDER);
            currentHeight[index] = cury;
        }
        else if(SDL_GetMouseState(&curx, &cury) & SDL_BUTTON_RIGHT)
        {
            
        }
        
        for (int i = 0; i < WATER_POINTS; i++)
        {
            float x = currentHeight[i] - targetHeight;
            float acc = -k * x - d * vel[i];
            
            vel[i] += acc;
            currentHeight[i] += vel[i];
        }
        
        for (int j = 0; j < 8; j++)
        {
            for (int i = 0; i < WATER_POINTS; i++)
            {
                if (i > 0)
                {
                    lDeltas[i] = s * (currentHeight[i] - currentHeight[i - 1]);
                    vel[i - 1] += lDeltas[i];
                }
                if (i < WATER_POINTS - 1)
                {
                    rDeltas[i] = s * (currentHeight[i] - currentHeight[i + 1]);
                    vel[i + 1] += rDeltas[i];
                }
            }

            for (int i = 0; i < WATER_POINTS; i++)
            {
                if (i > 0)
                    currentHeight[i - 1] += lDeltas[i];
                if (i < WATER_POINTS - 1)
                    currentHeight[i + 1] += rDeltas[i];
            }
        }
        
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
        SDL_RenderClear(renderer);
        
        uint32_t* pixels = (uint32_t*)surface->pixels;
        
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                int index = x + y * WIDTH;
                pixels[index] = 0xAAAAffff;
            }
        }
        
        for (int i = 0; i < WATER_POINTS - 1; i++)
        {
//            int point = ((int)currentHeight[i] * WIDTH) + (i);
//            if(point >= 0 && point < WIDTH * HEIGHT)
//            {
//                pixels[point] = 0xff0000ff;
//            }

            gfx_drawTriangle(pixels,
                     0x4488ffff,
                     0x4488ffff,
                     0x112244ff,
                     i * DIVIDER, currentHeight[i],
                     (i + 1) * DIVIDER, currentHeight[i + 1],
                     (i + 1) * DIVIDER, HEIGHT );

            gfx_drawTriangle(pixels,
                      0x4488ffff,
                      0x112244ff,
                      0x112244ff,
                      i * DIVIDER, currentHeight[i],
                      (i + 1) * DIVIDER, HEIGHT,
                      (i) * DIVIDER, HEIGHT );

//            drawLine(pixels, 0x2244ffff, i * DIVIDER, currentHeight[i], (i + 1) * DIVIDER, currentHeight[i + 1]);


//            if(testing)
//            {
//                for (int x = -3; x < 3; x++) {
//                    for (int y = -3; y < 3; y++) {
//                        int point = (i * DIVIDER + x + (int)(currentHeight[i] + y) * WIDTH);
//                        if(i * DIVIDER + x > 0 && i * DIVIDER + x < WIDTH - 1&&
//                           currentHeight[i] + y > 0 && currentHeight[i] + y < HEIGHT - 1 &&
//                           (x) % 2 == 0 && y % 2 == 0)
//                            pixels[point] = 0xff0000ff;
//                    }
//                }
//            }
        }
//        drawLine(pixels, 0x2244ffff, (WATER_POINTS - 1) * DIVIDER, currentHeight[(WATER_POINTS - 1)], ((WATER_POINTS - 1) + 1) * DIVIDER, currentHeight[0]);
//
//
//        drawTriangle(pixels,
//                    0x44,0x88,0xff,
//                    0x44,0x88,0xff,
//                    0x22,0x44,0x88,
//                     (WATER_POINTS - 1) * DIVIDER, currentHeight[(WATER_POINTS - 1)],
//                     ((WATER_POINTS - 1) + 1) * DIVIDER, currentHeight[0],
//                     ((WATER_POINTS - 1) + 1) * DIVIDER, HEIGHT);
//
//         drawTriangle(pixels,
//                     0x44,0x88,0xff,
//                     0x22,0x44,0x88,
//                     0x22,0x44,0x88,
//                      (WATER_POINTS - 1) * DIVIDER, currentHeight[(WATER_POINTS - 1)],
//                      ((WATER_POINTS - 1) + 1) * DIVIDER, HEIGHT,
//                      ((WATER_POINTS - 1)) * DIVIDER, HEIGHT );
//
        
        
//        int multi = 40;
        
//        drawTriangle(pixels,
//                     0xff00ffff,
//                     0 * multi, 0 * multi,
//                     2 * multi, 2 * multi,
//                     0 * multi, 4 * multi);
//        drawTriangle(pixels,
//                     0, 255, 0,
//                     255, 0, 0,
//                     0, 0, 255,
//                     curx, cury,
//                     4 * multi, 3 * multi,
//                     8 * multi, 4 * multi);
        
        
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_SetRenderTarget(renderer, texture);
        

        
        
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        
//        SDL_Delay(100);
        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
        std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
    }
    
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
