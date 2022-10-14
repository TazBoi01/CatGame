#pragma once
#include "GameObject.h"
#include <string>
#include <map>

class ImageHandler :
	public GameObject
{
protected:
	// Image map containing pointer to image surfaces
	std::map<std::string, SDL_Surface*> m_ImageSurfaces;
public:
	// Constructor/Destructor
	ImageHandler();
	~ImageHandler();

	// Add image to image map
	void AddImage(std::string name, const char* path);

	// Getter to return image surface from map at index
	SDL_Surface* GetImageSurface(std::string imageName) { return m_ImageSurfaces[imageName]; }

	// Inherited via GameObject
	virtual void Update(double deltaTime) override;
	virtual void Render(SDL_Renderer* pRenderer) override;
};

