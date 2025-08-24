#pragma once
#include "source.h"
class Scene
{
public:
    Scene() =default;
    virtual ~Scene() =default;

    virtual void Start(){};
    virtual void Update() =0;
    virtual void End(){};
    virtual void OnHandleEvent(const SDL_Event& event) =0;
    virtual void Render() =0;

protected:
    // Scene data and resources
};