#pragma once
#include "ImageHandler.h"
#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include <SDL.h>
#include <vector>
#include <string>

class Catnip :
    public GameObject
{
private:
    // X position
    float m_x;
    // Y position
    float m_y;
    // Current animation
    std::string m_currentAnimation;
    // Allocate animation component
    AnimationComponent m_animation;
    // Allocate collider component
    ColliderComponent m_collider;

public:
    // Constructor - pass in renderer pointer + sizes of images, and destructor
    Catnip(SDL_Renderer* pRenderer, SDL_Surface* pImageCatnip, float w, float h,
        int frameRate, int frameCount, CollisionReferee* pReferee, Vector2 position);
    ~Catnip();

    // Set current animation
    void SetCurrentAnimation(std::string currentAnimation) { m_currentAnimation = currentAnimation; }



    // Inherited via GameObject
    virtual void Update(double deltaTime) override;
    virtual void Render(SDL_Renderer* pRenderer) override;
    virtual std::string GetName() override { return "catnip"; }
};

