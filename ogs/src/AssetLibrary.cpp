#include "AssetLibrary.h"

void ogs::AssetLibrary::GetMesh(std::string const& path, MeshLoadedCallback callback)
{
	if (_loaded_meshes.contains(path))
	{
		callback(_loaded_meshes[path].get());
		return;
	}

	if (_currently_loading.contains(path))
	{
		LogWarning("Loading mesh '{}' is currently being loaded.", path);
		return;
	}

	auto future = std::async(std::launch::async, 
		[path = path, &queued_meshes = _queued_meshes]() 
		-> std::unique_ptr<Wavefront_File> {
		auto obj_file = wavefront_load(path);
		++queued_meshes;
		return std::move(obj_file);
		});
	_currently_loading.insert(path);
	_mesh_queue.push_back({ std::move(future), path, callback });
}

void ogs::AssetLibrary::Maintain()
{
	auto const queue_size = _queued_meshes.load();
	for (auto i = 0; i < queue_size; ++i)
	{
		if (_mesh_queue.size() == 0)
		{
			return;
		}
		auto& item = _mesh_queue.front();
		--_queued_meshes;

		auto obj_file = std::get<std::future<std::unique_ptr<Wavefront_File>>>(item).get();
		if (!obj_file)
		{
			_currently_loading.erase(std::get<std::string>(item));
			_mesh_queue.pop_front();
			return;
		}

		_loaded_meshes.emplace(std::get<std::string>(item), std::make_unique<Mesh>(*obj_file));
		std::get<MeshLoadedCallback>(item)(_loaded_meshes[std::get<std::string>(item)].get());

		_currently_loading.erase(std::get<std::string>(item));
		_mesh_queue.pop_front();
	}
}
