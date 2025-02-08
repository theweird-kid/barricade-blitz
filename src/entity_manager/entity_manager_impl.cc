#include "entity_manager.hpp"
#include <SDL2/SDL_events.h>
#include <memory>

void EntityManager::addEntity(const std::string& path, SDL_Renderer* renderer, Entity::Type t)
{
    // CREATE PLAYER
    if(t == Entity::Type::PLAYER) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = m_SCREEN_HEIGHT - 70;
        int width = 100;
        int height = 20;
        std::pair<float,float> speed = {0.15, 0};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::PLAYER));
    }
    // CREATE ENEMY
    else if(t == Entity::Type::ENEMY) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = 60;
        int width = 100;
        int height = 20;
        std::pair<float,float> speed = {0.15, 0};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::ENEMY));
    }
    // CREATE BALL
    else if(t == Entity::Type::BALL) {
        int x = m_SCREEN_WIDTH / 2;
        int y = m_SCREEN_HEIGHT / 2;
        int width = 30;
        int height = 30;
        std::pair<float,float> speed = {0.08, 0.08};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::BALL));
    }
}

void EntityManager::handleEvent(SDL_Event& event, std::shared_ptr<GameClient> client)
{
    for(auto& entity : m_Entities) {
        entity->handleEvent(event, client);

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

void EntityManager::reset()
{
    for(auto& entity : m_Entities) {
        entity->reset();
    }
}

void EntityManager::render(SDL_Renderer* renderer)
{
    // Render all entities to the screen
    for(auto& entity : m_Entities) {
        entity->render(renderer);
    }
}

void EntityManager::setEnemyPos(float x, float y) {
    if(m_Entities[1]->getX() != x)m_Entities[1]->setX(x);
    if(m_Entities[1]->getY() != y)m_Entities[1]->setY(y);
}

void EntityManager::setBallData(float x_pos, float y_pos, float x_vel, float y_vel) {
    m_Entities[2]->setX(x_pos);
    m_Entities[2]->setY(y_pos);
    m_Entities[2]->setVelocity({x_vel, y_vel});
}
