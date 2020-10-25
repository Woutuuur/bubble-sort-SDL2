#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <utility>
#include <time.h>
#include <iomanip>
#include "program.h"
#include "bubblesort.h"

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 600

int main()
{
    srand(time(NULL));
    try
    {
        Program* program = new Program("Bubble sort visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);
        while (program->running())
        {
            program->handleEvents();
            program->render();
        }
        delete program;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}