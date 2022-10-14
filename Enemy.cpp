#include "Enemy.h"
#include "SDL_image.h"
#include <iostream>

Enemy::Enemy(SDL_Renderer* pRenderer, SDL_Surface* pImageEnemy, SDL_Surface* pImageBar, float x, float y, CollisionReferee* pReferee, GameObject* pTarget)
	: m_x(x)
	, m_y(y)
	, m_healthBar(Vector2{ (float)x, (float)y }, pRenderer, pImageBar, m_kMaxHealth)
	, m_health(m_kMaxHealth)
	, m_horizontalDirection(0)
	, m_verticalDirection(0)
	, m_currentAnimation("idle")
	, m_playingAnimation(false)
	, m_animation(12, 40, 40, 5, pRenderer, pImageEnemy)
	, m_collider(this, m_transform, pReferee, true)
	, m_pTarget(pTarget)
	, m_stateMachine(this, pTarget, AIStateMachine::StateName::Pace)
{
	// Sets rectangle coordinates as well as width and height
	m_transform.x = x;
	m_transform.y = y;
	m_transform.w = 40;
	m_transform.h = 40;
	m_collider.SetPosition(Vector2{ (float)m_transform.x, (float)m_transform.y });
	m_collider.SetSize(Vector2{ 40, 40 });

	m_animation.AddAnimationSequence("idle", 0, 0);
	m_animation.AddAnimationSequence("runLeft", 1, 2);
	m_animation.AddAnimationSequence("runRight", 3, 4);

	m_horizontalDirection = 1;
}

Enemy::~Enemy()
{
}

///////////////////////////////
// Update object with deltaTime
void Enemy::Update(double deltaTime)
{
	constexpr float kMouseSpawnXAndY = 400;
	constexpr float kMouseBoundsLeft = 200;
	constexpr float kMouseBoundsRight = 700;
	
	Vector2 pathToTarget = m_pTarget->GetPosition() - this->GetPosition();	// Get path
	if (pathToTarget.GetSquareLength() <= m_kDetectDistanceSqr)				// Check distance
	{
		m_stateMachine.ChangeToState(AIStateMachine::StateName::Chase);		// Chase
	}
	else if(m_stateMachine.GetCurrentStateName() == AIStateMachine::StateName::Chase)
	{
		m_stateMachine.ChangeToState(AIStateMachine::StateName::Return);
	}
	else if(m_x > kMouseBoundsLeft && m_x < kMouseBoundsRight && m_y == kMouseSpawnXAndY && m_stateMachine.GetCurrentStateName() != (AIStateMachine::StateName::Pace))
	{
		m_stateMachine.ChangeToState(AIStateMachine::StateName::Pace);
	}
	
	if(m_health)

	if (m_horizontalDirection == 1)
	{
		m_animation.PlayAnimation("runLeft");
	}
	else if (m_horizontalDirection == -1)
	{
		m_animation.PlayAnimation("runRight");
	}
	else
	{
		m_animation.PlayAnimation("idle");
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

////////////////
// Render object
void Enemy::Render(SDL_Renderer* pRenderer)
{
	m_animation.Render(pRenderer);
	m_healthBar.Render(pRenderer);
}

///////////////////////////////////////////////////////////
// Attempt to move object, return whether successful or not
bool Enemy::TryMove(Vector2 deltaPosition, double speed, double deltaTime)
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

//////////////////////////
// When collides with item
void Enemy::OnCollisionUpdate(ColliderComponent* pOtherCollider)
{
	std::string objectCompared = pOtherCollider->GetOwner()->GetName();
	if (pOtherCollider->GetIsTrigger() == false)
	{
		m_horizontalDirection *= -1;
	}
}

//////////////////////////
// Attempts to move object
bool Enemy::Move(Vector2 direction, double deltaTime, float speedMultiplier)
{
	bool didMove = TryMove(direction, (m_Speed * speedMultiplier), deltaTime);
	if (direction.m_x > 0)
		m_horizontalDirection = 1;
	else if(direction.m_x < 0)
		m_horizontalDirection = -1;
	if (direction.m_y > 0)
		m_verticalDirection = 1;
	else if (direction.m_y < 0)
		m_verticalDirection = -1;
	return didMove;
}

