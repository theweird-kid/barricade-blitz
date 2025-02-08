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

    addEntity("assets/blue.bmp", Entity::Type::PLAYER);
    addEntity("assets/red.bmp", Entity::Type::ENEMY);
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
        m_Client->SendScore(playerScore, enemyScore);
        std::cout << "PLAYER: " << playerScore << "\tENEMY: " << enemyScore << std::endl;
        if(playerScore == 40 || enemyScore == 40) {
            resetGame();
        }
    }

    // Get Data From Server
    if(m_Client && m_Client->IsConnected() && !m_Client->Incoming().empty()) {
        auto msg = m_Client->Incoming().pop_front().msg;
        InternalMessageType mess = m_Client->OnMessage(msg);

        if(mess.id == GameMsg::Score_Update) {
            int player, enemy;
            ParseScoreUpdate(mess.message, player, enemy);
            // Reverse the scores
            playerScore = enemy;
            enemyScore = player;
        }

        // Update Enemy Position
        else if(mess.id == GameMsg::Enemy_Update) {
            float xPos, yPos;
            ParseEnemyUpdateMessage(mess.message, xPos, yPos);
            TransformEnemyCoordinates(xPos, yPos);
            m_EntityManager->setEnemyPos(xPos, yPos);
        }

        // Update Ball Position and Velocity
        else if(mess.id == GameMsg::Ball_Update) {
            float xPos, yPos, xVel, yVel;
            ParseBallUpdateMessage(mess.message, xPos, yPos, xVel, yVel);
            TransformBallCoordinates(xPos, yPos, xVel, yVel);
            m_EntityManager->setBallData(xPos, yPos, xVel, yVel);
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

void Game::ParseScoreUpdate(const std::string& message, int player, int enemy) {
    // Assuming the message format is "PLAYER: <player>\tENEMY: <enemy>"
    size_t playerStart = message.find("PLAYER: ") + 8;
    size_t playerEnd = message.find("\t", playerStart);
    player = std::stoi(message.substr(playerStart, playerEnd - playerStart));

    size_t enemyStart = message.find("ENEMY: ") + 7;
    enemy = std::stoi(message.substr(enemyStart));
}

void Game::ParseEnemyUpdateMessage(const std::string& message, float& xPos, float& yPos) {
        // Assuming the message format is "Xpos: <xPos>\tYpos: <yPos>"
        size_t xPosStart = message.find("Xpos: ") + 6;
        size_t xPosEnd = message.find("\t", xPosStart);
        xPos = std::stoi(message.substr(xPosStart, xPosEnd - xPosStart));

        size_t yPosStart = message.find("Ypos: ") + 6;
        yPos = std::stoi(message.substr(yPosStart));
    }

void Game::TransformEnemyCoordinates(float& xPos, float& yPos) {
        // Example transformation: mirror the position horizontally and vertically
        xPos = SCREEN_WIDTH - xPos;
        yPos = SCREEN_HEIGHT - yPos;
}

void Game::ParseBallUpdateMessage(const std::string& message, float& xPos, float& yPos, float& xVel, float& yVel) {
    size_t xPosStart = message.find("Xpos: ") + 6;
    size_t xPosEnd = message.find("\t", xPosStart);
    xPos = std::stof(message.substr(xPosStart, xPosEnd - xPosStart));

    size_t yPosStart = message.find("Ypos: ") + 6;
    size_t yPosEnd = message.find("\t", yPosStart);
    yPos = std::stof(message.substr(yPosStart, yPosEnd - yPosStart));

    size_t xVelStart = message.find("Xvel: ") + 6;
    size_t xVelEnd = message.find("\t", xVelStart);
    xVel = std::stof(message.substr(xVelStart, xVelEnd - xVelStart));

    size_t yVelStart = message.find("Yvel: ") + 6;
    yVel = std::stof(message.substr(yVelStart));
}

void Game::TransformBallCoordinates(float& xPos, float& yPos, float& xVel, float& yVel) {
    // Example transformation: mirror the position horizontally and vertically
    xPos = SCREEN_WIDTH - xPos;
    yPos = SCREEN_HEIGHT - yPos;
    xVel = -xVel;
    yVel = -yVel;
}
