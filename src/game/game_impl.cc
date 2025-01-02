#include "game.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

Game::Game(SDL_Window* window, SDL_Renderer* renderer, GameSound* sound)
    : m_Window(window), m_Renderer(renderer), m_GameSound(sound)
{
    init();
}

Game::~Game() {
    clean();
    SDL_Quit();
}

/*
void Game::run() {
    isRunning = true;
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}
*/

void Game::init() {

    // Init Background Texture
    m_BackgroundTexture = std::make_unique<Texture>("assets/background.bmp", m_Renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize the Entity Manager
    m_EntityManager = std::make_unique<EntityManager>(SCREEN_WIDTH, SCREEN_HEIGHT);

    addEntity("assets/sample.bmp", Entity::Type::PLAYER);
    addEntity("assets/sample.bmp", Entity::Type::ENEMY);
    addEntity("assets/ball.bmp", Entity::Type::BALL);

}

void Game::handleEvents(SDL_Event& event) {
    // Handling In game events
    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym)
        {
            default:
                break;
        }
    }
    // Pass the event to the entity manager
    m_EntityManager->handleEvent(event);
}

void Game::update() {
    // Update game logic here

    // handle Collision
    m_EntityManager->handleCollison(m_GameSound);
}

// Add Entity to the Entity Manager
void Game::addEntity(const std::string& path, Entity::Type t)
{
    this->m_EntityManager->addEntity(path, m_Renderer, t);
}

void Game::render() {

    // Clear the screen
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255); // Set background color to white
    SDL_RenderClear(m_Renderer);

    // Render Background
    m_BackgroundTexture->render(m_Renderer, 0, 0);

    // Draw game objects here
    m_EntityManager->render(m_Renderer);

    // Present the rendered frame
    SDL_RenderPresent(m_Renderer);

}


void Game::clean() {
    // No need to explicitly destroy gWindow and gRenderer,
    // as the unique_ptrs will handle it automatically
}
