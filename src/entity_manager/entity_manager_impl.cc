#include "entity_manager.hpp"
#include <SDL2/SDL_events.h>

void EntityManager::addEntity(const std::string& path, SDL_Renderer* renderer, Entity::Type t)
{
    if(t == Entity::Type::PLAYER) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = m_SCREEN_HEIGHT - 70;
        int width = 100;
        int height = 20;
        std::pair<int,int> speed = {30, 0};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::PLAYER));
    }
    else if(t == Entity::Type::ENEMY) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = 60;
        int width = 100;
        int height = 20;
        std::pair<int,int> speed = {30, 0};
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::ENEMY));
    }
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

        // Check for boundary conditions
        if(entity->getType() == Entity::Type::BALL) continue;

        if(entity->getX() > m_SCREEN_WIDTH - entity->getWidth()) {
            entity->setX(m_SCREEN_WIDTH - entity->getWidth());
        }
        else if(entity->getX() <  0) {
            entity->setX(0);
        }
    }
}

void EntityManager::render(SDL_Renderer* renderer)
{
    for(auto& entity : m_Entities) {
        entity->render(renderer);
    }
}
