#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "j1Module.h"

class j1UI : public j1Module
{
public:

	j1UI();
	~j1UI();

	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	int pLife;
};

#endif //__MODULEUI_H__