#ifndef HUD_HPP
#define HUD_HPP

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class Hud
{
public:
    // Constructor
    Hud(SDL_Renderer* renderer, int& player, int& enemy)
    : m_Renderer(renderer),
    playerScore(player), enemyScore(enemy)
    {


    }

    void update()
    {

    }

    void render(float& deltaTime)
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Create an ImGui window
        ImGui::Begin("Score");

        /*
        // Calculate and display FPS
        float fps = 1.0f / deltaTime;
        ImGui::Text("FPS: %.2f", fps);
        */
        // Display scores using ImGui::Text()
        ImGui::Text("Player Score: %d", playerScore);
        ImGui::Text("Enemy Score: %d", enemyScore);

        ImGui::End();


        // Render ImGui
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
    }

private:

private:
    // Refrence to Renderer
    SDL_Renderer* m_Renderer = nullptr;

    // References to player - enemy scores
    int& playerScore;
    int& enemyScore;

    // Score Textures

};

#endif // HUD_HPP
