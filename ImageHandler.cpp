#include "ImageHandler.h"
#include <SDL_image.h>
ImageHandler::ImageHandler()
{
	// Load all images into the surface map
	AddImage("cat", "assets/catSprite.png");
	AddImage("background", "assets/carpet.jpg");
	AddImage("mouse", "assets/mouse.png");
	AddImage("couch", "assets/couch.png");
	AddImage("catnip", "assets/catnip.jpg");
	AddImage("menu", "assets/menu.png");
	AddImage("lose", "assets/lose.png");
	AddImage("win", "assets/win.png");
	AddImage("button", "assets/button.jpg");
	AddImage("buttonHighlighted", "assets/buttonHighlighted.jpg");
	AddImage("boss", "assets/boss.png");
	AddImage("healthBar", "assets/healthBar.jpg");
	AddImage("winFlag", "assets/winFlag.png");
}

ImageHandler::~ImageHandler()
{
	for (auto& iter : m_ImageSurfaces)
	{
		SDL_FreeSurface(iter.second);
	}
}

////////////////////////////
// Add image to surfaces map
void ImageHandler::AddImage(std::string name, const char* path)
{
	// Load an image with given path, place it in the map
	SDL_Surface* pImageObject = IMG_Load(path);
	m_ImageSurfaces[name] = pImageObject;
}

void ImageHandler::Update(double)
{
}

void ImageHandler::Render(SDL_Renderer*)
{
}
