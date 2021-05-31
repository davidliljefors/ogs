#pragma once

#include <deque>
#include <mutex>

namespace ogs::containers
{

template<typename T, typename Mutex>
class GuardedRef
{
public:
	GuardedRef() = delete;
	GuardedRef( GuardedRef const& ) = delete;
	GuardedRef( GuardedRef&& ) = default;

 	GuardedRef( T& t, std::unique_lock<Mutex> lock )
		: lock( std::move( lock ) ), rw( std::ref( t ) )
	{}

	T& get()
	{
		return rw.get();
	}
private:
	std::reference_wrapper<T> rw;
	std::unique_lock<Mutex> lock;
};

template<typename T>
class ts_queue
{
public:
	ts_queue() = default;

	auto size()
	{
		std::unique_lock lock( _mutex );
		return _deque.size();
	}

	void push_back( T&& val )
	{
		std::unique_lock lock( _mutex );
		_deque.push_back( std::move( val ) );
	}

	template<typename... Ts>
	void emplace_back( Ts&&... args )
	{
		std::unique_lock lock( _mutex );
		_deque.emplace_back( std::forward<Ts>( args )... );
	}

	void push_back( T const& val )
	{
		std::unique_lock lock( _mutex );
		_deque.push_back( val );
	}

	auto front()
	{
		std::unique_lock lock( _mutex );
		return GuardedRef{ _deque.front(), std::move( lock ) };
	}

	void pop_front()
	{
		std::unique_lock lock( _mutex );
		return _deque.pop_front();
	}

	auto operator[]( size_t index )
	{
		std::unique_lock lock( _mutex );
		return GuardedRef{ _deque[index], std::move( lock ) };
	}

	auto begin()
	{
		std::unique_lock lock( _mutex );
		return _deque.begin();
	}

	auto end()
	{
		std::unique_lock lock( _mutex );
		return _deque.end();
	}

	auto empty()
	{
		std::unique_lock lock( _mutex );
		return _deque.empty();
	}

private:
	std::mutex _mutex{};
	std::deque<T> _deque{};
};
}