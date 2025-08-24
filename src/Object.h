#pragma once
#include "source.h"
#include <list>

typedef struct Player{
    SDL_Texture* texture;
    SDL_FPoint position;
    SDL_Rect container;
    float velocity;

    Uint32 bulletCooldown;
    Uint32 lastShotTime;

    int HP;
    int damage;
} Player;

typedef struct Bullet{
    SDL_Texture* texture;
    SDL_FPoint position;
    SDL_Rect container;
    float velocity;
    SDL_FPoint direction;
} Bullet;

typedef struct NPC{
    SDL_Texture* texture;
    SDL_FPoint position;
    SDL_Rect container;
    float velocity;

    Uint32 lastAttackTime;
    Uint32 attackCooldown;

    std::list<Bullet*> bullets;

    int HP;
    int damage;

} NPC;