#include "StartScene.h"

StartScene::StartScene()
    : game(&Game::GetInstance()), backgroundTexture(nullptr), font(nullptr) {
    // Load background texture
    backgroundTexture = IMG_LoadTexture(game->GetRenderer(), "../assets/background.png");
    if (!backgroundTexture) {
        throw std::runtime_error("Failed to load start scene background texture");
    }

    backgroundRect = { 0, 0, game->GetWindowWidth(), game->GetWindowHeight() };

    // Load font
    font = TTF_OpenFont("../assets/font.ttf", 24);
    if (!font) {
        throw std::runtime_error("Failed to load font");
    }

    textColor = { 255, 255, 255, 255 }; // White color
}

StartScene::~StartScene() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void StartScene::Update() {
    // Update logic for start scene (if any)
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
        Game::GetInstance().SetSceneID(1); // Switch back to main scene
    }
}

void StartScene::Render() {
    SDL_RenderCopy(game->GetRenderer(), backgroundTexture, nullptr, &backgroundRect);

    // Render title text
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Game start", textColor);
    if (titleSurface) {
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(game->GetRenderer(), titleSurface);
        if (titleTexture) {
            SDL_Rect titleRect = { (game->GetWindowWidth() - titleSurface->w) / 2,
                                    (game->GetWindowHeight() - titleSurface->h) / 2,
                                    titleSurface->w, titleSurface->h };
            SDL_RenderCopy(game->GetRenderer(), titleTexture, nullptr, &titleRect);
            SDL_DestroyTexture(titleTexture);
        }
        SDL_FreeSurface(titleSurface);
    }
}

void StartScene::OnHandleEvent(const SDL_Event& event) {

}