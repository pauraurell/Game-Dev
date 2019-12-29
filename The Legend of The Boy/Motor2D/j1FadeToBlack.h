#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlackVisualEffect(float time = 2.0f);
	bool FadeToBlack(j1Module* SceneIn, j1Module* SceneOut, float time = 1.0f);
	void ResetFade();

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;
	Uint32 start_time;
	Uint32 total_time;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_H__