#pragma once
#include "Scene.h"
class EndScene : public Scene {
public:
    EndScene();
    ~EndScene();

    void Update() override;
    void Render() override;
    void OnHandleEvent(const SDL_Event& event) override;

private:
    SDL_Texture* backgroundTexture;
    SDL_Rect backgroundRect;
    TTF_Font* font;
};