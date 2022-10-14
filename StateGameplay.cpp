#include "Background.h"
#include "Catnip.h"
#include "CollisionReferee.h"
#include "Couch.h"
#include "MysteryNip.h"
#include "Enemy.h"
#include "GameObject.h"
#include "ImageObject.h"
#include "StateGameplay.h"
#include "WinZone.h"
#include "RandomNumberGenerator.h"
#include "Vector2.h"

StateGameplay::StateGameplay(CatGameStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pPlayer(nullptr)
	, m_pEnemy(nullptr)
	, m_pRenderer(pOwner->GetGame()->GetRenderer())
	, m_pCollisionReferee(pOwner->GetGame()->GetReferee())
	, m_pSoundReferee(pOwner->GetGame()->GetSoundReferee())
	, m_pImageHandler(pOwner->GetGame()->GetImageHandler())
	, m_catnipNumber(0)
	, m_gameObjects()
	, m_seed()
{
}

StateGameplay::~StateGameplay()
{
}

/////////////////////////
// Initializes this scene
void StateGameplay::Enter()
{
	// Check if there is saved data.
	SaveData* pSave = m_pOwner->GetSave();
	SaveData::Data& data = pSave->GetData(m_pOwner->GetIndex());

	// If data not loaded this sesson, set default values
	if (pSave->GetIsLoaded() == false)
	{
		constexpr int kStateGameplay = 0;

		data.m_playerPosition = Vector2{ 10.f, 20.f };
		data.m_bossPosition = Vector2{ 400.f, 400.f };
		data.m_bossHealth = 1000;
		data.m_health = 500;
		data.m_currentScene = kStateGameplay;
		RandomNumberGenerator rng;
		data.m_seed = rng.GetSeed();
	}

	// Allocate ImageHandler
	ImageHandler handler;

	constexpr int kArraySize = 9;

	// Array of possible spawn positions
	Vector2 spawnCell[kArraySize] = { Vector2{250, 50}, Vector2{450, 50}, Vector2{650, 50},
	Vector2{250, 200}, Vector2{450, 200}, Vector2{650, 200},
	Vector2{100, 500}, Vector2{300, 500}, Vector2{500, 500} };


	// Mix up spawn cells
	Vector2 mixedCells[kArraySize]{};
	// Use a random number generator to pix a cell to add to the array
	RandomNumberGenerator rng(data.m_seed);

	// Store seed for saving
	m_seed = data.m_seed;

	for (size_t i = 0; i < kArraySize; ++i)
	{
		// Choose random index to put into the shuffled array
		int index = rng.RandomInt(0, (kArraySize - 1));
		// Check array so far, see if cell is occupied
		bool cellOccupied = false;
		for (size_t j = 0; j < i; ++j)
		{
			// If cell occupied, don't place item
			if (mixedCells[j].m_x == spawnCell[index].m_x && mixedCells[j].m_y == spawnCell[index].m_y)
			{
				cellOccupied = true;
			}
		}
		// If cell isn't occupied, drop it in/ else reverse index
		if(!cellOccupied)
		{
			mixedCells[i] = spawnCell[index];
		}
		else
		{
			i -= 1;
		}
	}

	// Allocate background tile
	Background* pBackground = new Background(m_pRenderer, m_pImageHandler->GetImageSurface("background"), 800, 600, 100, 100);

	m_gameObjects.push_back(pBackground);

	// Allocate objects
	WinZone* pWinZone = new WinZone(SDL_FRect{ 750, 550, 50, 50 }, m_pRenderer, m_pImageHandler->GetImageSurface("winFlag"), m_pCollisionReferee);

	m_gameObjects.push_back(pWinZone);

	Couch* pCouch = new Couch(m_pRenderer, m_pImageHandler->GetImageSurface("couch"), m_pCollisionReferee, 150, 100, mixedCells[0]);
	Couch* pCouch2 = new Couch(m_pRenderer, m_pImageHandler->GetImageSurface("couch"), m_pCollisionReferee, 150, 100, mixedCells[1]);
	Couch* pCouch3 = new Couch(m_pRenderer, m_pImageHandler->GetImageSurface("couch"), m_pCollisionReferee, 150, 100, mixedCells[2]);
	Couch* pCouch4 = new Couch(m_pRenderer, m_pImageHandler->GetImageSurface("couch"), m_pCollisionReferee, 150, 100, mixedCells[3]);

	m_gameObjects.push_back(pCouch);
	m_gameObjects.push_back(pCouch2);
	m_gameObjects.push_back(pCouch3);
	m_gameObjects.push_back(pCouch4);

	Catnip* pCatnip = new Catnip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, mixedCells[4]);
	Catnip* pCatnip2 = new Catnip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, mixedCells[5]);

	m_gameObjects.push_back(pCatnip);
	m_gameObjects.push_back(pCatnip2);

	MysteryNip* pMysteryNip = new MysteryNip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, mixedCells[6]);
	MysteryNip* pMysteryNip2 = new MysteryNip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, mixedCells[7]);
	MysteryNip* pMysteryNip3 = new MysteryNip(m_pRenderer, m_pImageHandler->GetImageSurface("catnip"), 30, 30, 12, 4, m_pCollisionReferee, mixedCells[8]);

	m_gameObjects.push_back(pMysteryNip);
	m_gameObjects.push_back(pMysteryNip2);
	m_gameObjects.push_back(pMysteryNip3);

	// Create new player object
	Player* pPlayer = new Player(m_pRenderer, m_pImageHandler->GetImageSurface("cat"), m_pImageHandler->GetImageSurface("healthBar"), m_pImageHandler->GetImageSurface("background"), 
								data.m_playerPosition.m_x, data.m_playerPosition.m_y, m_pCollisionReferee, m_pSoundReferee);
	m_pPlayer = pPlayer;
	m_gameObjects.push_back(m_pPlayer);

	// Set player health to saved health
	m_pPlayer->SetHealth(data.m_health);


	// Create 2 mice
	Enemy* pMouse = new Enemy(m_pRenderer, m_pImageHandler->GetImageSurface("mouse"), m_pImageHandler->GetImageSurface("healthBar"), 400, 400, m_pCollisionReferee, pPlayer);
	m_gameObjects.push_back(pMouse);

	m_pEnemy = pMouse;
}

/////////////////////
// Updates this scene
void StateGameplay::Update(double deltaTime)
{
	for (GameObject* pObject : m_gameObjects)
	{
		pObject->Update(deltaTime);
	}
	if (m_pPlayer->GetHealth() <= 0)
	{
		m_pOwner->LoadScene(CatGameStateMachine::SceneName::Lose);
	}
	if (m_pPlayer->GetWinCondition())
	{
		m_pOwner->LoadScene(CatGameStateMachine::SceneName::Boss);
	}
	if (m_pPlayer->GetCatnips() > m_catnipNumber && m_pEnemy != nullptr)
	{
		m_catnipNumber = m_pPlayer->GetCatnips();
		// enemyHealth -= (playerCatnips * 100)
		m_pEnemy->SetHealth(m_pEnemy->GetMaxHealth() - (m_catnipNumber * 100));
		if (m_pEnemy->GetHealth() <= 0)
		{
			m_gameObjects.pop_back();
			delete m_pEnemy;
			m_pEnemy = nullptr;
		}
	}
}

/////////////////////
// Renders this scene
void StateGameplay::Render(SDL_Renderer* pRenderer)
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
bool StateGameplay::HandleEvent(SDL_Event* pEvent)
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
void StateGameplay::Exit()
{
	for (GameObject* object : m_gameObjects)
	{
		delete object;
	}
}

/////////////////////////////////////////
// Saves this game, returns if successful
bool StateGameplay::Save()
{
	constexpr int kStateGameplay = 0;
	// Get data references
	SaveData* pSave = m_pOwner->GetSave();
	SaveData::Data& saveData = pSave->GetData(m_pOwner->GetIndex());

	// Update save data values
	saveData.m_playerPosition = m_pPlayer->GetPosition();
	saveData.m_health = m_pPlayer->GetHealth();
	saveData.m_seed = m_seed;
	saveData.m_currentScene = kStateGameplay;

	return pSave->Save(m_pOwner->GetIndex());
}
