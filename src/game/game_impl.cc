#include "game.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

Game::Game() : m_Window(nullptr, SDL_DestroyWindow), m_Renderer(nullptr, SDL_DestroyRenderer) {
    init();
}

Game::~Game() {
    clean();
    SDL_Quit();
}

void Game::run() {
    isRunning = true;
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_Window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow("Barricade Blitz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN), SDL_DestroyWindow);

    if (m_Window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_Renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_SOFTWARE), SDL_DestroyRenderer);

    m_ScreenSurface = std::unique_ptr<SDL_Surface>(SDL_GetWindowSurface(m_Window.get()));

    if (m_Renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(m_Renderer.get(), 255, 255, 255, 255); // Set background color to white
    SDL_RenderClear(m_Renderer.get());
    SDL_RenderPresent(m_Renderer.get());

    // temp
    m_tmpSurface = std::unique_ptr<SDL_Surface>(SDL_LoadBMP("assets/hello_world.bmp"));
    if(m_tmpSurface.get() == nullptr) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    m_Entities[0]->setX(m_Entities[0]->getX() - 5);
                    break;
                case SDLK_RIGHT:
                    m_Entities[0]->setX(m_Entities[0]->getX() + 5);
                    break;
                case SDLK_a:
                    m_Entities[1]->setX(m_Entities[1]->getX() - 5);
                    break;
                case SDLK_d:
                    m_Entities[1]->setX(m_Entities[1]->getX() + 5);
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::update() {
    // Update game logic here
}

void Game::render() {

    // Clear the screen
    SDL_SetRenderDrawColor(m_Renderer.get(), 100, 255, 255, 255); // Set background color to white
    SDL_RenderClear(m_Renderer.get());

    // Draw game objects here
    for(auto& entity : m_Entities) {
        entity->render(m_Renderer.get());
    }
    // Present the rendered frame
    SDL_RenderPresent(m_Renderer.get());


    //SDL_BlitSurface(m_tmpSurface.get(), NULL, m_ScreenSurface.get(), NULL);
    //SDL_UpdateWindowSurface(m_Window.get());
}

void Game::addEntity(const std::string& path, Entity::Type t)
{
    if(t == Entity::Type::PLAYER) {
        int x = SCREEN_WIDTH / 2 - 25;
        int y = SCREEN_HEIGHT - 50;
        int width = 50;
        int height = 20;
        int speed = 30;
        m_Entities.push_back(std::make_unique<Entity>(path, m_Renderer.get(), x, y, width, height, speed, Entity::Type::PLAYER));
    }
    else if(t == Entity::Type::ENEMY) {
        int x = SCREEN_WIDTH / 2 - 25;
        int y = 50;
        int width = 50;
        int height = 20;
        int speed = 30;
        m_Entities.push_back(std::make_unique<Entity>(path, m_Renderer.get(), x, y, width, height, speed, Entity::Type::ENEMY));
    }
    else if(t == Entity::Type::BALL) {
        m_Entities.push_back(std::make_unique<Entity>(path, m_Renderer.get(), 0, 0, 50, 50, 5, Entity::Type::BALL));
    }
}

void Game::clean() {
    // No need to explicitly destroy gWindow and gRenderer,
    // as the unique_ptrs will handle it automatically
}
