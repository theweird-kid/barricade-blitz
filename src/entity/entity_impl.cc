#include "entity.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

void Entity::render(SDL_Renderer* renderer)
{
    update();
    m_Texture->render(renderer, m_Xpos, m_Ypos);
}

void Entity::handleEvent(SDL_Event& event)
{
    // Player movement
    if(m_Type == Entity::Type::PLAYER ) {
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_LEFT:
                    m_Xvel = -m_Xspeed * delataTime;
                    break;
                case SDLK_RIGHT:
                    m_Xvel = m_Xspeed * delataTime;
                    break;
                default:
                    break;
            }
        }
        else if(event.type == SDL_KEYUP) {
            switch(event.key.keysym.sym) {
                case SDLK_LEFT:
                    m_Xvel = 0;
                    break;
                case SDLK_RIGHT:
                    m_Xvel = 0;
                    break;
                default:
                    break;
            }
        }
    }

    // Enemy movement
    if(m_Type == Entity::Type::ENEMY)
    {
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_a:
                    m_Xvel = -m_Xspeed * delataTime;
                    break;
                case SDLK_d:
                    m_Xvel = m_Xspeed * delataTime;
                    break;
                default:
                    break;
            }
        }
        else if(event.type == SDL_KEYUP) {
            switch(event.key.keysym.sym) {
                case SDLK_a:
                    m_Xvel = 0;
                    break;
                case SDLK_d:
                    m_Xvel = 0;
                    break;
                default:
                    break;
            }
        }
    }
}

void Entity::reset()
{
    if(m_Type == Entity::Type::PLAYER) {
        m_Xpos = m_SCREEN_WIDTH / 2 - 25;
        m_Ypos = m_SCREEN_HEIGHT - 70;
    }
    else if(m_Type == Entity::Type::ENEMY) {
        m_Xpos = m_SCREEN_WIDTH / 2 - 25;
        m_Ypos = 60;
    }
    else if(m_Type == Entity::Type::BALL) {
        m_Xpos = m_SCREEN_WIDTH/2;
        m_Ypos = m_SCREEN_HEIGHT/2;
    }
}

void Entity::update()
{
    m_Xpos += m_Xvel;
    m_Ypos += m_Yvel;
}
