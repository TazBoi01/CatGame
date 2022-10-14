#pragma once
#include "GameObject.h"
#include "Vector2.h"

class ParticleSystem :
    public GameObject
{
private:
    /////////////////////////////////
    // Struct for individual particle
    struct Particle
    {
        Vector2 m_positionOffeset;
        Vector2 m_velocity;
    };

    ////////////////////////////
    // Struct for particle color
    struct Color
    {
        int m_r;
        int m_g;
        int m_b;
    
        Color(int r, int g, int b) { m_r = r; m_g = g; m_b = b; }
    };
private:
    //TODO: GET RID OF CONST
    // Particle source position
    Vector2 m_centerPoint;

    // Number of particles in this effect
    const int m_kParticleCount;

    // Square of max distance particles travel
    const float m_kRadiusSqr;

    // Array of particles
    Particle* m_pParticles;

    // Particles color if colored
    Color m_color;

    // Particles texture if textured
    SDL_Texture* m_pTexture;

    // Particle size
    int m_particleSize;
    
    // Particle speed
    float m_particleSpeed;

public:
    // Colored particle system
    ParticleSystem(Vector2 centerPoint, int particleCount, float radius, int r, int g, int b);

    // Textured particle system
    ParticleSystem(Vector2 centerPoint, int particleCount, float radius, SDL_Texture* pTexture);

    ~ParticleSystem();

    // Set particle speed
    void SetParticleSpeed(float newSpeed) { m_particleSpeed = newSpeed; }

    // Set particle size
    void SetParticleSize(int newSize) { m_particleSize = newSize; }

    // Set center point
    void SetCenterPoint(Vector2 newCenter) { m_centerPoint = newCenter; }

    // Get this objects name
    virtual std::string GetName() override { return "particleSystem"; }

    // Get this objects position
    virtual Vector2 GetPosition() override { return m_centerPoint; }

    // Update this objects state
    virtual void Update(double deltaTime) override;

    // Render this object
    virtual void Render(SDL_Renderer* pRenderer) override;
};

