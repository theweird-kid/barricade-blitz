#include "entity_manager.hpp"
#include <SDL2/SDL_events.h>

void EntityManager::addEntity(const std::string& path, SDL_Renderer* renderer, Entity::Type t)
{
    // CREATE PLAYER
    if(t == Entity::Type::PLAYER) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = m_SCREEN_HEIGHT - 70;
        int width = 100;
        int height = 20;
        std::pair<int,int> speed = {2, 0};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::PLAYER));
    }
    // CREATE ENEMY
    else if(t == Entity::Type::ENEMY) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = 60;
        int width = 100;
        int height = 20;
        std::pair<int,int> speed = {2, 0};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::ENEMY));
    }
    // CREATE BALL
    else if(t == Entity::Type::BALL) {
        int x = m_SCREEN_WIDTH / 2;
        int y = m_SCREEN_HEIGHT / 2;
        int width = 30;
        int height = 30;
        std::pair<int,int> speed = {1, 1};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::BALL));
    }
}

void EntityManager::handleEvent(SDL_Event& event)
{
    for(auto& entity : m_Entities) {
        entity->handleEvent(event);

        if(entity->getType() == Entity::Type::BALL) continue;

        // Check for boundary conditions of player and enemy
        if(entity->getX() > m_SCREEN_WIDTH - entity->getWidth()) {
            entity->setX(m_SCREEN_WIDTH - entity->getWidth());
        }
        else if(entity->getX() <  0) {
            entity->setX(0);
        }
    }
}

void EntityManager::handleCollison(Sound* gameSound, bool& updateScore)
{
    for(auto& entity : m_Entities) {

        // Handle Ball - Wall Collision
        if(entity->getType() == Entity::Type::BALL) {
            bool playWallCollisionSound = false;

            // Check for boundary conditions
            if(entity->getX() > m_SCREEN_WIDTH )
            {
                entity->setVelocity({-entity->getVelocity().first, entity->getVelocity().second});
            }
            else if(entity->getX() < 0)
            {
                entity->setVelocity({-entity->getVelocity().first, entity->getVelocity().second});
            }
            else if(entity->getY() > m_SCREEN_HEIGHT)
            {
                entity->setVelocity({entity->getVelocity().first, -entity->getVelocity().second});

                // Set Ball - Wall Collision Flag
                playWallCollisionSound = true;
                // Increase Enemy Score
                m_EnemyScore += 1;  updateScore = true;
            }
            else if(entity->getY() < 0)
            {
                entity->setVelocity({entity->getVelocity().first, -entity->getVelocity().second});

                // Set Ball - Wall Collission Flag
                playWallCollisionSound = true;
                // Increase Player Score
                m_PlayerScore += 1; updateScore = true;
            }

            if(playWallCollisionSound) gameSound->playBallWallCollision();
            continue;
        }

        // Handle Entity - Ball Collision
        for(auto& other : m_Entities) {
            if(entity == other) continue;

            if(entity->getX() < other->getX() + other->getWidth() &&
               entity->getX() + entity->getWidth() > other->getX() &&
               entity->getY() < other->getY() + other->getHeight() &&
               entity->getY() + entity->getHeight() > other->getY()
            )
            {
                // PLAYER and BALL collision
                if(entity->getType() == Entity::Type::PLAYER && other->getType() == Entity::Type::BALL) {
                    other->setVelocity({other->getVelocity().first, -other->getVelocity().second});
                    gameSound->playBallPlayerCollision();
                }
                // ENEMY and BALL collision
                else if(entity->getType() == Entity::Type::ENEMY && other->getType() == Entity::Type::BALL) {
                    other->setVelocity({other->getVelocity().first, -other->getVelocity().second});
                    gameSound->playBallPlayerCollision();
                }
            }
        }
    }
}

void EntityManager::render(SDL_Renderer* renderer)
{
    // Render all entities to the screen
    for(auto& entity : m_Entities) {
        entity->render(renderer);
    }
}
