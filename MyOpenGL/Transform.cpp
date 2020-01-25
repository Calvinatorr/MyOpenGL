#include "Transform.h"

const glm::vec3 Transform::WORLD_FORWARD = glm::vec3(0.0, 0.0, 1.0f);
const glm::vec3 Transform::WORLD_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

Transform::Transform()
	: position(glm::vec3(0.0f)), rotation(glm::quat()), scale(glm::vec3(1.0f))
{
}

Transform::Transform(const glm::mat4 & Matrix)
{
	ConstructFromMatrix(Matrix);
}


Transform::~Transform()
{
}

void Transform::ConstructFromMatrix(const glm::mat4 & Matrix)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(Matrix, scale, rotation, position, skew, perspective);
}

void Transform::CombineTransform(const Transform & NewTransform)
{
	glm::mat4 a = GetMatrix();
	glm::mat4 b = NewTransform.GetMatrix();
	glm::mat4 result = a * b;

	ConstructFromMatrix(a);
}

json Transform::Serialize()
{
	json jsonData = {
		{"transform", {
			{"position", {position.x, position.y, position.z			}},
			{"rotation", {rotation.x, rotation.y, rotation.z, rotation.w}},
			{"scale",	 {scale.x, scale.y, scale.z						}},
		}}
	};
	return jsonData;
}

void Transform::Deserialize(const json & Data)
{
	// If the transform isn't empty
	if (!Data.empty())
	{
		auto pos = Data["position"];
		position = glm::vec3(pos[0], pos[1], pos[2]);
		auto rot = Data["rotation"];
		rotation.x = rot[0]; rotation.y = rot[1]; rotation.z = rot[2]; rotation.w = rot[3];
		auto s = Data["scale"];
		scale = glm::vec3(s[0], s[1], s[2]);
	}
}

Transform Transform::operator*(const Transform & NewTransform)
{
	*this *= NewTransform;
	return *this;
}

void Transform::operator*=(const Transform & NewTransform)
{
	CombineTransform(NewTransform);
}

glm::vec3 Transform::GetForward() const
{
	return rotation * WORLD_FORWARD;
}

glm::vec3 Transform::GetUp() const
{
	return rotation * WORLD_UP;
}

glm::vec3 Transform::GetRight() const
{
	return glm::normalize(glm::cross(GetUp(), GetForward()));
}

glm::mat4 Transform::GetMatrix() const
{
	glm::mat4 s = glm::scale(scale);
	glm::mat4 r = glm::toMat4(rotation);
	glm::mat4 t = glm::translate(position);

	glm::mat4 transform = t * r * s;
	return transform;
}

glm::mat4 Transform::GetMatrixWithoutScale() const
{
	glm::mat4 r = glm::toMat4(rotation);
	glm::mat4 t = glm::translate(position);

	glm::mat4 transform = t * r;
	return transform;
}

void Transform::DrawDetails()
{
#if WITH_EDITOR
	if (ImGui::TreeNodeEx("Transform", panelFlags))
	{
		float alpha = .2f;

		// Position
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, alpha));
		ImGui::DragFloat3("Position", glm::value_ptr(position), .01f, .0f, .0f, "%.3f", .05f);

		const auto defaultPosition = glm::vec3();
		if (position != defaultPosition)
		{
			ImGui::SameLine();
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 0.0f, alpha));
			if (ImGui::Button("r##position"))
			{
				position = defaultPosition;
			}
		}


		// Rotation
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 1.0f, alpha));
		//ImGui::DragFloat4("Rotation", glm::value_ptr(rotation), .01f, -1, 1, "%.3f", .05f);
		glm::vec3 eulerRotation = glm::eulerAngles(rotation) * (180.0f / glm::pi<float>()); // Degrees and euler
		ImGui::DragFloat3("Rotation", glm::value_ptr(eulerRotation), .1f, -180.0f, 180.0f, "%.3f", 1.0f);
		rotation = glm::highp_quat(eulerRotation * (glm::pi<float>() / 180.0f));

		const auto defaultRotation = glm::vec3();
		if (eulerRotation != defaultRotation)
		{
			ImGui::SameLine();
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 0.0f, alpha));
			if (ImGui::Button("r##rotation"))
			{
				rotation = glm::highp_quat();
			}
		}


		// Scale
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 1.0f, 0.0f, alpha));
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), .01f, .0f, .0f, "%.3f", .05f);

		const auto defaultScale = glm::vec3(1.0f, 1.0f, 1.0f);
		if (scale != defaultScale)
		{
			ImGui::SameLine();
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 0.0f, alpha));
			if (ImGui::Button("r##scale"))
			{
				scale = defaultScale;
			}
		}


		ImGui::PopStyleColor();
		ImGui::TreePop();
	}
#endif
}
