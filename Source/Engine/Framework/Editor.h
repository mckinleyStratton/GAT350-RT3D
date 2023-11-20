#pragma once

namespace nc
{
	class Editor
	{
	public:
		void ProgessGui(class Scene* scene);

	private:
		class Object* m_selected = nullptr;
	};
}