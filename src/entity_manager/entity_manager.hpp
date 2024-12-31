#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "../entity/entity.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <vector>

class EntityManager {
public:
    EntityManager(int s_width, int s_height)
    : m_SCREEN_WIDTH(s_width), m_SCREEN_HEIGHT(s_height)
    {}
    void addEntity(const std::string& path, SDL_Renderer* renderer, Entity::Type t);
    void handleEvent(SDL_Event& event);
    void render(SDL_Renderer* renderer);

    // Entities
    std::vector<std::unique_ptr<Entity>> m_Entities;

private:
    int m_SCREEN_WIDTH = 1280;
    int m_SCREEN_HEIGHT = 720;
};


#endif // ENTITY_MANAGER_HPP
