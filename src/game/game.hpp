#ifndef GAME_HPP
#define GAME_HPP

#include "../entity_manager/entity_manager.hpp"
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

    Game(SDL_Window* window, SDL_Renderer* renderer, GameSound* sound);
    ~Game();

    void init();
    void run();
    void quit();
    void render();
    void update();
    void handleEvents(SDL_Event& event);
    void clean();
    bool running() { return isRunning; }

    // Entity Manager
    void addEntity(const std::string& path, Entity::Type t);

private:
    bool isRunning = false;

    // Refrence from Application context
    SDL_Window* m_Window = nullptr;             // Window
    SDL_Renderer* m_Renderer = nullptr;         // Renderer
    GameSound* m_GameSound = nullptr;           // Game Sound

    // Background Texture
    std::unique_ptr<Texture> m_BackgroundTexture;

    // Entitiy Manager
    std::unique_ptr<EntityManager> m_EntityManager;

};

#endif // GAME_HPP
