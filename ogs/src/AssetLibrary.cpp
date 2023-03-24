#include "ogspch.h"

#include "AssetLibrary.h"
#include "temp/Mesh.h"

ogs::Texture* ogs::AssetLibrary::GetTexture(std::string const& path)
{
	if (m_loadedTextures.contains(path))
	{
		return m_loadedTextures[path].get();
	}

	m_loadedTextures.emplace(path, std::make_unique<Texture>(path));
	return m_loadedTextures[path].get();
}

ogs::MeshResource* ogs::AssetLibrary::GetMesh(std::string const& path)
{
	if (m_loadedMeshes.contains(path))
	{
		return m_loadedMeshes[path].get();
	}
	auto obj_file = wavefront_load(path);
	m_loadedMeshes.emplace(path, std::make_unique<MeshResource>(*obj_file));
	return m_loadedMeshes[path].get();
}