#pragma once
#include "GameObject.h"
#include <string>
#include <map>

class AnimationComponent
{
private:
	// Animation sequence to store name and frames
	struct AnimationSequence
	{
	public:
		std::string m_name;
		int m_firstFrame = 0;
		int m_lastFrame = 0;
	};

private:
	// Animation frames per second
	const double m_kAnimationFrameRate;

	// How many frames horizontally in animation
	int m_spriteSheetColumns;

	// Texture for entire sprite sheet
	SDL_Texture* m_pSpriteSheetTexture;

	// Rect for object position and size
	SDL_FRect m_transform;

	// Current frame posititon and size 
	SDL_FRect m_currentFrameTransform;

	// Index for current frame
	int m_currentFrameNumber;

	// Remaining time until next frame in animation
	double m_nextFrameTime;

	// Name of current animation sequence
	std::string m_currentSequenceName;

	// Map holding all animations for object
	std::map<std::string, AnimationSequence> m_allAnimations;

public:
	// Constructor and destructor
	AnimationComponent( double frameRate, float frameWidth, float frameHeight, 
		int frameCount, SDL_Renderer* pRenderer, SDL_Surface* pImageSurface);
	~AnimationComponent();

	// Add a new animation sequence to the map of animations
	void AddAnimationSequence(std::string name, int firstFrame, int lastFrame);
	// Play selected animation
	void PlayAnimation(std::string sequenceName);

	// Set objects x position
	void SetTransformX(float x) { m_transform.x = x; }
	// Set objects Y position
	void SetTransformY(float y) { m_transform.y = y; }

	// Getter for transform
	SDL_FRect GetTransform() { return m_transform; }
	// Update animation frame
	void Update(double deltaTime);
	// Render current animation frame
	void Render(SDL_Renderer* pRenderer);

private:
	// Reset time to next frame based on frame rate
	void ResetFrameTime();
	// Updates the position of the current frame transform on the sprite sheet
	void UpdateSourceTransform();
};

