#ifndef HUD_HPP
#define HUD_HPP

#include "../texture.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <memory>

class Hud
{
public:
    // Constructor
    Hud(SDL_Renderer* renderer, int& player, int& enemy)
    : m_Renderer(renderer),
    playerScore(player), enemyScore(enemy)
    {
        // Initialize TTF
        if(TTF_Init() == -1)
        {
            std::cerr << "[ERROR]: TTF_Init() Failed! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }

        // Create Texture
        createTexture();
    }

    void update()
    {
        // Create text surfaces for player and enemy scores
        std::string newPlayerScoreStr = "Player: " + std::to_string(playerScore);
        std::string newEnemyScoreStr = "Enemy: " + std::to_string(enemyScore);

        playerScoreTexture->updateText(newPlayerScoreStr);
        enemyScoreTexture->updateText(newEnemyScoreStr);
    }

    void render(SDL_Renderer* renderer)
    {
        playerScoreTexture->render(renderer, 10, 0);
        enemyScoreTexture->render(renderer, 1150, 0);
    }

private:
    void createTexture()
    {
        // Create text surfaces for player and enemy scores
        std::string playerScoreStr = "Player: " + std::to_string(playerScore);
        std::string enemyScoreStr = "Enemy: " + std::to_string(enemyScore);

        playerScoreTexture = std::make_unique<Texture>(playerScoreStr, m_Renderer);
        enemyScoreTexture = std::make_unique<Texture>(enemyScoreStr, m_Renderer);
    }

private:
    // References to player - enemy scores
    int& playerScore;
    int& enemyScore;

    // Refrence to Renderer
    SDL_Renderer* m_Renderer = nullptr;

    // Score Textures
    std::unique_ptr<Texture> playerScoreTexture;
    std::unique_ptr<Texture> enemyScoreTexture;

};

#endif // HUD_HPP
