#pragma once
#include "Game.h"
#include <random>
#include <list>

class NPCController {
public:
    static NPCController& GetInstance(){
        static NPCController instance;
        return instance;
    }
    ~NPCController();

    std::list<NPC*>& GetNPCs() { return npcs; }

    void Update(Player& player);
    void Render();

private:
    Game* game;
    NPC* npcTemplate;
    Bullet* bulletTemplate;

    std::list<NPC*> npcs;
    std::mt19937 rng;
    std::uniform_real_distribution<float> distX;
    std::uniform_real_distribution<float> distY;

    NPCController();
    NPCController(const NPCController&) = delete;
    NPCController& operator=(const NPCController&) = delete;

    void SpawnNPC();
    void BulletRender(NPC* npc);
    void BulletUpdate(NPC* npc,Player& player);

};
