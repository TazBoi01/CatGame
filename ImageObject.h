#pragma once
#include "ImageHandler.h"
#include <vld.h>
#include <SDL.h>
#include <vector>
#include <string>

class ImageObject :
    public GameObject
{
private:
    // X position
    float m_x;

    // Y position
    float m_y;

    // Number of Objects created
    int m_numberObjects;

    // Rectangle for object rendering
    std::vector<SDL_FRect> m_transformVector;

    // Texture for object appearance
    SDL_Texture* m_pTexture;

public:
    // Constructor and destructor
    ImageObject(SDL_Renderer* pRenderer, SDL_Surface* pImageSurface, int numberObjects, float w, float h);
    ~ImageObject();
    // Inherited via GameObject
    virtual void Update(double deltaTime) override;
    virtual void Render(SDL_Renderer* pRenderer) override;
};
