#pragma once

#include <deque>
#include <mutex>

namespace ogs::containers
{
template<typename T>
class ts_queue
{
public:
	ts_queue() = default;
	
	auto size()
	{
		std::unique_lock lock(_mutex);
		return _deque.size();
	}

	void push_back(T&& val)
	{
		std::unique_lock lock(_mutex);
		_deque.push_back(std::move(val));
	}

	void push_back(T const& val)
	{
		std::unique_lock lock(_mutex);
		_deque.push_back(val);
	}

	auto& front()
	{
		std::unique_lock lock(_mutex);
		return _deque.front();
	}

	void pop_front()
	{
		std::unique_lock lock(_mutex);
		return _deque.pop_front();
	}

	auto& operator[](size_t index)
	{
		std::unique_lock lock(_mutex);
		return _deque[index];
	}
	
	auto begin()
	{
		std::unique_lock lock(_mutex);
		return _deque.begin();
	}

	auto end()
	{
		std::unique_lock lock(_mutex);
		return _deque.end();
	}

	auto empty()
	{
		std::unique_lock lock(_mutex);
		return _deque.empty();
	}
	
private:
	std::mutex _mutex{};
	std::deque<T> _deque{};
};
}