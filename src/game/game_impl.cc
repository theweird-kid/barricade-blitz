#include "game.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <memory>

Game::Game(SDL_Window* window, SDL_Renderer* renderer, Sound* sound, std::shared_ptr<GameClient> client)
    : m_Window(window), m_Renderer(renderer), m_GameSound(sound), m_Client(client)
{
    init();
    m_GameMode = GameMode::OFFLINE;
}

Game::~Game() {
    clean();
    //SDL_Quit();
}

void Game::init() {

    // Init Background Texture
    m_BackgroundTexture = std::make_unique<Texture>("assets/background.bmp", m_Renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize the Entity Manager
    m_EntityManager = std::make_unique<EntityManager>(SCREEN_WIDTH, SCREEN_HEIGHT, playerScore, enemyScore);

    // Initialize the HUD
    m_Hud = std::make_unique<Hud>(m_Renderer, playerScore, enemyScore);

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
    m_EntityManager->handleEvent(event, m_Client);
}

void Game::update() {

    // handle Collision
    m_EntityManager->handleCollison(m_GameSound, updateScore);

    // Update Huds
    if(updateScore)
    {
        updateScore = false;
        m_Client->SendStatus(playerScore, enemyScore);
        std::cout << "PLAYER: " << playerScore << "\tENEMY: " << enemyScore << std::endl;
        if(playerScore == 40 || enemyScore == 40) {
            resetGame();
        }
    }
}

// Add Entity to the Entity Manager
void Game::addEntity(const std::string& path, Entity::Type t)
{
    this->m_EntityManager->addEntity(path, m_Renderer, t);
}

void Game::render(float& deltaTime) {

    // Clear the screen
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255); // Set background color to white
    SDL_RenderClear(m_Renderer);

    // Render Background
    m_BackgroundTexture->render(m_Renderer, 0, 0);

    // Draw game objects here
    m_EntityManager->render(m_Renderer);

    // Draw HUD
    m_Hud->render(deltaTime);

    // Present the rendered frame
    SDL_RenderPresent(m_Renderer);

}

void Game::resetGame()
{
    isGameRunning = false;
    playerScore = 0;
    enemyScore = 0;
    m_EntityManager->reset();
}

void Game::clean() {
    // No need to explicitly destroy gWindow and gRenderer,
    // as the unique_ptrs will handle it automatically
}
