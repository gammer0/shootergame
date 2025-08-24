#pragma once
#include "Scene.h"
#include "Game.h"
#include <stdexcept>

class StartScene : public Scene
{
public:
    StartScene();
    ~StartScene();

    void Update() override;
    void OnHandleEvent(const SDL_Event& event) override;
    void Render() override;

private:
    Game* game = nullptr;
    SDL_Texture* backgroundTexture = nullptr;
    SDL_Rect backgroundRect;
    SDL_Color textColor;
    TTF_Font* font = nullptr;
};
