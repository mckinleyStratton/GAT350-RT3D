#include "RotateComponent.h"
#include "Framework/Actor.h"
#include "Core/Math/MathUtils.h"


namespace nc
{
	CLASS_DEFINITION(RotateComponent);

	bool nc::RotateComponent::Initialize()
	{
		return true;
	}


	void RotateComponent::Update(float dt)
	{
		m_owner->transform.euler += euler * dt;
		glm::quat rotation = EulerToQuaternion(euler * dt);
		m_owner->transform.rotation = m_owner->transform.rotation * rotation;
	}


	void RotateComponent::ProccessGui()
	{
		ImGui::DragFloat3("EulerRotation", glm::value_ptr(euler));
	}

	void RotateComponent::Read(const json_t& value)
	{
		READ_NAME_DATA(value, "eulerRotation", euler);
	}

}