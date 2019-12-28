#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read();
	float ReadSec();
	void Stop();
	void Restart();

private:
	uint32	started_at;
	uint32	stopped_at;
	uint32	skipped;
	uint32	skippedBefore;
	bool	running;
};

#endif //__j1TIMER_H__
