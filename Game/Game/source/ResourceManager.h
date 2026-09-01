#pragma once
#include "appframe.h"
#include <string>
#include <unordered_map>

class ResourceManager
{
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager() = default;
	~ResourceManager() = default;

	void LoadResource();

	int GetImage(const std::string& key) const;
	int GetModel(const std::string& key) const;

private:
	std::unordered_map<std::string, int> _images;
	std::unordered_map<std::string, int> _models;
};

