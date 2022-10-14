#pragma once
#include "ImageHandler.h"

class Background :
    public GameObject
{
private:
    // Width of window
    int m_windowW;

    // Height of window
    int m_windowH;

    // Rect for background tile
    SDL_FRect m_transform;

    // Texture for object appearance
    SDL_Texture* m_pTexture;

public:
    // Constructor and destructor
    Background(SDL_Renderer* pRenderer, SDL_Surface* pImageCarpet, int windowW, int windowH, float w, float h);
    ~Background();

    // Inherited via GameObject
    virtual void Update(double deltaTime) override;
    virtual void Render(SDL_Renderer* pRenderer) override;
};

