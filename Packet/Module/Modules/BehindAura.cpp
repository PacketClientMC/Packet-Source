#include "BehindAura.h"

BehindAura::BehindAura() : IModule(0, Category::COMBAT, "A combat aura used for PvP or HvH, you will always stay behind the enemy.") {
	//registerBoolSetting("Mobs", &isMobAura, isMobAura);
	registerFloatSetting("LerpSpeed", &lerpSpeed, lerpSpeed, 1, 50);
	registerFloatSetting("Distance", &behindDist, behindDist, 0.5f, 5.f);
	registerFloatSetting("Range", &range, range, 0.5f, 9.f);
	registerBoolSetting("UseLerpTo", &useLerp, useLerp);
	registerBoolSetting("UseSetPos", &usePos, usePos);
	registerBoolSetting("OnAttack", &useAttack, useAttack);
	registerBoolSetting("OnTick", &useTick, useTick);

	registerBoolSetting("BasicYawCheck", &basicCheck, basicCheck);
	registerBoolSetting("CalcYawCheck", &calcYawCheck, calcYawCheck);
}

BehindAura::~BehindAura() {
}

const char* BehindAura::getModuleName() {
	return ("BehindAura");
}

static std::vector<C_Entity*> targetListBA;

void findEntityBA(C_Entity* currentEntity, bool isRegularEntity) {
	static auto bauraMod = moduleMgr->getModule<BehindAura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (bauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < bauraMod->range) {
		targetListBA.push_back(currentEntity);
	}
}

void BehindAura::onTick(C_GameMode* gm) {
	targetListBA.clear();
	g_Data.forEachEntity(findEntityBA);

	C_LocalPlayer* player = g_Data.getLocalPlayer();
	vec3_t pos = *targetListBA[0]->getPos();
	float yaw = targetListBA[0]->yaw;

	if (basicCheck && useTick && !targetListBA.empty()) {
		if (useLerp) {
			if (yaw >= -45 && yaw <= 45) {
				player->lerpTo(vec3_t(pos.x, pos.y + 1.62f, pos.z - behindDist), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
			if (yaw >= -135 && yaw <= -44) {
				player->lerpTo(vec3_t(pos.x - behindDist, pos.y + 1.62f, pos.z), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
			if (yaw >= 131 && yaw >= -134 && yaw != -135) {
				player->lerpTo(vec3_t(pos.x, pos.y + 1.62f, pos.z + behindDist), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
			if (yaw >= 45 && yaw <= 130) {
				player->lerpTo(vec3_t(pos.x + behindDist, pos.y + 1.62f, pos.z), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
		} else if (usePos) {
			if (yaw >= -45 && yaw <= 45) {
				player->setPos(vec3_t(pos.x, pos.y + 1.62f, pos.z - behindDist));
			}
			if (yaw >= -135 && yaw <= -44) {
				player->setPos(vec3_t(pos.x - behindDist, pos.y + 1.62f, pos.z));
			}
			if (yaw >= 131 && yaw >= -134 && yaw != -135) {
				player->setPos(vec3_t(pos.x, pos.y + 1.62f, pos.z + behindDist));
			}
			if (yaw >= 45 && yaw <= 130) {
				player->setPos(vec3_t(pos.x + behindDist, pos.y + 1.62f, pos.z));
			}
		}
	} else if (calcYawCheck && useTick && !targetListBA.empty()) {
		float theirYaw = (yaw - 90) * (PI / 180);
		float length = behindDist;

		float gotoX = -sin(theirYaw) * length;
		float gotoZ = cos(theirYaw) * length;

		gm->player->setPos(pos.add(vec3_t(gotoX, 0.5f, gotoZ)));
	}
}

void BehindAura::onEnable() {
}

void BehindAura::onAttack(C_Entity* attackedEnt) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	vec3_t pos = *attackedEnt->getPos();
	float yaw = attackedEnt->yaw;
	float dist2 = player->getPos()->dist(pos);
	if (basicCheck && useAttack) {
		if (useLerp) {
			if (yaw >= -45 && yaw <= 45) {
				player->lerpTo(vec3_t(pos.x, pos.y + 1.62f, pos.z - behindDist), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
			if (yaw >= -135 && yaw <= -44) {
				player->lerpTo(vec3_t(pos.x - behindDist, pos.y + 1.62f, pos.z), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
			if (yaw >= 131 && yaw >= -134 && yaw != -135) {
				player->lerpTo(vec3_t(pos.x, pos.y + 1.62f, pos.z + behindDist), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
			if (yaw >= 45 && yaw <= 130) {
				player->lerpTo(vec3_t(pos.x + behindDist, pos.y + 1.62f, pos.z), vec2_t(1, 1), (int)fmax((int)lerpSpeed * 0.1, 2.f));
			}
		} else if (usePos) {
			if (yaw >= -45 && yaw <= 45) {
				player->setPos(vec3_t(pos.x, pos.y + 1.62f, pos.z - behindDist));
			}
			if (yaw >= -135 && yaw <= -44) {
				player->setPos(vec3_t(pos.x - behindDist, pos.y + 1.62f, pos.z));
			}
			if (yaw >= 131 && yaw >= -134 && yaw != -135) {
				player->setPos(vec3_t(pos.x, pos.y + 1.62f, pos.z + behindDist));
			}
			if (yaw >= 45 && yaw <= 130) {
				player->setPos(vec3_t(pos.x + behindDist, pos.y + 1.62f, pos.z));
			}
		}
	}
}