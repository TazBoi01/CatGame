#include "SDLGame.h"
#include <chrono>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

// Initialize all game pointers to nullptr
SDLGame::SDLGame()
    : m_pWindow(nullptr)
    , m_pRenderer(nullptr)
    , m_pStateMachine(nullptr)
    , m_pSoundReferee(nullptr)
    , m_pSoundGameMusic(nullptr)
    , m_pImageHandler(nullptr)
    , m_doQuit(false)
{
}

SDLGame::~SDLGame()
{
}

/////////////////////////////////////////////////////////////////////////////////
// Attempt to initialize SDL aspects of game, return an error code (0 if success)
int SDLGame::InitSDL(GameStateMachine* pStateMachine)
{
    int errorCode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    if (errorCode != 0)
    {

        std::cout << "Failed to initialize SDL." << std::endl;

        return errorCode;

    }

    std::cout << "Successfully initialized SDL!" << std::endl;


    // SDL Window Init
    m_pWindow = SDL_CreateWindow("New Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    // Window Creation Error
    if (m_pWindow == nullptr)
    {
        std::cout << "Failed to create window: " << SDL_GetError();
        SDL_Quit();
        return -1;
    }

    // Create SDL Renderer
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

    // Render Creation Error
    if (m_pRenderer == nullptr)
    {
        std::cout << "Failed to create renderer: " << SDL_GetError();
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_ttf.
    errorCode = TTF_Init();
    if (errorCode != 0)
    {
        std::cout << "TTF_Init() failed. Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return -1;
    }

    // Init SDL Mixer
    errorCode = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    if (errorCode != 0)
    {
        std::cout << "Mix_OpenAudio() failed. Error: " << Mix_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return -1;
    }

    // Init MP# for music
    int audioFlags = MIX_INIT_MP3;
    errorCode = Mix_Init(audioFlags);
    if (errorCode != audioFlags)
    {
        std::cout << "Mixer_Init() failed. Error: " << Mix_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return -1;
    }

    // Create sound referee and game music, store them. Then play music
    m_pSoundReferee = new SoundReferee;
    m_pSoundGameMusic = new SoundGameMusic{ m_pSoundReferee };
    m_pSoundGameMusic->PlayMusic();

    // Create image handler
    m_pImageHandler = new ImageHandler;

    // Set chunk volumes
    Mix_VolumeChunk(m_pSoundReferee->GetSound("walk"), 5);
    Mix_VolumeChunk(m_pSoundReferee->GetSound("win"), 128);
    Mix_VolumeChunk(m_pSoundReferee->GetSound("click"), 128);

    // Init state machine, return -1 if fail
    if (pStateMachine != nullptr && pStateMachine->Init())
    {
        m_pStateMachine = pStateMachine;
    }
    else
    {
        DeinitSDL();
        return -1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
// Runs the game loop. Won't exit until a quit event has been detected
void SDLGame::RunGameLoop()
{
    // Allocate quit flag
    bool quit = false;

    // Grab a time to compare to current time in loop
    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    while (!quit)
    {
        // Get current time
        auto thisFrameTime = std::chrono::high_resolution_clock::now();

        // Find duration of frame by difference of 'thisFrame' and 'lastFrame'
        std::chrono::duration<double> lastFrameDuration = thisFrameTime - lastFrameTime;

        // Record delta time in double
        double deltaTime = lastFrameDuration.count();

        // Update last frame to this frame
        lastFrameTime = thisFrameTime;

        // Update game state with delta time, and display output before restarting loop
        quit = ProcessEvents();
        UpdateGameState(deltaTime);
        m_pSoundGameMusic->UpdateMusic();
        DisplayOutput();

        if (m_doQuit)
            quit = true;
    }
}

////////////////////////////////////
// Deinitializes SDL aspects of game
void SDLGame::DeinitSDL()
{
    // Delete Game Pointers
    delete m_pSoundGameMusic;
    delete m_pSoundReferee;
    delete m_pImageHandler;
    // Close audio
    Mix_CloseAudio();
    // If allocated, cleanup state machine
    if (m_pStateMachine != nullptr)
        m_pStateMachine->Cleanup();
    // Destroy SDL Renderer
    SDL_DestroyRenderer(m_pRenderer);
    // SDL Window Cleanup
    SDL_DestroyWindow(m_pWindow);
    // SDL Overall Deinit
    SDL_Quit();
}

////////////
// Quit game
void SDLGame::Quit()
{
    m_doQuit = true;
}

/////////////////////////////////////////////////
// Processes game events, bool for if exit or not
bool SDLGame::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        bool quit = m_pStateMachine->HandleEvent(&event);
        if (quit)
            return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Updates the state of the game simulation based on the passage of time and any events that were detected
void SDLGame::UpdateGameState(double deltaTime)
{
    m_pStateMachine->UpdateCurrentState(deltaTime);
}

/////////////////////////////
// Draws this programs output
void SDLGame::DisplayOutput()
{
    m_pStateMachine->RenderCurrentState(m_pRenderer);
}