#include "bullet.h"

void Bullet::Draw(SDL_Renderer *renderer) {
    SDL_FRect rect = {
        this->position.x,
        this->position.y,
        this->width,
        this->height
    };
    SDL_SetRenderDrawColorFloat(renderer, this->color.r, this->color.g, this->color.b, 1.f);
    SDL_RenderFillRect(renderer, &rect);
    //SDL_RenderTexture(renderer, images::bullet_texture, &images::bullet_src_rect, &rect);

    for (size_t i = 0; i < this->particles.size(); i++)
    {
        Particle& particle = this->particles.at(i);
        particle.Draw(renderer);
    }
}

void Bullet::Update(uint32_t deltaTime) {
    Bullet& me = *(this);
    me.position.x += me.velocity.x * deltaTime;
    me.position.y += me.velocity.y * deltaTime;

    me.particleSpawnDelay += deltaTime;
    if (me.particleSpawnDelay > me.particleSpawnRate)
    {
        me.particleSpawnDelay = 0;
        Particle p{
            me.position.x + 2.5f,
            me.position.y
        };
        p.color.r = 1.f;
        p.color.g = 0.f;
        p.color.b = 0.f;
        p.velocity.y = me.velocity.y / 2.f;
        me.particles.push_back(p);
    }

    for (size_t i = 0; i < me.particles.size(); i++)
    {
        Particle& particle = me.particles.at(i);

        particle.Update(deltaTime);

        particle.lifetime += deltaTime;
        if (particle.lifetime > particle.maxLifetime)
        {
            me.particles.erase(me.particles.begin() + i);
            break;
        }
    }
}
