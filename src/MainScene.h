#pragma once
#include "Scene.h"
#include "Game.h"
#include<list>

class MainScene : public Scene
{
public:
    MainScene();
    ~MainScene();

    void Update() override;
    void OnHandleEvent(const SDL_Event& event) override;
    void Render() override;

    const Player& GetPlayer() const { return player; }

private:
    Game* game = nullptr;
    Player player;
    SDL_Rect dstRect;
    const Uint8* state = nullptr;

    Bullet* bulletTemplate = nullptr;
    std::list<Bullet*> bullets;

    void PlayerKeyboardControl();
    void BulletRender();
    void BulletUpdate();
    void BulletCollision();

};