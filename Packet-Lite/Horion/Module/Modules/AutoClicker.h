#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class AutoClicker : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	bool weapons = false;
	bool breakBlocks = false;
	bool rightclick = false;
	bool hold = true;

public:
	AutoClicker();
	~AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
