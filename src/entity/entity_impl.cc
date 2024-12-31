#include "entity.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

void Entity::render(SDL_Renderer* renderer)
{
    m_Texture->render(renderer, m_Xpos, m_Ypos);
}

void Entity::handleEvent(SDL_Event& event)
{
    // Player movement
    if(m_Type == Entity::Type::PLAYER) {
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_LEFT:
                    m_Xpos -= m_Xspeed * delataTime;
                    break;
                case SDLK_RIGHT:
                    m_Xpos += m_Xspeed * delataTime;
                    break;
                default:
                    break;
            }
        }
    }
    // Enemy movement
    else if(m_Type == Entity::Type::ENEMY) {
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_a:
                    m_Xpos -= m_Xspeed * delataTime;
                    break;
                case SDLK_d:
                    m_Xpos += m_Xspeed * delataTime;
                    break;
                default:
                    break;
            }
        }
    }
}
