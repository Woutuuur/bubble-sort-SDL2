#ifndef PROGRAM_H
#define PROGRAM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <stack>
#include "button.h"

class Program
{
    public:
        Program();
        Program(const char* title, int xpos, int ypos, int w, int h, bool fullscreen);
        ~Program();

        void handleEvents();
        void render() const;
        
        bool running() const {return isRunning;}
        void stop() {isRunning = false;}

        void renderVector() const;
        void renderLegend() const;
    private:
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;

        Button* nextStep;
        Button* prevStep;
        Button* allSteps;

        std::vector<int> vec;
        std::stack<std::pair<std::vector<int>, std::pair<int, int>>> prevSteps;
        std::pair<int, int> current = std::make_pair(0,0);
        std::pair<int, int> swapped = std::make_pair(-1, -1);

        int screen_w, screen_h, screen_x, screen_y;
        TTF_Font* font;

        SDL_Surface* arrowImage;
        SDL_Texture* arrowTexture;

        SDL_Surface* arrow2Image;
        SDL_Texture* arrow2Texture;
};

#endif