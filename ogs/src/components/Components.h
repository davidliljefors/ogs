#pragma once

#include "CoreTypes.h"

namespace ogs {

template<typename T>
struct Handle
{
	u16 id;
};

template<typename Instance>
class LinearStorage
{
public:
	Handle<Instance> Alloc()
	{
		const auto instanceIndex = static_cast<u16>(m_instances.size());
		m_instances.emplace_back();

		const u16 instanceId = [&] {
			if (!m_freelist.empty())
			{
				const u16 id = m_freelist.back();
				m_freelist.pop_back();
				return id;
			}
			else
			{
				const u16 id = static_cast<u16>(m_lookup.size());
				m_lookup.emplace_back();
				return id;
			}
		}();

		m_lookup[instanceId] = instanceIndex;
		m_reverseLookup[instanceIndex] = instanceId;
		return Handle<Instance>{instanceId};
	}

	void Free(Handle<Instance> instance)
	{
		m_freelist.push_back(instance.id);
		const u16 instanceIndex = m_lookup[instance.id];

		if (instanceIndex == u16(m_instances.size() - 1))
		{
			m_reverseLookup.erase(instanceIndex);
			m_instances.pop_back();
		}
		else
		{
			m_reverseLookup.erase(instanceIndex);
			m_instances[instanceIndex] = m_instances.back();
			m_instances.pop_back();
			m_lookup[m_reverseLookup[m_instances.size()]] = instanceIndex;
		}
	}

	Instance& Get(Handle<Instance> handle)
	{
		return m_instances[m_lookup[handle.id]];
	}

protected:
	std::vector<Instance> m_instances;
	std::unordered_map<u16, u16> m_reverseLookup;
	std::vector<u16> m_lookup;
	std::vector<u16> m_freelist;
};

}