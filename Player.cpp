#include "Player.h"
#include "RandomNumberGenerator.h"
#include "SDL_image.h"
#include <iostream>

Player::Player(SDL_Renderer* pRenderer, SDL_Surface* pImagePlayer, SDL_Surface* pImageBar, SDL_Surface* pImageParticle, float x, float y,
	CollisionReferee* pReferee, SoundReferee* pSoundReferee)
	: m_health(m_kMaxHealth)
	, m_healthBar(Vector2{ (float)x, (float)y }, pRenderer, pImageBar, m_kMaxHealth)
	, m_pParticleTrail(nullptr)
	, m_pParticleTexture(nullptr)
	, m_Speed(m_kNormalSpeed)
	, m_x(x)
	, m_y(y)
	, m_horizontalDirection(0)
	, m_verticalDirection(0)
	, m_animation(12, 100, 100, 9, pRenderer, pImagePlayer)
	, m_collider(this, m_transform, pReferee, true)
	, m_pSoundReferee(pSoundReferee)
	, m_activeChannels{-1, -1, -1}
	, m_winCondition(false)
	, m_isSprinting(false)
	, m_catnips(0)

{
	// Sets rectangle coordinates as well as width and height
	m_transform.x = (float)x;
	m_transform.y = (float)y;
	m_transform.w = 100;
	m_transform.h = 100;
	m_collider.SetPosition(Vector2{ (float)m_transform.x, (float)m_transform.y });
	m_collider.SetSize(Vector2{ 100, 100 });

	// Sets animation sequences with name, first frame, and last frame
	m_animation.AddAnimationSequence("idle", 0, 0);
	m_animation.AddAnimationSequence("runLeft", 1, 2);
	m_animation.AddAnimationSequence("runRight", 3, 4);
	m_animation.AddAnimationSequence("runDown", 5, 6);
	m_animation.AddAnimationSequence("runUp", 7, 8);

	// Set particle texture and set properties
	m_pParticleTexture = SDL_CreateTextureFromSurface(pRenderer, pImageParticle);

	// Set particle trail
	m_pParticleTrail = new ParticleSystem{ Vector2{ m_x, m_y }, 64, 20, m_pParticleTexture };
	m_pParticleTrail->SetParticleSize(2);
	m_pParticleTrail->SetParticleSpeed(20);
}

Player::~Player()
{
	delete m_pParticleTrail;
	if (m_activeChannels[0] == 0)
	{
		Mix_HaltChannel(m_activeChannels[0]);
		m_activeChannels[0] = -1;
	}
	SDL_DestroyTexture(m_pParticleTexture);
}

///////////////////////////////////////////////////////////////////////
// Update object pos and animation with deltaTime and direction members
void Player::Update(double deltaTime)
{
	// Constexpr for directions
	constexpr int kLeft = -1;
	constexpr int kRight = 1;
	constexpr int kUp = -1;
	constexpr int kDown = 1;
	constexpr int kNoMovement = 0;

	ProcessInput();

	if (m_horizontalDirection == 0 && m_verticalDirection == 0)
	{
		m_animation.PlayAnimation("idle");
	}
	else if (m_horizontalDirection == kLeft && m_verticalDirection == kNoMovement)
	{
		m_animation.PlayAnimation("runLeft");
	}
	else if (m_horizontalDirection == kRight && m_verticalDirection == kNoMovement)
	{
		m_animation.PlayAnimation("runRight");
	}
	else if (m_horizontalDirection == kNoMovement && m_verticalDirection == kUp)
	{
		m_animation.PlayAnimation("runUp");
	}
	else if (m_horizontalDirection == kNoMovement && m_verticalDirection == kDown)
	{
		m_animation.PlayAnimation("runDown");
	}
	m_animation.Update(deltaTime);

	// Set Particle effect position
	if (m_verticalDirection == kDown)
	{
		if (m_pParticleTrail != nullptr)
			m_pParticleTrail->SetCenterPoint(Vector2{ (m_x + 50), m_y });
	}
	else
	{
		if (m_pParticleTrail != nullptr)
			m_pParticleTrail->SetCenterPoint(Vector2{ (m_x + 50), (m_y + 100) });
	}


	if (m_activeChannels[0] == -1)
	{
		if (m_horizontalDirection != 0 || m_verticalDirection != 0)
		{
			m_activeChannels[0] = m_pSoundReferee->PlaySound("walk", m_activeChannels[0], -1);
		}
	}
	else if (m_verticalDirection == 0 && m_horizontalDirection == 0) 
	{
		Mix_HaltChannel(m_activeChannels[0]);
		m_activeChannels[0] = -1;
	}

	this->TryMove(Vector2{ (float)m_horizontalDirection, (float)m_verticalDirection }, m_Speed, deltaTime);

	// Reset health to max if greater than max
	if (m_health > m_kMaxHealth)
		m_health = m_kMaxHealth;

	// Set animation position
	m_animation.SetTransformX(m_x);
	m_animation.SetTransformY(m_y);
	
	m_transform = m_animation.GetTransform();

	// Set health bar position
	constexpr float kYOffset = 10.f;
	m_healthBar.SetTransform(m_x, (m_y - kYOffset));
	m_healthBar.CheckHealth(m_health);

	// Check if sprinting
	if (m_Speed > m_kNormalSpeed)
	{
		m_isSprinting = true;
	}
	else
	{
		m_isSprinting = false;
	}

	// If sprinting, update particle
	if (m_isSprinting)
	{
		m_pParticleTrail->Update(deltaTime);
	}

	// Reset all channels when sound effect done playing
	for (auto& channel : m_activeChannels)
	{
		if (!Mix_Playing(channel))
		{
			channel = -1;
		}
	}

}

////////////////
// Render object
void Player::Render(SDL_Renderer* pRenderer)
{
	m_animation.Render(pRenderer);
	m_healthBar.Render(pRenderer);
	// If sprinting, render particle effect
	if (m_isSprinting)
	{
		m_pParticleTrail->Render(pRenderer);
	}

}

////////////////////////////////
// Process player keyboard input
bool Player::ProcessInput()
{
	// Get current keyboard state
	const Uint8* pKeyState = SDL_GetKeyboardState(nullptr);
	// Process keyboard states, bool is true if Lctrl+q
	bool quitCheck = ProcessContinuousKeys(pKeyState);

	SDL_Event evt;
	// Process any events until there are none left
	while (SDL_PollEvent(&evt) != 0)
	{

		// Check event type to know what kind it is
		switch (evt.type)
		{
			// If window is closed, quit the game by return true
		case SDL_WINDOWEVENT:
		{
			if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				return true;
			}
		}
		}
	}
	// Return false if !quit to continue loop, return true if quit
	return quitCheck;
}

/////////////////////////////////////////
// Process a player holding down a button
bool Player::ProcessContinuousKeys(const Uint8* pKeyState)
{
	// Constexpr for directions
	constexpr int kLeft = -1;
	constexpr int kRight = 1;
	constexpr int kUp = -1;
	constexpr int kDown = 1;
	constexpr int kNoMovement = 0;

	// If Lshift, sprint; else set back to normal 100
	if (pKeyState[SDL_SCANCODE_LSHIFT])
	{
		SetSpeed(300);
	}
	else
	{
		SetSpeed(100);
	}
	// Horizontal movement - if holding A, if D is also held, will idle instead
	if (pKeyState[SDL_SCANCODE_A] && !pKeyState[SDL_SCANCODE_D])
	{
		// Left
		SetHorizontalDirection(kLeft);
	}
	else if (pKeyState[SDL_SCANCODE_D] && !pKeyState[SDL_SCANCODE_A])
	{
		// Right
		SetHorizontalDirection(kRight);
	}
	else
	{
		// No movement
		SetHorizontalDirection(kNoMovement);
	}

	// Vertical Movement
	if (pKeyState[SDL_SCANCODE_W] && !pKeyState[SDL_SCANCODE_S])
	{
		// Up
		SetVerticalDirection(kUp);
	}
	else if (pKeyState[SDL_SCANCODE_S] && !pKeyState[SDL_SCANCODE_W])
	{
		// Down
		SetVerticalDirection(kDown);
	}
	else
	{
		// No movement
		SetVerticalDirection(kNoMovement);
	}

	// If Lctrl+Q are held, return true to quit program
	if (pKeyState[SDL_SCANCODE_LCTRL] && pKeyState[SDL_SCANCODE_Q])
	{
		return true;
	}
	// Return false to continue loop
	return false;
}

///////////////////////////////////////////////////////////
// Attempt to move object, return whether successful or not
bool Player::TryMove(Vector2 deltaPosition, double speed, double deltaTime)
{
	bool didMove = m_collider.TryMove(deltaPosition, speed, deltaTime);
	if (didMove)
	{

		// Determine movement amount of x with delta time, updates position, and then pass m_x to m_transform
		double deltaPositionX = speed * deltaTime;
		m_x += (float)deltaPositionX * deltaPosition.m_x;

		// Determine movement amount of y with delta time, updates position, and then pass m_y to m_transform
		double deltaPositionY = speed * deltaTime;
		m_y += (float)deltaPositionY * deltaPosition.m_y;
		
		m_transform.x = m_x;
		m_transform.y = m_y;
	}

	return didMove;
}

///////////////////////////////////////////
// When an item first collides with another
void Player::OnCollisionEntry(ColliderComponent* pOtherCollider)
{
	// Name object touched
	std::string objectCompared = pOtherCollider->GetOwner()->GetName();
	// Set the players status to being healed
	if (objectCompared == "catnip")
	{
		m_catnips += 1;
		pOtherCollider->GetOwner()->~GameObject();
		// Play "hit" sound because enemy will be damaged
		if (m_activeChannels[1] == -1)
		{
			m_activeChannels[1] = m_pSoundReferee->PlaySound("hit", m_activeChannels[1], 0);
		}
	}
	else if (objectCompared == "mouse")
	{
		m_health -= 100;
	}
	else if (objectCompared == "mysteryNip")
	{
		RandomNumberGenerator rng;
		bool positiveOutcome = rng.RandomInt(0, 1);
		// If bool is true, heal, else damage player, then delete object
		if (positiveOutcome)
			m_health += 100;
		else
			m_health -= 100;
		pOtherCollider->GetOwner()->~GameObject();
	}
	else if (objectCompared == "boss")
	{
		std::cout << m_health << std::endl;
		m_health -= 100;
	}
}

//////////////////////////////////////
// When collides with item, continuous
void Player::OnCollisionUpdate(ColliderComponent* pOtherCollider)
{
	// Name object touched
	std::string objectCompared = pOtherCollider->GetOwner()->GetName();
	// Set the players status to being healed
	if (objectCompared == "catnip")
	{
		m_catnips += 1;
		pOtherCollider->GetOwner()->~GameObject();
		// Play "hit" sound because enemy will be damaged
		if (m_activeChannels[1] == -1)
		{
			m_activeChannels[1] = m_pSoundReferee->PlaySound("hit", m_activeChannels[1], 0);
		}
	}
	// Randomly choose whether to heal or damage player
	else if (objectCompared == "mysteryNip")
	{
		RandomNumberGenerator rng;
		bool positiveOutcome = rng.RandomInt(0, 1);
		// If bool is true, heal, else damage player, then delete object
		if (positiveOutcome)
			m_health += 100;
		else
			m_health -= 100;
		pOtherCollider->GetOwner()->~GameObject();
		std::cout << m_health << std::endl;
	}
	// Player wins the game
	else if (objectCompared == "winZone")
	{
		m_winCondition = true;
		if (m_activeChannels[2] == -1)
		{
			m_activeChannels[2] = m_pSoundReferee->PlaySound("win", m_activeChannels[2], 0);
		}
		std::cout << "You won!" << std::endl;
	}
}