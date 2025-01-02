#ifndef MENU_HPP
#define MENU_HPP

#include "../texture.hpp"
#include "../sound/sound.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

class Menu
{
private:
    static const int SCREEN_WIDTH = 1280;
    static const int SCREEN_HEIGHT = 720;

public:
    Menu(SDL_Window* window, SDL_Renderer* renderer)
        : m_Window(window), m_Renderer(renderer)
    {
        // Init Background Texture
        m_MenuTexture = std::make_unique<Texture>("assets/main_menu.bmp", m_Renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    }

    void handleEvents(SDL_Event& event)
    {

    }

    void update()
    {

    }

    void render()
    {
        // Clear the screen
        SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255); // Set background color to white
        SDL_RenderClear(m_Renderer);

        // Render Background
        m_MenuTexture->render(m_Renderer, 0, 0);

        // Present the rendered frame
        SDL_RenderPresent(m_Renderer);
    }

private:

    // Refrence from Application context
    SDL_Window* m_Window = nullptr;             // Window
    SDL_Renderer* m_Renderer = nullptr;         // Renderer

    // Main Menu Texture
    std::unique_ptr<Texture> m_MenuTexture;
};

#endif // MENU_HPP
