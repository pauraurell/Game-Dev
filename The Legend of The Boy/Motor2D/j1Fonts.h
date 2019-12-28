#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"
#include "SDL\include\SDL_pixels.h"
#include "p2List.h"

#define TRI_FONT "fonts/VCR_OSD_MONO.ttf"

struct SDL_Texture;
struct _TTF_Font;

class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text 
	SDL_Texture* Print(const char* text, SDL_Color color = { 15, 15, 15, 255 }, _TTF_Font* font = NULL, int size = 20);
	SDL_Texture* PrintTextBox(const char* text, SDL_Color color, _TTF_Font* font, Uint32 box_width, int size = 20);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	p2List<_TTF_Font*>	fonts;
	_TTF_Font*			default = nullptr;

private:
	const char* path = nullptr;
};


#endif // __j1FONTS_H__