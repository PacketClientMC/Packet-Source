#include "../../Module/ModuleManager.h"
#include "LongJump.h"

LongJump::LongJump() : IModule(0, Category::MOVEMENT, "ye very cool - packet") {
	registerBoolSetting("SlowDown", &this->slowDown, this->slowDown);
	registerBoolSetting("Damage", &this->damage, this->damage);
	//registerBoolSetting("V2", &this->newMode, this->newMode);
	registerFloatSetting("Height", &this->height, this->height, 0.2f, 5.f);
	registerFloatSetting("Speed", &this->speed, this->speed, 0.3f, 5.f);
}

const char* LongJump::getModuleName() {
	return "LongJump";
}

void LongJump::onEnable() {
	if (damage) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
	if (slowDown) {
		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Only on The Hive
			auto player = g_Data.getLocalPlayer();
			if (player->onGround) {
				*g_Data.getClientInstance()->minecraft->timer = 3.f;
			}
		}
			if (!strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Not on The Hive
				*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
	}
	if (!slowDown) {
			*g_Data.getClientInstance()->minecraft->timer = 35.f;
	}
	if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Only on The Hive
		auto player = g_Data.getLocalPlayer();
		if (player->onGround) {
			vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

			vec3_t pos;
			pos.x = 0.f + pPos.x;
			pos.y = 0.5 + pPos.y;
			pos.z = 0.f + pPos.z;

			g_Data.getLocalPlayer()->setPos(pos);
		}
	}
}

void LongJump::onTick(C_GameMode* gm) {
	if (!newMode) {
		auto timerMod = moduleMgr->getModule<Timer>();
		if (timerMod->isEnabled()) {
			timerMod->setEnabled(false);
		}
		if (slowDown) {
			*g_Data.getClientInstance()->minecraft->timer = 0.f;
		}
		auto longjumpMod = moduleMgr->getModule<LongJump>();
		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Only on The Hive
			auto player = g_Data.getLocalPlayer();
			if (!player->onGround) {
				if (longjumpMod->isEnabled()) {
					longjumpMod->setEnabled(false);
				}
			}
		}
	}
}

void LongJump::onMove(C_MoveInputHandler* input) {
	if (!newMode) {
		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Only on The Hive
			testTimer++;
			if (testTimer == 8) {
				*g_Data.getClientInstance()->minecraft->timer = 3.f;
			}
			if (testTimer >= 9) {
				*g_Data.getClientInstance()->minecraft->timer = 1.f;
				testTimer = 0;
			}
			if (testTimer <= 7 || testTimer >= 9) {
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
			}
		}
		if (slowDown) {
			testTimer++;
			if (testTimer == 8) {
				*g_Data.getClientInstance()->minecraft->timer = 3.f;
			}
			if (testTimer >= 9) {
				*g_Data.getClientInstance()->minecraft->timer = 1.f;
				testTimer = 0;
			}
			if (testTimer <= 7 || testTimer >= 9) {
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
			}
		}
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (player->onGround && pressed)
			player->jumpFromGround();

		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = height;
		player->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
		auto longjumpMod = moduleMgr->getModule<LongJump>();
		if (longjumpMod->isEnabled()) {
			longjumpMod->setEnabled(false);
		}
		auto glideMod = moduleMgr->getModule<Glide>();
		if (glideMod->isEnabled()) {
			glideMod->setEnabled(false);
		}
		auto bhopMod = moduleMgr->getModule<Bhop>();
		if (bhopMod->isEnabled()) {
			bhopMod->setEnabled(false);
		}
	}
}

void LongJump::onDisable() {
	auto player = g_Data.getLocalPlayer();
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (newMode) {
		if (!player->onGround) {
			player->velocity.x = 0.f;
			player->velocity.z = 0.f;
		}
	}
}