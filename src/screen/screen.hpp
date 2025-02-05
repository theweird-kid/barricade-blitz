#ifndef SCREEN_HPP
#define SCREEN_HPP

// Include ImGui
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

#include "../sound/sound.hpp"
#include "../game/game.hpp"
#include "../menu/menu.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <memory>
#include <chrono>
#include <thread>

class Screen
{
private:
    static const int SCREEN_WIDTH = 1280;
    static const int SCREEN_HEIGHT = 720;

public:

    // Constructor
    Screen()
        : m_Window(nullptr, SDL_DestroyWindow), m_Renderer(nullptr, SDL_DestroyRenderer), m_Game(nullptr)
    {
        // Init global Application Context
        init();

        // Initialize the Sound Object
        m_GameSound = std::make_unique<Sound>();

        // Initialize the Menu Object
        m_Menu = std::make_unique<Menu>(m_Window.get(), m_Renderer.get());

        // Connect to remote
        m_Client = std::make_shared<GameClient>();
        m_Client->Connect("127.0.0.1", 60000);

        // Init Game object
        m_Game = std::make_unique<Game>(m_Window.get(), m_Renderer.get(), m_GameSound.get(), m_Client);
        m_Game->init();

        // Play the music
        if(m_GameSound->getMusicStatus()) m_GameSound->playMusic();
    }

    // Destructor
    ~Screen()
    {
        // Join Message thread
        //m_Client->StopMessageHandling();
        //if(m_ClientThread.joinable()) { m_ClientThread.join(); }

        // Clean up
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        Mix_Quit();
        SDL_Quit();
    }

    void playGame()
    {
        m_Game->init();
    }

    void run()
    {
        m_isRunning = true;
        m_Game->setRunning(false);
        while (m_isRunning) {
            handleEvents();

            // Calculate delta Time
            auto currentTime = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - m_LastFrameTime).count();
            m_LastFrameTime = currentTime;

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

        // Init ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui_ImplSDL2_InitForSDLRenderer(m_Window.get(), m_Renderer.get());
        ImGui_ImplSDLRenderer2_Init(m_Renderer.get());

        // Set background color
        SDL_SetRenderDrawColor(m_Renderer.get(), 255, 255, 255, 255); // Set background color to white
        SDL_RenderClear(m_Renderer.get());
        SDL_RenderPresent(m_Renderer.get());

    }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Handle In Game events
            if(m_Game->running()) m_Game->handleEvents(event);

            // Handle Menu events
            else m_Menu->handleEvents(event);

            // Handle screen events here
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:                   // Quit
                        m_isRunning = false;
                        break;
                    case SDLK_g:                        // Start/Stop Game
                        toggle_runGame = true;
                        break;
                    case SDLK_r:
                        m_Game->resetGame();
                        break;
                    case SDLK_m:                        // Play/Pause Music
                        toggle_musicStatus = true;
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT) {
                m_isRunning = false;
            }
        }
    }

    void update()
    {
        // Toggle Game Status
        if(toggle_runGame) {
            m_Game->setRunning(!m_Game->running());

            // reset toggle status
            toggle_runGame = false;
        }

        // Toggle Music Status
        if(toggle_musicStatus)
        {
            if(m_GameSound->getMusicStatus())
                m_GameSound->pauseMusic();
            else
                m_GameSound->playMusic();

            // reset toggle state
            toggle_musicStatus = false;
        }

        // Update Game status
        if(m_Game->running()) m_Game->update();
        else m_Menu->update();

    }

    void render()
    {
        // Render game
        if(m_Game->running()) m_Game->render(deltaTime);
        // Render Main Menu
        else m_Menu->render();

    }

private:
    // SDL Window Instance
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window;
    // SDL Renderer Instance
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer;

    // Menu Object
    std::unique_ptr<Menu> m_Menu;
    // Game Object
    std::unique_ptr<Game> m_Game;

    // Game Sound Object
    std::unique_ptr<Sound> m_GameSound;

    // Delta Time
    float deltaTime = 0.0f;

    // Status
    bool m_isRunning = false;

    // toggle states
    bool toggle_runGame = false;
    bool toggle_musicStatus = false;

    // For FPS calculation
    std::chrono::high_resolution_clock::time_point m_LastFrameTime;

    // Net Connection
    std::shared_ptr<GameClient> m_Client;
};

#endif // SCREEN_HPP
