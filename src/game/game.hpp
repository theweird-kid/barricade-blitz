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

    Game(SDL_Window* window, SDL_Renderer* renderer);
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
    bool m_toggleMusic = false;
    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;

    // Background Texture
    std::unique_ptr<Texture> m_BackgroundTexture;

    // Entitiy Manager
    std::unique_ptr<EntityManager> m_EntityManager;
    // Game Sound
    std::unique_ptr<GameSound> m_GameSound;
};

#endif // GAME_HPP
