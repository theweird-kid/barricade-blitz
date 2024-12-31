#include "entity.hpp"
#include <SDL2/SDL_render.h>

void Entity::render(SDL_Renderer* renderer)
{
    m_Texture->render(renderer, m_Xpos, m_Ypos);
}
