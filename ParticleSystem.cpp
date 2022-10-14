#include "ParticleSystem.h"
#include "RandomNumberGenerator.h"

ParticleSystem::ParticleSystem(Vector2 centerPoint, int particleCount, float radius, int r, int g, int b)
	: m_centerPoint(centerPoint)
	, m_kParticleCount(particleCount)
	, m_kRadiusSqr(radius * radius)
	, m_pParticles(new Particle[particleCount])
	, m_pTexture(nullptr)
	, m_color(r, g, b)
	, m_particleSize(1)
	, m_particleSpeed(10)
{
	RandomNumberGenerator rng;

	// Initialize random setup.
	std::random_device device;
	std::mt19937 twister;
	twister.seed(device());
	std::uniform_real_distribution<float> random(-1, 1);

	// Set each particles initial position and a random velocity
	for (int i = 0; i < particleCount; ++i)
	{
		m_pParticles[i].m_positionOffeset = Vector2{ 0,0 };

		// Particles will travel up to maxSpeed pixels per second
		m_pParticles[i].m_velocity = Vector2{ rng.RandomFloat(-1.f, 1.f), rng.RandomFloat(-1.f, 1.f) } * m_particleSpeed;
	}
}

ParticleSystem::ParticleSystem(Vector2 centerPoint, int particleCount, float radius, SDL_Texture* pTexture)
	: m_centerPoint(centerPoint)
	, m_kParticleCount(particleCount)
	, m_kRadiusSqr(radius* radius)
	, m_pParticles(new Particle[particleCount])
	, m_pTexture(pTexture)
	, m_color(-1, -1, -1)
	, m_particleSize(1)
	, m_particleSpeed(10)
{
	RandomNumberGenerator rng;

	// Initialize random setup.
	std::random_device device;
	std::mt19937 twister;
	twister.seed(device());
	std::uniform_real_distribution<float> random(-1, 1);

	// Set each particles initial position and a random velocity
	for (int i = 0; i < particleCount; ++i)
	{
		m_pParticles[i].m_positionOffeset = Vector2{ 0,0 };

		// Particles will travel up to maxSpeed pixels per second
		m_pParticles[i].m_velocity = Vector2{ rng.RandomFloat(-1.f, 1.f), rng.RandomFloat(-1.f, 1.f) } * m_particleSpeed;
	}
}

ParticleSystem::~ParticleSystem()
{
	// Delete array of particles
	delete[] m_pParticles;
}

void ParticleSystem::Update(double deltaTime)
{
	for (int i = 0; i < m_kParticleCount; ++i)
	{
		// Update each particles position
		m_pParticles[i].m_positionOffeset.m_x += m_pParticles[i].m_velocity.m_x * (float)deltaTime;
		m_pParticles[i].m_positionOffeset.m_y += m_pParticles[i].m_velocity.m_y * (float)deltaTime;

		// Reset particle if it goes outside of radius
		if (m_pParticles[i].m_positionOffeset.GetSquareLength() >= m_kRadiusSqr)
		{
			m_pParticles[i].m_positionOffeset = Vector2{ 0,0 };
		}
	}
}

void ParticleSystem::Render(SDL_Renderer* pRenderer)
{
	for (int i = 0; i < m_kParticleCount; ++i)
	{
		Particle& particle = m_pParticles[i];
		Vector2& particlePosition = particle.m_positionOffeset;
	
		if (m_pTexture != nullptr)
		{
			SDL_FRect particleTransform{ (m_centerPoint.m_x + particlePosition.m_x),
										(m_centerPoint.m_y + particlePosition.m_y), (float)m_particleSize, (float)m_particleSize };
			SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &particleTransform);
		}
		else
		{
			SDL_Rect particleTransform{ (int)(m_centerPoint.m_x + particlePosition.m_x),
									(int)(m_centerPoint.m_y + particlePosition.m_y), m_particleSize, m_particleSize };
			SDL_SetRenderDrawColor(pRenderer, (Uint8)m_color.m_r, (Uint8)m_color.m_g, (Uint8)m_color.m_b, 255);
			SDL_RenderFillRect(pRenderer, &particleTransform);
		}
	}
}
