#include "Game.h"
#include "Scene.h"

#include "MainScene.h"
#include "EndScene.h"
#include "StartScene.h"

Game::Game()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to initialize SDL: %s", SDL_GetError());
    }
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to initialize SDL_image: %s", SDL_GetError());
    }
    if(TTF_Init() != 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to initialize SDL_ttf: %s", SDL_GetError());
    }
    if(Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL_mixer: %s", SDL_GetError());
    }

    window = SDL_CreateWindow("Shooter Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if(!window){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window: %s", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create renderer: %s", SDL_GetError());
    }

}

Game::~Game()
{
    for (auto scene : scenes) {
        delete scene;
    }
    scenes.clear();

    // if(currentScene){
    //     delete currentScene;
    //     currentScene = nullptr;
    // }

    if(renderer){
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window){
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::Start()
{   
    isRunning = true;
    deltaTime = static_cast<Uint32>(1000.0f / fps);
    scenes = { new StartScene(), new MainScene(), new EndScene() };
    sceneID = 0;
    currentScene = scenes[sceneID];
    while(isRunning){
        SDL_Event event;
        Uint32 frameStart = SDL_GetTicks();
        OnHandleEvent(event);
        Update();
        Render();
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < deltaTime){
            SDL_Delay(deltaTime - frameTime);
        }
    }
}

void Game::Update()
{
    if(currentScene){
        currentScene->Update();
    }

}

void Game::End()
{
    SetSceneID(2);
}

void Game::OnHandleEvent(SDL_Event &event)
{
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
            break;
        }
        
        if(currentScene){
            currentScene->OnHandleEvent(event);
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(currentScene){
        currentScene->Render();
    SDL_RenderPresent(renderer);
    }
}
