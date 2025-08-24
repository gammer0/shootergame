#include "NPCController.h"
#include "MainScene.h"
#include <stdexcept>
#include <cmath>

NPCController::NPCController()
    : game(&Game::GetInstance()),
      npcTemplate(new NPC()),
      distX(0.0f, static_cast<float>(game->GetWindowWidth())),
      distY(0.0f, static_cast<float>(game->GetWindowHeight())) {
    // Initialize NPC template

    try{
        npcTemplate->texture = IMG_LoadTexture(game->GetRenderer(), "../assets/NPC.png");
        npcTemplate->position = { 0, 0 };
        if (!npcTemplate->texture) {
            throw std::runtime_error("Failed to load NPC texture");
        }
        npcTemplate->velocity = 0.5f;
        npcTemplate->container = { 0, 0, 32, 32 };
        npcTemplate->lastAttackTime = 0;
        npcTemplate->attackCooldown = 1000; // 1 second cooldown
        npcTemplate->HP = 2;
        npcTemplate->damage = 1;

    } catch (const std::exception& e) {
        // Handle exceptions
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing NPCController: %s", e.what());
    }

    try{
        bulletTemplate = new Bullet();
        bulletTemplate->texture = IMG_LoadTexture(game->GetRenderer(), "../assets/Red_bullet.png");
        bulletTemplate->position = { 0, 0 };
        if (!bulletTemplate->texture) {
            throw std::runtime_error("Failed to load enemy bullet texture");
        }
        bulletTemplate->velocity = 0.7f;
        bulletTemplate->container = { 0, 0, 2 * 2, 4 * 2 };

    } catch (const std::exception& e) {
        // Handle exceptions
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing NPCController bullets: %s", e.what());
    }

    std::random_device rd;
    rng =std::mt19937(rd());

}

NPCController::~NPCController() {
    if(npcTemplate){
        if(npcTemplate->texture){
            SDL_DestroyTexture(npcTemplate->texture);
            npcTemplate->texture = nullptr;
        }
        delete npcTemplate;
        npcTemplate = nullptr;
    }

    if(bulletTemplate){
        if(bulletTemplate->texture){
            SDL_DestroyTexture(bulletTemplate->texture);
            bulletTemplate->texture = nullptr;
        }
        delete bulletTemplate;
        bulletTemplate = nullptr;
    }

    for(auto& npc : npcs){
        if(npc->texture){
            SDL_DestroyTexture(npc->texture);
            npc->texture = nullptr;

            for(auto& bullet : npc->bullets){
                if(bullet->texture){
                    SDL_DestroyTexture(bullet->texture);
                    bullet->texture = nullptr;
                }
                delete bullet;
            }
        }
        delete npc;
    }
}

void NPCController::Update(Player& player) {
    // Example: Spawn an NPC every 5 seconds
    static Uint32 lastSpawnTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastSpawnTime >= 1000) {
        SpawnNPC();
        lastSpawnTime = currentTime;
    }

    // Update NPC positions and behaviors
    for (auto it = npcs.begin(); it != npcs.end(); ) {
        NPC* npc = *it;
        npc->position.y += game->GetDeltaTime() * npc->velocity;

        // Update container position
        npc->container.x = static_cast<int>(npc->position.x - 16);
        npc->container.y = static_cast<int>(npc->position.y - 16);

        BulletUpdate(npc, player);

        // Remove NPC if it goes off-screen
        if (npc->position.y > game->GetWindowHeight()) {
            
            // for (auto bullet : npc->bullets) {
            //     if (bullet->texture) {
            //         SDL_DestroyTexture(bullet->texture);
            //         bullet->texture = nullptr;
            //     }
            //     delete bullet;
            // }
            // npc->bullets.clear();
            it = npcs.erase(it);
            delete npc;

        } else {
            ++it;
        }
    }
}

void NPCController::Render() {
    for (auto npc : npcs) {
        BulletRender(npc);        
        SDL_RenderCopyEx(game->GetRenderer(), npc->texture, nullptr, &npc->container, 0, nullptr, SDL_FLIP_VERTICAL);

    }
}

void NPCController::SpawnNPC() {
    NPC* newNPC = new NPC(*npcTemplate);
    newNPC->position.x = distX(rng);
    newNPC->position.y = 0;
    npcs.push_back(newNPC);
}

void NPCController::BulletRender(NPC* npc) {
        
        for (auto bullet : npc->bullets) {
            float angle = std::atan2(bullet->direction.y, bullet->direction.x) * 180.0f / M_PI - 90;
            SDL_RenderCopyEx(game->GetRenderer(), bullet->texture, nullptr, &bullet->container, angle, nullptr, SDL_FLIP_VERTICAL);
        }

}

void NPCController::BulletUpdate(NPC* npc, Player& player) {

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - npc->lastAttackTime >= npc->attackCooldown) {
            // Shoot a bullet

            Bullet* newBullet = new Bullet(*bulletTemplate);
            newBullet->position.x = npc->position.x;
            newBullet->position.y = npc->position.y + 16; // Start below the NPC
            newBullet->container = { static_cast<int>(newBullet->position.x - 2), static_cast<int>(newBullet->position.y), 2 * 2, 4 * 2 };
            npc->bullets.push_back(newBullet);
            npc->lastAttackTime = currentTime;

            // Set bullet direction towards player
            SDL_FPoint direction = { player.position.x - newBullet->position.x, player.position.y - newBullet->position.y };
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
            }
            newBullet->direction = direction;
        }

        for (auto it = npc->bullets.begin(); it != npc->bullets.end(); ) {
            Bullet* bullet = *it;

            if (SDL_HasIntersection(&bullet->container, &player.container)) {
                // Handle collision (e.g., reduce player health)
                SDL_Log("Player hit by NPC bullet!");
                player.HP -= npc->damage;
                it = npc->bullets.erase(it);
                delete bullet;
                continue;
            }

            if(bullet->position.y > game->GetWindowHeight() + 32){
                it = npc->bullets.erase(it);
                delete bullet;
            } else {
                bullet->position.x += bullet->direction.x * bullet->velocity * game->GetDeltaTime();
                bullet->position.y += bullet->direction.y * bullet->velocity * game->GetDeltaTime();

                bullet->container.x = static_cast<int>(bullet->position.x - 2);
                bullet->container.y = static_cast<int>(bullet->position.y);
                ++it;
            }


        }
}
