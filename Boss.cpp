#include "Boss.h"
#include "RandomNumberGenerator.h"
#include <ctime>
#include <iostream>

Boss::Boss(SDL_Renderer* pRenderer, SDL_Surface* pImageBoss, SDL_Surface* pImageBar, float x, float y, GameObject* pTarget, CollisionReferee* pReferee)
	: m_health(1000)
	, m_healthBar(Vector2{ (float)x, (float)y }, pRenderer, pImageBar, m_kMaxHealth)
	, m_x(x)
	, m_y(y)
	, m_pTarget(pTarget)
	, m_currentAnimation("idle")
	, m_playingAnimation(false)
	, m_animation(12, 60, 100, 6, pRenderer, pImageBoss)
	, m_collider(this, m_transform, pReferee, true)
	, m_stateMachine(this, pTarget, AIStateMachine::StateName::Pace)
	, m_seconds(2)
	, m_verticalDirection(0)
	, m_horizontalDirection(1)
	, m_pRenderer(pRenderer)
	, m_pReferee(pReferee)
	, m_rng()
{
	// Sets rectangle coordinates as well as width and height
	m_transform.x = x;
	m_transform.y = y;
	m_transform.w = 60;
	m_transform.h = 100;
	m_collider.SetPosition(Vector2{ (float)m_transform.x, (float)m_transform.y });
	m_collider.SetSize(Vector2{ 60, 100 });

	m_animation.AddAnimationSequence("runLeft", 0, 1);
	m_animation.AddAnimationSequence("runRight", 2, 3);
	m_animation.AddAnimationSequence("charging", 4, 5);
}

Boss::~Boss()
{
}

void Boss::Update(double deltaTime)
{
	constexpr double kFirstPhasePercentage = .65;
	constexpr double kSecondPhasePercentage = .25;

	if (m_health > (m_kMaxHealth * kFirstPhasePercentage))
	{
	}
	else if (m_health < (m_kMaxHealth * kFirstPhasePercentage) && m_health > (m_kMaxHealth * kSecondPhasePercentage))
	{
		// Choose a number 2-4 seconds
		m_seconds -= deltaTime;
		if (m_seconds <= 0)
		{
			m_seconds = m_rng.RandomFloat(1, 2);
			if (m_stateMachine.GetCurrentStateName() == AIStateMachine::StateName::Pace)
			{
				std::cout << "boog" << std::endl;
				int choice = m_rng.RandomInt(1, 2);
				switch (choice)
				{
				case 0:
				{
					m_stateMachine.ChangeToState(AIStateMachine::StateName::Pace);
					break;
				}
				case 1:
				{
					m_horizontalDirection = 0;
					m_verticalDirection = 0;
					m_stateMachine.ChangeToState(AIStateMachine::StateName::Charge);
					break;
				}
				}
			}
		}
	}
	else if (m_health <= (m_kMaxHealth * kSecondPhasePercentage))
	{
		// Set the state to Chase for under .25
		if (m_stateMachine.GetCurrentStateName() == AIStateMachine::StateName::Pace)
		{
			m_stateMachine.ChangeToState(AIStateMachine::StateName::Chase);
		}
		// Choose a number 2-4 seconds
		m_seconds -= deltaTime;
		if (m_seconds <= 0)
		{
			m_seconds = m_rng.RandomFloat(2, 4);
			if (m_stateMachine.GetCurrentStateName() == AIStateMachine::StateName::Chase) 
			{
				int choice = m_rng.RandomInt(0, 1);
				switch (choice)
				{
				case 0:
				{
					m_stateMachine.ChangeToState(AIStateMachine::StateName::Chase);
					break;
				}
				case 1:
				{
					m_horizontalDirection = 0;
					m_verticalDirection = 0;
					m_stateMachine.ChangeToState(AIStateMachine::StateName::Charge);
					break;
				}
				}
			}
		}
	}

	if (m_horizontalDirection == 1)
	{
		m_animation.PlayAnimation("runLeft");
	}
	else if (m_horizontalDirection == -1)
	{
		m_animation.PlayAnimation("runRight");
	}
	else if (m_horizontalDirection == 0 && m_verticalDirection == 0)
	{
		m_animation.PlayAnimation("charging");
	}

	m_animation.Update(deltaTime);

	m_animation.SetTransformX(m_x);
	m_animation.SetTransformY(m_y);

	m_transform = m_animation.GetTransform();

    m_stateMachine.Update(deltaTime);

	// Update Health Bar
	constexpr float kYOffset = 10.f;
	m_healthBar.SetTransform(m_x, (m_y - kYOffset));
	m_healthBar.CheckHealth(m_health);
}

void Boss::Render(SDL_Renderer* pRenderer)
{
	m_animation.Render(pRenderer);
	m_healthBar.Render(pRenderer);
}


///////////////////////////////////////////////////////////
// Attempt to move object, return whether successful or not
bool Boss::TryMove(Vector2 deltaPosition, double speed, double deltaTime)
{
	bool didMove = m_collider.TryMove(deltaPosition, speed, deltaTime);
	if (didMove)
	{

		// Determine movement amount of x with delta time, updataes position, and then pass m_x to m_transform
		double deltaPositionX = speed * deltaTime;
		m_x += (float)deltaPositionX * deltaPosition.m_x;

		// Determine movement amount of y with delta time, updataes position, and then pass m_y to m_transform
		double deltaPositionY = speed * deltaTime;
		m_y += (float)deltaPositionY * deltaPosition.m_y;

		m_transform.x = m_x;
		m_transform.y = m_y;
	}

	return didMove;
}

bool Boss::Move(Vector2 direction, double deltaTime, float speedMultiplier)
{
	bool didMove = TryMove(direction, (m_kSpeed * speedMultiplier), deltaTime);
	
	if (direction.m_x > 0)
		m_horizontalDirection = 1;
	else if (direction.m_x < 0)
		m_horizontalDirection = -1;
	if (direction.m_y > 0)
		m_verticalDirection = 1;
	else if (direction.m_y < 0)
		m_verticalDirection = -1;

	return didMove;
}

///////////////////////////////////////////
// When an item first collides with another
void Boss::OnCollisionEntry(ColliderComponent* pOtherCollider)
{
	constexpr int kMaxHealth = 1000;
	std::string objectCompared = pOtherCollider->GetOwner()->GetName();
	if (objectCompared == "player")
	{
		// If boss is charging, and hits the player, heal self
		if (m_health < kMaxHealth && m_stateMachine.GetCurrentStateName() == AIStateMachine::StateName::Charge)
		{
			m_health +=50;
			if (m_health > kMaxHealth)
				m_health = kMaxHealth;
		}
		std::cout << "Enemy: " << m_health << std::endl;
	}
}