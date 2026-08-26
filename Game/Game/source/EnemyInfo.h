#pragma once
#include "EnemyBase.h"
#include "Resource.h"

namespace EnemyType 
{
	extern const EnemyData Enemy1;
	extern const EnemyData Enemy2;
}

class EnemyInfo : public EnemyBase
{
public:
	EnemyInfo(const EnemyData& data) :EnemyBase(data) {}
};