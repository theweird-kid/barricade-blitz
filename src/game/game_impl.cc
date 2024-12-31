#include "game.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
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

    // Init Background Texture
    m_BackgroundTexture = std::make_unique<Texture>("assets/background.bmp", m_Renderer.get(), SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize the Entity Manager
    m_EntityManager = std::make_unique<EntityManager>(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize the Game Sound
    m_GameSound = std::make_unique<GameSound>();
    // Play the music
    if(m_GameSound->getMusicStatus()) m_GameSound->playMusic();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Handle game events here
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        else if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym)
            {
                case SDLK_m:
                    m_toggleMusic = true;
                    break;
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }
        // Pass the event to the entity manager
        m_EntityManager->handleEvent(event);
    }
}

void Game::update() {
    // Update game logic here

    // Update the music status
    if(m_toggleMusic) {
        if(m_GameSound->getMusicStatus()) m_GameSound->pauseMusic();
        else m_GameSound->playMusic();
        m_toggleMusic = false;
    }

    // handle Collision
    m_EntityManager->handleCollison(m_GameSound.get());
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
