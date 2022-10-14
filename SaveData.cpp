#include "CatGameStateMachine.h"
#include "SaveData.h"
#include "Vector2.h"
#include <direct.h>
#include <fstream>
#include <iostream>
#include <string>

SaveData::SaveData()
    : m_isLoaded(false)
    , m_dataOne()
    , m_dataTwo()
    , m_dataThree()
{
}

SaveData::~SaveData()
{
}

////////////////////////////////////////////////////
// Attempt to save data to disk, return if succeeded
bool SaveData::Save(size_t index)
{
    // Make sure directory exists
    _mkdir(s_kDirectory); 

    // Combine directory and file name
    std::string path = s_kDirectory + std::string(s_kFileName) + std::to_string(index);

    std::ofstream file;
    // Open file
    file.open(path, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Failed to open " << path << std::endl;
        // Return failure
        return false;
    }

    // Write data to file
    file.write((char*)&m_dataArray[index], sizeof(m_dataArray[index]));

    // Close file
    file.close();
    // Return success
    return true;
}

////////////////////////////////////////////////////
// Attempt to save data to disk, return if succeeded
bool SaveData::Load(size_t index)
{
    // Combine directory and file name
    std::string path = s_kDirectory + std::string(s_kFileName) + std::to_string(index);

    std::ifstream file;
    // Open file
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Failed to open " << path << std::endl;
        // Return failure
        return false;
    }

    // Read data from file
    file.read((char*)&m_dataArray[index], sizeof(m_dataArray[index]));

    // Close file, file is loaded
    file.close();
    m_isLoaded = true;

    // Return success
    return true;
}
