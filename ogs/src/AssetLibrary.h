#pragma once

#include <string>
#include <unordered_map>

#include "Texture.h"


namespace ogs {

class MeshResource;

class AssetLibrary
{
	friend class GLContext;

public:
	Texture* GetTexture(std::string const& path);
	MeshResource* GetMesh(std::string const& path);


private:
	AssetLibrary() = default;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_loadedTextures;
	std::unordered_map<std::string, std::unique_ptr<MeshResource>> m_loadedMeshes;
};

}