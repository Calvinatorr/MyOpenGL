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