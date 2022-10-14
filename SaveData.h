#pragma once

class CatGameStateMachine;
struct Vector2;

class SaveData
{
public:
	// Game specific data to store
	struct Data
	{
		Vector2 m_playerPosition;
		Vector2 m_bossPosition;
		unsigned int m_seed;
		float m_health;
		float m_bossHealth;
		int m_currentScene;
		bool m_bossIsCharging;
	};
private:
	// Save data relative location
	static constexpr char s_kDirectory[] = "saves\\";

	// Save data full path with file name
	static constexpr char s_kFileName[] = "savedata.save";

	// If this save data has been loaded this session
	bool m_isLoaded;

	// Data save slot 1
	Data m_dataOne;

	// Data save slot 2
	Data m_dataTwo;

	// Data save slot 3
	Data m_dataThree;

	// Array of actual written data
	Data m_dataArray[3] = { m_dataOne, m_dataTwo, m_dataThree };

public:
	SaveData();
	~SaveData();

	// Attempt to save data to disk, return if succeeded
	bool Save(size_t index);

	// Attempt to load data to disk, return if succeeded
	bool Load(size_t index);

	// Get whether this save data has been loaded this session, can determine if save data is unavailable
	bool GetIsLoaded() { return m_isLoaded; }

	// Get a reference to the game state data
	Data& GetData(size_t index) { return m_dataArray[index]; }

};

