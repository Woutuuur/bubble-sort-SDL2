#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "program.h"
#include "bubblesort.h"

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
#define AMOUNT_OF_ELEMENTS 15


Program::Program(const char* title, int xpos, int ypos, int w, int h, bool fullscreen)
{
    // Intialize SDL components
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw std::runtime_error("Could not initialize SDL.");
    window  = SDL_CreateWindow(title, xpos, ypos, w, h, fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
    if (!window) throw std::runtime_error("Could not initialize window.");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) throw std::runtime_error("Could not initialize renderer.");
    isRunning = true;
    if (TTF_Init() != 0) throw std::runtime_error("Could not initialize TTF.");
    prevStep = new Button(renderer, 10, h - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT);
    nextStep = new Button(renderer, w - BUTTON_WIDTH - 10, h - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT);
    allSteps = new Button(renderer, w / 2 - BUTTON_WIDTH / 2, h - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT);

    nextStep->setText("Next step");
    prevStep->setText("Prev step");
    allSteps->setText("Finish");

    nextStep->setFontSize(100);
    prevStep->setFontSize(100);
    allSteps->setFontSize(100);

    for (int i = 0; i < AMOUNT_OF_ELEMENTS; i++)
        vec.push_back(rand() % 500);

    screen_w = w - 20;
    screen_h = h - BUTTON_HEIGHT - 20;
    screen_x = 10;
    screen_y = 10;

    font = TTF_OpenFont("./res/FreeSans.ttf", 100);
    arrowImage = SDL_LoadBMP("./res/arrow.bmp");
    arrowTexture = SDL_CreateTextureFromSurface(renderer, arrowImage);
    arrow2Image = SDL_LoadBMP("./res/arrow2.bmp");
    arrow2Texture = SDL_CreateTextureFromSurface(renderer, arrow2Image);
}

Program::~Program()
{
    delete nextStep;
    delete prevStep;
    delete allSteps;
    SDL_DestroyTexture(arrowTexture);
    SDL_DestroyTexture(arrow2Texture);
    SDL_FreeSurface(arrowImage);
    SDL_FreeSurface(arrow2Image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void Program::render() const
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //Render buttons
    if (current.first < vec.size())
        nextStep->render();
    if (current.first > 0 || current.second > 0 || (current.first == -2 && current.second == -2))
        prevStep->render();
    allSteps->render();
    renderVector();
    renderLegend();

    SDL_RenderPresent(renderer);
}

void Program::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                this->stop();
                break;
            case SDL_MOUSEBUTTONUP:
                if (nextStep->mouseHover() &&  current.first < vec.size())
                {
                    prevSteps.push(std::make_pair(vec, current));
                    swapped = sortStep(vec, current);
                }
                if (allSteps->mouseHover())
                {
                    prevSteps.push(std::make_pair(vec, current));
                    sortFull(vec);
                    current = std::make_pair(-2,-2);
                    swapped = std::make_pair(-1,-1);
                }
                if (prevStep->mouseHover() && prevSteps.size() > 0)
                {
                    vec = prevSteps.top().first;
                    current = prevSteps.top().second;
                    prevSteps.pop();
                    swapped = std::make_pair(-1,-1);
                }
                break;
            default:
                break;
        }
    }
}

void Program::renderLegend() const
{
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Rect textRect;

    SDL_Rect arrow2Rect = {10, 10, 50, 50};
    SDL_RenderCopy(renderer, arrow2Texture, NULL, &arrow2Rect);
    textSurface = TTF_RenderText_Solid(font, "= Compared elements", {255,255,255,255});
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect = {70, 10, 250, 50};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_Rect arrowRect = {10, 70, 50, 50};
    SDL_RenderCopy(renderer, arrowTexture, NULL, &arrowRect);
    textSurface = TTF_RenderText_Solid(font, "= Current iteration", {255,255,255,255});
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect = {70, 70, 250, 50};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    
    SDL_Rect elemRect = {10, 140, 50, 50};
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 255,255);
    SDL_RenderFillRect(renderer, &elemRect);

    textSurface = TTF_RenderText_Solid(font, "= Swapped elements", {255,255,255,255});
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect = {70, 140, 250, 50};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

}

void Program::renderVector() const
{
    int size = vec.size();
    SDL_Rect arrowRect;
    SDL_Rect elemRect;
    for (int i = 0; i < size; i++)
    {        
        elemRect.w = screen_w / size;
        elemRect.h = elemRect.w;
        elemRect.x = screen_x + screen_w / size * i;
        elemRect.y = screen_y + screen_h / 2 - elemRect.h / 2;
        
        if (i == swapped.first || i == swapped.second)
        {
            SDL_SetRenderDrawColor(renderer, 100,100,255,255);
            SDL_RenderFillRect(renderer, &elemRect);
        }
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderDrawRect(renderer, &elemRect);

        std::stringstream ss;
        ss << std::setw(3) << vec.at(i);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, ss.str().c_str(), {255,255,255,255});
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {elemRect.x + 5, elemRect.y + 5, elemRect.w - 10, elemRect.h - 10};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
    arrowRect = {screen_x + current.first * elemRect.w, screen_y + screen_h / 2 + elemRect.h - elemRect.w / 2, elemRect.w, elemRect.w};
    SDL_RenderCopy(renderer, arrowTexture, NULL, &arrowRect);

    arrowRect = {screen_x + current.second * elemRect.w, screen_y + screen_h / 2 + elemRect.h - elemRect.w / 2, elemRect.w, elemRect.w};
    SDL_RenderCopy(renderer, arrow2Texture, NULL, &arrowRect);

    arrowRect = {screen_x + (current.second - 1) * elemRect.w, screen_y + screen_h / 2 + elemRect.h - elemRect.w / 2, elemRect.w, elemRect.w};
    SDL_RenderCopy(renderer, arrow2Texture, NULL, &arrowRect);
}