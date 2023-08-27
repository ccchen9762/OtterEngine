#pragma once

#include <unordered_map>

#include "Resource/GraphicsResource.h"

class ResourcePool
{
public:
	template<class T, typename... Args>
	static std::shared_ptr<GraphicsResource> GetResource(const Graphics& graphics, Args&&... arg) {
		std::wstring key = T::GenerateUID(std::forward<Args>(arg)...);
		std::unordered_map<std::wstring, std::shared_ptr<GraphicsResource>>::const_iterator resource = GetInstance().m_resourcesMap.find(key);
		
		if (resource == GetInstance().m_resourcesMap.end()) {
			std::shared_ptr<T> newResource = std::make_shared<T>(graphics, std::forward<Args>(arg)...);
			GetInstance().m_resourcesMap[newResource->GetUID()] = newResource;
			
			return newResource;
		}
		
		return resource->second;
	}

private:
	ResourcePool() {}
	~ResourcePool() = default;

	// prevent copying
	ResourcePool(const ResourcePool&) = delete;
	ResourcePool& operator= (const ResourcePool&) = delete;

	static ResourcePool& GetInstance() {
		static ResourcePool resourcePool;
		return resourcePool;
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<GraphicsResource>> m_resourcesMap;
};