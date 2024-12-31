#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "./entity/entity.hpp"

#include <SDL2/SDL_render.h>
#include <vector>

class EntityManager {
public:
    EntityManager(int s_width, int s_height)
    : m_SCREEN_WIDTH(s_width), m_SCREEN_HEIGHT(s_height)
    {}
    void addEntity(const std::string& path, SDL_Renderer* renderer, Entity::Type t);

private:
    // Entities
    int m_SCREEN_WIDTH = 1280;
    int m_SCREEN_HEIGHT = 720;
    std::vector<std::unique_ptr<Entity>> m_Entities;
};

void EntityManager::addEntity(const std::string& path, SDL_Renderer* renderer, Entity::Type t)
{
    if(t == Entity::Type::PLAYER) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = m_SCREEN_HEIGHT - 50;
        int width = 50;
        int height = 20;
        int speed = 30;
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::PLAYER));
    }
    else if(t == Entity::Type::ENEMY) {
        int x = m_SCREEN_WIDTH / 2 - 25;
        int y = 50;
        int width = 50;
        int height = 20;
        int speed = 30;
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, x, y, width, height, speed, Entity::Type::ENEMY));
    }
    else if(t == Entity::Type::BALL) {
        m_Entities.push_back(std::make_unique<Entity>(path, renderer, 0, 0, 50, 50, 5, Entity::Type::BALL));
    }
}

#endif // ENTITY_MANAGER_HPP
