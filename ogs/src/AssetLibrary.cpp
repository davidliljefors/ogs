#include "AssetLibrary.h"
#include <ranges>

void ogs::AssetLibrary::GetMesh(std::string const& path, MeshLoadedCallback&& callback)
{
	if (_loaded_meshes.contains(path))
	{
		callback(_loaded_meshes[path].get());
		return;
	}

	if (_currently_loading.contains(path))
	{
		auto task = std::ranges::find_if(_mesh_queue, [&path = path](MeshLoadTask const& task)
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
		[path = path, &finished_tasks = _finished_tasks]()
		-> std::unique_ptr<Wavefront_File> {
		auto obj_file = wavefront_load(path);
		finished_tasks.push_back(path);
		return std::move(obj_file);
		});

	_currently_loading.insert(path);

	std::vector<MeshLoadedCallback> callbacks;
	callbacks.push_back(std::move(callback));
	_mesh_queue.emplace_back(path, std::move(future), std::move(callbacks));
}

void ogs::AssetLibrary::Maintain()
{
	while (!_finished_tasks.empty())
	{
		auto& loaded = _finished_tasks.front();

		auto task = std::ranges::find_if(_mesh_queue, 
			[&loaded = loaded] (MeshLoadTask& task) 
			{ return task.Name == loaded; }
		);

		auto obj_file = task->File.get();
		if (!obj_file)
		{
			LogError("Object file was invalid after finishing load");
			return;
		}
		_loaded_meshes.emplace(task->Name, std::make_unique<Mesh>(*obj_file));
		for (auto&& callback : task->OnLoaded)
		{
			callback(_loaded_meshes[task->Name].get());
		}
		_currently_loading.erase(task->Name);
		_mesh_queue.erase(task);
		//std::erase_if(_mesh_queue, [&task = task](auto& item) {return item.Name == task->Name; });
		_finished_tasks.pop_front();
	}
}
