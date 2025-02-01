#ifndef GAME_HPP
#define GAME_HPP

#include "../entity_manager/entity_manager.hpp"
#include "../entity/entity.hpp"
#include "../hud/hud.hpp"
#include "../sound/sound.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <memory>
#include <vector>
#include <iostream>


class Game{
private:
    static const int SCREEN_WIDTH = 1280;
    static const int SCREEN_HEIGHT = 720;

public:

    // GAME MODE TYPE
    enum class GameMode {
        OFFLINE,
        ONLINE
    };

    Game(SDL_Window* window, SDL_Renderer* renderer, Sound* sound);
    ~Game();

    void init();
    void run();
    void quit();
    void render(float& deltaTime);
    void update();
    void handleEvents(SDL_Event& event);
    void clean();
    bool running() { return isGameRunning; }

    void setRunning(bool flag) { isGameRunning = flag; }
    void resetGame();

    // Entity Manager
    void addEntity(const std::string& path, Entity::Type t);

private:
    bool isGameRunning = false;
    bool updateScore = false;

    // Refrence from Application context
    SDL_Window* m_Window = nullptr;             // Window
    SDL_Renderer* m_Renderer = nullptr;         // Renderer
    Sound* m_GameSound = nullptr;           // Game Sound

    // Hud Object
    std::unique_ptr<Hud> m_Hud;

    // Background Texture
    std::unique_ptr<Texture> m_BackgroundTexture;

    // Entitiy Manager
    std::unique_ptr<EntityManager> m_EntityManager;

    // Game Mode
    GameMode m_GameMode;

    // Player - Enemy Scores
    int playerScore = 0;
    int enemyScore = 0;

};

#endif // GAME_HPP
