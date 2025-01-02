#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "../game/game.hpp"
#include "../hud/hud.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <memory>

class Screen
{
public:
    static const int SCREEN_WIDTH = 1280;
    static const int SCREEN_HEIGHT = 720;
public:
    enum class GameMode {
        OFFLINE,
        ONLINE
    };

    // Constructor
    Screen()
        : m_Window(nullptr, SDL_DestroyWindow), m_Renderer(nullptr, SDL_DestroyRenderer), m_Game(nullptr)
    {
        init();
        m_Game = std::unique_ptr<Game>(new Game(m_Window.get(), m_Renderer.get()));
        m_Game->init();
    }

    // Destructor
    ~Screen() {}

    void playGame()
    {
        m_Game->init();
    }

    void run()
    {
        m_isRunning = true;
        m_isGameRunning = true;
        while (m_isRunning) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void init()
    {
        // Initialize Video and Audio
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Initialize SDL Mixer
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL Mixer could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Create the window
        m_Window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow("Barricade Blitz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN), SDL_DestroyWindow);
        if (m_Window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Create the renderer
        m_Renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_SOFTWARE), SDL_DestroyRenderer);

        if (m_Renderer == nullptr) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_SetRenderDrawColor(m_Renderer.get(), 255, 255, 255, 255); // Set background color to white
        SDL_RenderClear(m_Renderer.get());
        SDL_RenderPresent(m_Renderer.get());
    }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Handle In Game events
            m_Game->handleEvents(event);

            // Handle screen events here
            if (event.type == SDL_QUIT) {
                m_isRunning = false;
            }
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        m_isRunning = false;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void update()
    {
        if(m_isGameRunning) m_Game->update();
    }

    void render()
    {
        if(m_isGameRunning) m_Game->render();
    }

private:
    // SDL Window Instance
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window;
    // SDL Renderer Instance
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer;

    // Game Object
    std::unique_ptr<Game> m_Game;
    // Game Mode
    Screen::GameMode m_GameMode;
    // Hud Object
    std::unique_ptr<Hud> m_Hud;

    // Status
    bool m_isRunning = false;
    bool m_isGameRunning = false;
    bool m_toggleMusic = false;
};

#endif // SCREEN_HPP
