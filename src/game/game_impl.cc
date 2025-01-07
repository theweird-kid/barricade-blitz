#include "game.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <memory>

Game::Game(SDL_Window* window, SDL_Renderer* renderer, Sound* sound)
    : m_Window(window), m_Renderer(renderer), m_GameSound(sound)
{
    init();
    m_GameMode = GameMode::ONLINE;
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

    // Initialize the client and connect to the server
    m_Client = std::make_unique<net::Client>();
    if(!m_Client->connect("127.0.0.1", 7777)) {
        std::cerr << "Failed to connect to the server\n";
    }
    else {
        // Run the client in separate thread
        std::thread(&net::Client::run, m_Client.get()).detach();
    }

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
    m_EntityManager->handleCollison(m_GameSound, updateScore);

    // Update Huds
    if(updateScore)
    {
        //m_Hud->update();
        updateScore = false;
        std::cout << "PLAYER: " << playerScore << "\tENEMY: " << enemyScore << std::endl;
        if(playerScore == 5 || enemyScore == 5) {
            resetGame();
        }
    }

    // Send the game state to the server
    if(m_GameMode == GameMode::ONLINE)
    {
        net::MessageClient msg;
        msg.x_player_position = m_EntityManager->m_Entities[0]->getX();
        msg.y_player_position = m_EntityManager->m_Entities[0]->getY();
        msg.x_player_velocity = m_EntityManager->m_Entities[0]->getVelocity().first;
        msg.y_player_velocity = m_EntityManager->m_Entities[0]->getVelocity().second;
        msg.PlayerScore = playerScore;
        msg.EnemyScore = enemyScore;

        m_Client->sendMessage(msg);
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
