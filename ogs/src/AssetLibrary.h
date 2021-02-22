#pragma once

#include <unordered_map>
#include <string>
#include <atomic>
#include <set>
#include <future>
#include <memory>

#include <functional>
#include "ts_queue.h"
#include "temp/wavefront.h"
#include "temp/Mesh.h"

namespace ogs
{
class AssetLibrary {
	using MeshLoadedCallback = std::function<void(Mesh*)>;
	friend class GLContext;
public:

	void GetMesh(std::string const& path, MeshLoadedCallback on_loaded);

private:
	AssetLibrary() = default;
	void Maintain();
	std::unordered_map<std::string, std::unique_ptr<Mesh>> _loaded_meshes;
	std::set<std::string> _currently_loading;
	containers::ts_queue<std::tuple<std::future<std::unique_ptr<Wavefront_File>>, std::string, MeshLoadedCallback>> _mesh_queue;
	std::atomic_int _queued_meshes = 0;
};
}