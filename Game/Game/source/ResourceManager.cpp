#include "ResourceManager.h"
#include "Resource.h"

void ResourceManager::LoadResource()
{
	OutputDebugString("ロード開始\n");
	_images[image::Enemy1] = LoadGraph(image::Enemy1);
	OutputDebugString("Enemy1 OK\n");
	_models[mv1::SkySphere] = MV1LoadModel(mv1::SkySphere);
	OutputDebugString("SkySphere OK\n");
	_models[mv1::Ground] = MV1LoadModel(mv1::Ground);
	OutputDebugString("Ground OK\n");
	_models[mv1::Dungeon] = MV1LoadModel(mv1::Dungeon);
	OutputDebugString("Dungeon OK\n");
	_models[mv1::Chara] = MV1LoadModel(mv1::Chara);
	OutputDebugString("Chara OK\n");
	_models[mv1::Map] = MV1LoadModel(mv1::Map);
	OutputDebugString("Map OK\n");
}

int ResourceManager::GetImage(const std::string& key) const
{
	auto it = _images.find(key);
	return (it != _images.end()) ? it->second : -1;
}

int ResourceManager::GetModel(const std::string& key) const
{
	auto it = _models.find(key);
	return (it != _models.end()) ? it->second : -1;
}