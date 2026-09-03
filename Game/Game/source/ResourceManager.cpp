#include "ResourceManager.h"
#include "Resource.h"

void ResourceManager::LoadResource()
{
	_images[image::Enemy1] = LoadGraph(image::Enemy1);
	_models[mv1::SkySphere] = MV1LoadModel(mv1::SkySphere);
	_models[mv1::Ground] = MV1LoadModel(mv1::Ground);
	_models[mv1::Dungeon] = MV1LoadModel(mv1::Dungeon);
	_models[mv1::Chara] = MV1LoadModel(mv1::Chara);
	_models[mv1::Map] = MV1LoadModel(mv1::Map);
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