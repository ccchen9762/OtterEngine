#pragma once

#include <string>
#include <vector>
#include <memory>

class Graphics;
class GraphicsResource;

namespace RG {

	class Job;

	class Pass
	{
	public:
		Pass(const std::wstring& name);
		virtual ~Pass() = default;

		const std::wstring& GetName() const { return m_name; }

		void RegisterResource(const std::wstring& resource);

		virtual void Execute(const Graphics& graphics) const = 0;

	private:
		std::wstring m_name;
		std::vector<std::unique_ptr<GraphicsResource>> m_pResources;
	};
}