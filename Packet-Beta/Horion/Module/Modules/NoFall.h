#pragma once

#include "Module.h"

class NoFall : public IModule {
public:
	C_MoveInputHandler* inputHandler = nullptr;
	
				SettingEnum mode = this;

	NoFall();
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
};