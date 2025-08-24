#include "EndScene.h"
#include "Game.h"
#include <stdexcept>

EndScene::EndScene() : backgroundTexture(nullptr), font(nullptr) {
    // Load background texture
    backgroundTexture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "../assets/background.png");
    if (!backgroundTexture) {
        throw std::runtime_error("Failed to load end scene background texture");
    }

    backgroundRect = { 0, 0, Game::GetInstance().GetWindowWidth(), Game::GetInstance().GetWindowHeight() };

    // Load font
    font = TTF_OpenFont("../assets/font.ttf", 24);
    if (!font) {
        throw std::runtime_error("Failed to load font");
    }
}

EndScene::~EndScene() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void EndScene::Update() {
    // Update logic for end scene (if any)
    
}

void EndScene::Render() {
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), backgroundTexture, nullptr, &backgroundRect);

    // Render "Game Over" text
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), textSurface);
        if (textTexture) {
            SDL_Rect textRect = { (Game::GetInstance().GetWindowWidth() - textSurface->w) / 2,
                                   (Game::GetInstance().GetWindowHeight() - textSurface->h) / 2,
                                   textSurface->w, textSurface->h };
            SDL_RenderCopy(Game::GetInstance().GetRenderer(), textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}


void EndScene::OnHandleEvent(const SDL_Event& event) {

}