#include "MainScene.h"
#include "NPCController.h"
#include <stdexcept>
MainScene::MainScene(): game(&Game::GetInstance())
{
    try{
        player.texture = IMG_LoadTexture(game->GetRenderer(), "../assets/plane.png");  // Assign the appropriate texture
        player.position = { static_cast<float>(game->GetWindowWidth() / 2), static_cast<float>(game->GetWindowHeight() * 5 / 6) };
        player.velocity = 1;

        player.container = { static_cast<int>(std::lround(player.position.x - 32)), static_cast<int>(std::lround(player.position.y - 32)), 32 * 2, 32 * 2 }; 

        player.bulletCooldown = 250; // milliseconds
        player.HP = 3;
        player.damage = 1;

        if(!player.texture){
            throw std::runtime_error("Failed to load texture");
        }
    }catch(const std::exception& e){
        SDL_Log("Error: %s", e.what());
        exit(EXIT_FAILURE);
    }

    state = SDL_GetKeyboardState(NULL);

    try{
        bulletTemplate = new Bullet();
        bulletTemplate->texture = IMG_LoadTexture(game->GetRenderer(), "../assets/Blue_bullet.png");
        bulletTemplate->position = { 0, 0 };
        bulletTemplate->velocity = 1.25;

        bulletTemplate->container = { static_cast<int>(std::lround(bulletTemplate->position.x - 2)), static_cast<int>(std::lround(bulletTemplate->position.y - 8)), 2 * 2, 4 * 2 };

        if(!bulletTemplate->texture){
            throw std::runtime_error("Failed to load bullet texture");
        }
    }catch(const std::exception& e){
        SDL_Log("Error: %s", e.what());
        exit(EXIT_FAILURE);
    }
}

MainScene::~MainScene()
{
    if(player.texture){
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }

    if(bulletTemplate){
        if(bulletTemplate->texture){
            SDL_DestroyTexture(bulletTemplate->texture);
            bulletTemplate->texture = nullptr;
        }
        delete bulletTemplate;
        bulletTemplate = nullptr;
    }

    for(auto& bullet : bullets){
        if(bullet->texture){
            SDL_DestroyTexture(bullet->texture);
            bullet->texture = nullptr;
        }
        delete bullet;
    }
}

void MainScene::Update()
{
    PlayerKeyboardControl();
    BulletUpdate();
    NPCController::GetInstance().Update(player);

    BulletCollision();

    if (player.HP <= 0) {
        // Handle player death (e.g., restart game, show game over screen)
        SDL_Log("Player has died!");
        game->End(); // End the game for simplicity
    }

}

void MainScene::OnHandleEvent(const SDL_Event& event)
{
}

void MainScene::Render()
{

    BulletRender();
    // Render the player scaled
    SDL_RenderCopy(game->GetRenderer(), player.texture, nullptr, &player.container);
    NPCController::GetInstance().Render();
}

void MainScene::PlayerKeyboardControl()
{

    if(state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]){
        player.position.y -= game->GetDeltaTime() * player.velocity;
    }
    if(state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]){
        player.position.y += game->GetDeltaTime() * player.velocity;
    }
    if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]){
        player.position.x -= game->GetDeltaTime() * player.velocity;
    }
    if(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]){
        player.position.x += game->GetDeltaTime() * player.velocity;
    }

    // Keep the player within the window bounds
    if(player.position.x < 32){
        player.position.x = 32;
    }
    if(player.position.x > game->GetWindowWidth() - 32){
        player.position.x = game->GetWindowWidth() - 32;
    }
    if(player.position.y < 32){
        player.position.y = 32;
    }
    if(player.position.y > game->GetWindowHeight() - 32){
        player.position.y = game->GetWindowHeight() - 32;
    }

    // Update the destination rectangle for rendering
    player.container.x = static_cast<int>(std::lround(player.position.x - 32));
    player.container.y = static_cast<int>(std::lround(player.position.y - 32));

    if(state[SDL_SCANCODE_J]){
        Uint32 currentTime = SDL_GetTicks();
        if(currentTime - player.lastShotTime >= player.bulletCooldown){
            // Shoot a bullet
            [this]() {
                Bullet* newBullet = new Bullet(*bulletTemplate);
                newBullet->position.x = player.position.x;
                newBullet->position.y = player.position.y; // Start above the player

                newBullet->container = { static_cast<int>(newBullet->position.x), static_cast<int>(newBullet->position.y), 2 * 2, 4 * 2 };
                bullets.push_back(newBullet);
            }();
            player.lastShotTime = currentTime;
        }


    }
}

void MainScene::BulletRender()
{
    for (const auto& bullet : bullets) {
        SDL_RenderCopy(game->GetRenderer(), bullet->texture, nullptr, &bullet->container);
    }
    // SDL_Log("rendering bullets");
}


void MainScene::BulletUpdate()
{

    for (auto it = bullets.begin(); it != bullets.end(); ) {
        Bullet* bullet = *it;
        if(bullet->position.y < -32){
            it = bullets.erase(it);
            delete bullet;
        } else {
            bullet->position.y -= game->GetDeltaTime() * bullet->velocity;

            bullet->container.x = static_cast<int>(bullet->position.x - 2);
            bullet->container.y = static_cast<int>(bullet->position.y);
            ++it;
        }
    }
    // SDL_Log("Bullet count: %zu", bullets.size());
}

void MainScene::BulletCollision()
{
    auto& npcs = NPCController::GetInstance().GetNPCs();
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        Bullet* bullet = *bulletIt;
        bool bulletErased = false;

        for (auto npcIt = npcs.begin(); npcIt != npcs.end(); ) {
            NPC* npc = *npcIt;

            // Check for collision
            if (SDL_HasIntersection(&bullet->container, &npc->container)) {
                // Collision detected
                npc->HP -= player.damage; // Reduce NPC HP by player's damage
                if (npc->HP <= 0) {
                    // NPC is dead, remove it
                    npcIt = npcs.erase(npcIt);
                    delete npc;
                } else {
                    ++npcIt;
                }

                // Remove the bullet
                bulletIt = bullets.erase(bulletIt);
                delete bullet;
                bulletErased = true;
                break; // Exit the inner loop since the bullet is gone
            } else {
                ++npcIt;
            }
        }

        if (!bulletErased) {
            ++bulletIt; // Only increment if the bullet wasn't erased
        }
    }
}


