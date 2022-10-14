#include "AnimationComponent.h"
#include <iostream>

AnimationComponent::AnimationComponent(double frameRate, float frameWidth, float frameHeight, int frameCount, SDL_Renderer* pRenderer, SDL_Surface* pImageSurface)
	:m_kAnimationFrameRate(frameRate)
	,m_pSpriteSheetTexture(nullptr)
	,m_transform{0, 0, frameWidth, frameHeight}
	,m_currentFrameTransform(m_transform)
	,m_currentFrameNumber(-1)
	,m_nextFrameTime(0)
{
	// Gets amount of frames
	m_spriteSheetColumns = pImageSurface->w / (int)frameWidth;

	// Get amount of rows and check to see if the sprite and the frame count match up
	int rows = pImageSurface->h / (int)frameHeight;
	if (rows * m_spriteSheetColumns < frameCount)
	{
		std::cout << "Error: sprite too small" << std::endl;
		SDL_FreeSurface(pImageSurface);
		return;
	}


	// Set the object position and dimensions
	m_transform.x = 0;
	m_transform.y = 0;
	m_transform.w = frameWidth;
	m_transform.h = frameHeight;

	// Copy over tranasform to source rectangle
	m_currentFrameTransform = m_transform;

	// Create sprite texture from the loaded image surface, then free surface
	m_pSpriteSheetTexture = SDL_CreateTextureFromSurface(pRenderer, pImageSurface);
}

/////////////
// Destructor
AnimationComponent::~AnimationComponent()
{
	SDL_DestroyTexture(m_pSpriteSheetTexture);
}

///////////////////////////////////////////
// Adds animation sequence to animation map
void AnimationComponent::AddAnimationSequence(std::string name, int firstFrame, int lastFrame)
{
	m_allAnimations[name] = AnimationSequence{ name, firstFrame, lastFrame };
}

/////////////////////////////
// Play an animation sequence
void AnimationComponent::PlayAnimation(std::string sequenceName)
{
	// If the sequence is already playing or not added, do nothing
	if (sequenceName == m_currentSequenceName || m_allAnimations.find(sequenceName) == m_allAnimations.end())
	{
		return;
	}

	// Find the named sequence and set the current frame to its first frame
	AnimationSequence& newSequence = m_allAnimations[sequenceName];
	m_currentFrameNumber = newSequence.m_firstFrame;

	// Update the current sequence name, frame time, and source transform rect
	m_currentSequenceName = sequenceName;
	ResetFrameTime();
	UpdateSourceTransform();
}

//////////////////////////////
// Update the animations frame
void AnimationComponent::Update(double deltaTime)
{
	// If current frame number is initial value, nothing is being played and do nothing
	if (m_currentFrameNumber == -1)
	{
		return;
	}

	// Find the current sequence in the animation map
	AnimationSequence& currentSequence = m_allAnimations[m_currentSequenceName];
	
	// Allocate a dirty frame flag and set it to false
	bool frameIsDirty = false;

	// As long as delta time is over time until next frame, consume current frame
	while (deltaTime > m_nextFrameTime)
	{
		// Mark frame as dirty
		frameIsDirty = true;

		// Subtract time until next frame from deltaTime and continue, reset frame
		deltaTime -= m_nextFrameTime;
		ResetFrameTime();

		// If we are at the end of the animation, reset to first frame
		if (currentSequence.m_lastFrame == m_currentFrameNumber)
		{
			m_currentFrameNumber = currentSequence.m_firstFrame;
		}
		// If not increment to next frame index
		else
		{
			++m_currentFrameNumber;
		}
	}

	// Now that delta time is less than next frame time, subtract delta time from m_nextFrameTime
	m_nextFrameTime -= deltaTime;

	// Update source transform rect if frame is dirty
	if (frameIsDirty)
	{
		UpdateSourceTransform();
	}
}

////////////////////////////////////
// Render current frame of animation
void AnimationComponent::Render(SDL_Renderer* pRenderer)
{
	SDL_Rect tempRect{ (int)m_currentFrameTransform.x, (int)m_currentFrameTransform.y, (int)m_currentFrameTransform.w, (int)m_currentFrameTransform.h };
	SDL_RenderCopyF(pRenderer, m_pSpriteSheetTexture, &tempRect, &m_transform);
}

///////////////////////////////////////////////
// Reset time to next frame based on frame rate
void AnimationComponent::ResetFrameTime()
{
	m_nextFrameTime = 1. / m_kAnimationFrameRate;
}

//////////////////////////////////////////////////////////////////////////////
// Update source transform rect x and y position based on current frame number
void AnimationComponent::UpdateSourceTransform()
{
	int frameX = m_currentFrameNumber % m_spriteSheetColumns;
	int frameY = m_currentFrameNumber / m_spriteSheetColumns;
	m_currentFrameTransform.x = frameX * m_currentFrameTransform.w;
	m_currentFrameTransform.y = frameY * m_currentFrameTransform.h;
}