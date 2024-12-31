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

    // Init Background Texture
    m_BackgroundTexture = std::make_unique<Texture>("assets/background.bmp", m_Renderer.get(), SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize the Entity Manager
    m_EntityManager = std::make_unique<EntityManager>(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Handle game events here
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        // Pass the event to the entity manager
        m_EntityManager->handleEvent(event);
    }
}

void Game::update() {
    // Update game logic here

    // Update the position of the ball
    for(auto& entity: m_EntityManager->m_Entities) {
        if(entity->getType() == Entity::Type::BALL)
        {
            entity->setY(entity->getY() + entity->getSpeed().second * entity->delataTime);
            entity->setX(entity->getX() + entity->getSpeed().first * entity->delataTime);

            if(entity->getX() > SCREEN_WIDTH) {
                entity->setSpeed({-entity->getSpeed().first, entity->getSpeed().second});
            }
            else if(entity->getX() < 0) {
                entity->setSpeed({-entity->getSpeed().first, entity->getSpeed().second});
            }

            if(entity->getY() > SCREEN_HEIGHT) {
                entity->setSpeed({entity->getSpeed().first, -entity->getSpeed().second});
            }
            else if(entity->getY() < 0) {
                entity->setSpeed({entity->getSpeed().first, -entity->getSpeed().second});
            }
        }
    }
}

// Add Entity to the Entity Manager
void Game::addEntity(const std::string& path, Entity::Type t)
{
    this->m_EntityManager->addEntity(path, m_Renderer.get(), t);
}

void Game::render() {

    // Clear the screen
    SDL_SetRenderDrawColor(m_Renderer.get(), 255, 255, 255, 255); // Set background color to white
    SDL_RenderClear(m_Renderer.get());

    // Render Background
    m_BackgroundTexture->render(m_Renderer.get(), 0, 0);

    // Draw game objects here
    m_EntityManager->render(m_Renderer.get());

    // Present the rendered frame
    SDL_RenderPresent(m_Renderer.get());

}


void Game::clean() {
    // No need to explicitly destroy gWindow and gRenderer,
    // as the unique_ptrs will handle it automatically
}
