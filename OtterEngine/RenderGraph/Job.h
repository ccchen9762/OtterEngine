#pragma once

class Entity;
class Graphics;

namespace RG {

	class Operation;

	class Job
	{
	public:
		Job(const Operation* operation, const Entity* entity);
		~Job() = default;
		void Execute(const Graphics& graphics) const;

	private:
		const Entity* m_pParentEntity;
		const Operation* m_pOperation;
	};
}