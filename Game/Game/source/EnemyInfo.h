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
	// EnemyInfoのコンストラクタはEnemyDataを受け取り、EnemyBaseのコンストラクタを呼び出す
	EnemyInfo(const EnemyData& data) :EnemyBase(data) {}
};