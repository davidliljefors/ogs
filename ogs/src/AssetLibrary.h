#pragma once

#include <unordered_map>
#include <string>
#include <atomic>
#include <set>
#include <future>
#include <memory>
#include <deque>

#include <functional>
#include "temp/wavefront.h"
#include "temp/Mesh.h"
#include "ts_queue.h"

namespace ogs
{
class AssetLibrary {
	using MeshLoadedCallback = std::function<void(Mesh*)>;

	struct MeshLoadTask
	{
		std::string Name;
		std::future<std::unique_ptr<Wavefront_File>> File;
		std::vector<MeshLoadedCallback> OnLoaded;
	};

	friend class GLContext;
public:

	void GetMesh(std::string const& path, MeshLoadedCallback&& on_loaded);

private:
	AssetLibrary() = default;
	void Maintain();
	std::unordered_map<std::string, std::unique_ptr<Mesh>> _loaded_meshes;
	std::set<std::string> _currently_loading;
	std::vector<MeshLoadTask> _mesh_queue;
	containers::ts_queue<std::string> _finished_tasks;
};
}