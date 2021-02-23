#include "AssetLibrary.h"

void ogs::AssetLibrary::GetMesh(std::string const& path, MeshLoadedCallback&& callback)
{
	if (_loaded_meshes.contains(path))
	{
		callback(_loaded_meshes[path].get());
		return;
	}

	if (_currently_loading.contains(path))
	{
		auto task = std::find_if(_mesh_queue.begin(), _mesh_queue.end(), [&path = path](MeshLoadTask const& task)
			{
				return task.Name == path;
			});
		if (task != _mesh_queue.end())
		{
			task->OnLoaded.push_back(std::move(callback));
		}
		LogHint("Loading mesh '{}' is currently being loaded.", path);
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

	std::vector<MeshLoadedCallback> callbacks;
	callbacks.push_back(std::move(callback));
	_mesh_queue.emplace_back(path, std::move(future), std::move(callbacks), false);
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
		auto& mesh_load_task = _mesh_queue.front();
		--_queued_meshes;

		auto obj_file = std::move(mesh_load_task.File.get());
		if (!obj_file)
		{
			_currently_loading.erase(mesh_load_task.Name);
			_mesh_queue.pop_front();
			return;
		}

		_loaded_meshes.emplace(mesh_load_task.Name, std::make_unique<Mesh>(*obj_file));
		for (auto&& callback : mesh_load_task.OnLoaded)
		{
			callback(_loaded_meshes[mesh_load_task.Name].get());
		}

		_currently_loading.erase(mesh_load_task.Name);
		_mesh_queue.pop_front();
	}
}
