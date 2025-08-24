#pragma once
#include "source.h"
#include "Scene.h"
#include "Object.h"
#include <vector>
class Game {
public:
    ~Game();

    void Start();
    void Update();
    void End();
    void OnHandleEvent(SDL_Event& event);
    void Render();
    SDL_Renderer* GetRenderer() const { return renderer; }
    SDL_Window* GetWindow() const { return window; }
    int GetWindowWidth() const { return windowWidth; }
    int GetWindowHeight() const { return windowHeight; }
    int GetFPS() const { return fps; }
    Uint32 GetDeltaTime() const { return deltaTime; }
    void SetSceneID(int id) {
        sceneID = id;
        currentScene =scenes[sceneID];
    }

    void SetScene(Scene* scene) {
        currentScene = scene;
    }

    static Game& GetInstance(){
        static Game instance;
        return instance;
    }
private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Scene* currentScene = nullptr;
    bool isRunning = false;
    int windowWidth = 800;
    int windowHeight = 600;
    int fps = 240;
    Uint32 deltaTime;

    int sceneID;
    std::vector<Scene*> scenes;
};