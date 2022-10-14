#include "StateBoss.h"
#include "Background.h"
#include "Boss.h"
#include "Catnip.h"
#include "ParticleSystem.h"
#include "RandomNumberGenerator.h"
#include <iostream>

StateBoss::StateBoss(CatGameStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pPlayer(nullptr)
	, m_pBoss(nullptr)
	, m_pRenderer(pOwner->GetGame()->GetRenderer())
	, m_pCollisionReferee(pOwner->GetGame()->GetReferee())
	, m_pSoundReferee(pOwner->GetGame()->GetSoundReferee())
	, m_pImageHandler(pOwner->GetGame()->GetImageHandler())
	, m_catnipNumber(0)
	, m_catnipSpawned(true)
	, m_seconds(4)
	, m_gameObjects()
{
}

StateBoss::~StateBoss()
{
}

/////////////////////////
// Initializes this scene
void StateBoss::Enter()
{
	constexpr int kStateGameplay = 0;
	constexpr int kStateBoss = 1;

	// Check if there is saved data.
	SaveData* pSave = m_pOwner->GetSave();
	SaveData::Data& data = pSave->GetData(m_pOwner->GetIndex());

	// If data not loaded this sesson, set default values
	if (pSave->GetIsLoaded() == false || data.m_currentScene == kStateGameplay)
	{
		data.m_playerPosition = Vector2{ 10.f, 20.f };
		data.m_bossPosition = Vector2{ 400.f, 400.f };
		data.m_bossHealth = 1000;
		data.m_health = 500;
		data.m_bossIsCharging = false;
	}

	// Set current game state
	data.m_currentScene = kStateBoss;

	// Allocate ImageHandler
	ImageHandler handler;

	// Allocate background tile
	Background* pBackground = new Background(m_pRenderer, m_pImageHandler->GetImageSurface("background"), 800, 600, 100, 100);
	m_gameObjects.push_back(pBackground);


	// Create new player object
	Player* pPlayer = new Player(m_pRenderer, m_pImageHandler->GetImageSurface("cat"), m_pImageHandler->GetImageSurface("healthBar"), m_pImageHandler->GetImageSurface("background"), 
								data.m_playerPosition.m_x, data.m_playerPosition.m_y, m_pCollisionReferee, m_pSoundReferee);
	m_pPlayer = pPlayer;
	m_gameObjects.push_back(m_pPlayer);

	// Create new enemy object
	Boss* pMouse = new Boss(m_pRenderer, m_pImageHandler->GetImageSurface("boss"), m_pImageHandler->GetImageSurface("healthBar"), 
							data.m_bossPosition.m_x, data.m_bossPosition.m_y, pPlayer, m_pCollisionReferee);
	m_pBoss = pMouse;
	m_gameObjects.push_back(pMouse);

	// Check if boss was charging when saved
	if (data.m_bossIsCharging == true)
		m_pBoss->SetCurrentState(AIStateMachine::StateName::Return);

	// Create new Catnip object
	Catnip* pCatnip = new Catnip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, Vector2{350, 50});
	m_gameObjects.push_back(pCatnip);
	
	// Set player and boss health
	m_pPlayer->SetHealth(data.m_health);
	m_pBoss->SetHealth(data.m_bossHealth);
}

/////////////////////
// Updates this scene
void StateBoss::Update(double deltaTime)
{
	for (GameObject* pObject : m_gameObjects)
	{
		pObject->Update(deltaTime);
	}
	if (m_pPlayer->GetHealth() <= 0)
	{
		m_pOwner->LoadScene(CatGameStateMachine::SceneName::Lose);
	}
	// If player had more catnips than last update and boss is alive
	if (m_pPlayer->GetCatnips() > m_catnipNumber && m_pBoss != nullptr)
	{
		// Delete catnip, pop catnip in vector
		delete m_gameObjects.back();
		m_gameObjects.pop_back();
		// Set catnip number player has, and no catnip is currently spawned
		m_catnipNumber = m_pPlayer->GetCatnips();
		m_catnipSpawned = false;
		// enemyHealth -= (playerCatnips * 100)
		m_pBoss->SetHealth(m_pBoss->GetMaxHealth() - (m_catnipNumber * 100));
	}
	// If boss is already dead, or just died
	if (m_pBoss == nullptr || m_pBoss->GetHealth() <= 0)
	{
		// If boss just dies
		if (m_pBoss != nullptr)
		{
			// Play explosion sound, destroy mouse, create a particle system
			m_pSoundReferee->PlaySound("explosion", 0, 0);
			m_gameObjects.pop_back();
			ParticleSystem* pParticles = new ParticleSystem(m_pBoss->GetPosition(), 100, 100, 255, 0, 0);
			pParticles->SetParticleSpeed(100);
			pParticles->SetParticleSize(3);
			m_gameObjects.push_back(pParticles);
			// Delete boss
			delete m_pBoss;
			m_pBoss = nullptr;
		}
		// Decrement for 4 seconds, then load Win screen
		m_seconds -= deltaTime;
		if (m_seconds <= 0)
		{
			m_pOwner->LoadScene(CatGameStateMachine::SceneName::Win);
		}
	
	}
	// If there is no catnip on board and m_pBoss is still alive
	if (!m_catnipSpawned && m_pBoss != nullptr)
	{
		constexpr float kRangeLow = 0;
		constexpr float kRangeHigh = 340;
		constexpr float kRangeHighChase = 550;
		RandomNumberGenerator rng;
		Vector2 catnipPos;

		// Choose a random position for the catnip based on whether the mouse is in a certain phase or not
		if (m_pBoss->GetCurrentState() != AIStateMachine::StateName::Chase)
		{
			catnipPos = Vector2{ rng.RandomFloat(0, 750), rng.RandomFloat(kRangeLow, kRangeHigh) };
		}
		else
		{
			catnipPos = Vector2{ rng.RandomFloat(0, 750), rng.RandomFloat(kRangeLow, kRangeHighChase) };
		}

		// Create new Catnip object
		Catnip* pCatnip = new Catnip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, catnipPos);
		m_gameObjects.push_back(pCatnip);

		// Catnip now spawned
		m_catnipSpawned = true;
	}
}

/////////////////////
// Renders this scene
void StateBoss::Render(SDL_Renderer* pRenderer)
{

	// Clear
	SDL_SetRenderDrawColor(pRenderer, 128, 255, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(pRenderer);

	// Draw
	for (GameObject* pObject : m_gameObjects)
	{
		pObject->Render(pRenderer);
	}

	// Present
	SDL_RenderPresent(pRenderer);
}

///////////////////////////////////////////
// Handles given event, return true if quit
bool StateBoss::HandleEvent(SDL_Event* pEvent)
{
	switch (pEvent->type)
	{
	case SDL_WINDOWEVENT:
		if (pEvent->window.event == SDL_WINDOWEVENT_CLOSE)
			return true;
		break;
	case SDL_KEYDOWN:
	{
		switch (pEvent->key.keysym.scancode)
		{
			// Save button
		case SDL_SCANCODE_K:
			bool success = Save();
			if (success)
				std::cout << "Saved successfully." << std::endl;
			else
				std::cout << "Save failed." << std::endl;
			break;
		}
	}
	}

	return m_pPlayer->ProcessInput();
}

//////////////////////////
// Clean up and exit state
void StateBoss::Exit()
{
	for (GameObject* object : m_gameObjects)
	{
		delete object;
	}
}

/////////////////////////////////////////
// Saves this game, returns if successful
bool StateBoss::Save()
{
	constexpr int kStateBoss = 1;
	// Get data references
	SaveData* pSave = m_pOwner->GetSave();
	SaveData::Data& saveData = pSave->GetData(m_pOwner->GetIndex());

	// Update save data values
	saveData.m_playerPosition = m_pPlayer->GetPosition();
	saveData.m_health = m_pPlayer->GetHealth();
	saveData.m_bossPosition = m_pBoss->GetPosition();
	saveData.m_bossHealth = m_pBoss->GetHealth();
	saveData.m_currentScene = kStateBoss;
	saveData.m_bossIsCharging = false;

	// Store if boss charging or not
	if (m_pBoss->GetCurrentState() == AIStateMachine::StateName::Charge || m_pBoss->GetCurrentState() == AIStateMachine::StateName::Return)
		saveData.m_bossIsCharging = true;

	return pSave->Save(m_pOwner->GetIndex());
}