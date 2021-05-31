#include "ogspch.h"
#include "AssetLibrary.h"


ogs::Texture* ogs::AssetLibrary::GetTexture( std::string const& path )
{
	if ( _loaded_textures.contains( path ) )
	{
		return _loaded_textures[path].get();
	}

	_loaded_textures.emplace( path, std::make_unique<Texture>( path ) );
	return _loaded_textures[path].get();
}

ogs::Mesh* ogs::AssetLibrary::GetMesh( std::string const& path )
{
	if ( _loaded_meshes.contains( path ) )
	{
		return _loaded_meshes[path].get();
	}
	auto obj_file = wavefront_load( path );
	_loaded_meshes.emplace( path, std::make_unique<Mesh>( *obj_file ) );
	return _loaded_meshes[path].get();
}

void ogs::AssetLibrary::GetMeshAsync( std::string const& path, MeshLoadedCallback&& callback )
{
	if ( _loaded_meshes.contains( path ) )
	{
		callback( _loaded_meshes[path].get() );
		return;
	}

	if ( _currently_loading.contains( path ) )
	{
		auto task = std::ranges::find_if( _mesh_queue, [&path = path]( MeshLoadTask const& task )
			{
				return task.Name == path;
			} );
		if ( task != _mesh_queue.end() )
		{
			task->OnLoaded.push_back( std::move( callback ) );
		}
		return;
	}

	auto future = std::async( std::launch::async,
		[path = path, &finished_tasks = _finished_tasks]()
		-> std::unique_ptr<Wavefront_File> {
			auto obj_file = wavefront_load( path );
			finished_tasks.push_back( path );
			return std::move( obj_file );
		} );

	_currently_loading.insert( path );

	std::vector<MeshLoadedCallback> callbacks;
	callbacks.push_back( std::move( callback ) );
	_mesh_queue.emplace_back( path, std::move( future ), std::move( callbacks ) );
}


// Maintain is called in engine thread to synchronize threads with openGL. 
void ogs::AssetLibrary::Maintain()
{
	while ( !_finished_tasks.empty() )
	{
		{
			auto loaded = _finished_tasks.front();

			auto task = std::ranges::find_if( _mesh_queue,
				[&loaded = loaded.get()]( MeshLoadTask& task )
			{ return task.Name == loaded; }
			);

			auto obj_file = task->File.get();
			if ( !obj_file )
			{
				LogError( "Object file was invalid after finishing load" );
				return;
			}

			_loaded_meshes.emplace( task->Name, std::make_unique<Mesh>( *obj_file ) );
			for ( auto&& callback : task->OnLoaded )
			{
				callback( _loaded_meshes[task->Name].get() );
			}

			_currently_loading.erase( task->Name );
			_mesh_queue.erase( task );
		}
		_finished_tasks.pop_front();
	}
}
