#include "Transform.h"

const glm::vec3 Transform::WORLD_FORWARD = glm::vec3(0.0, 0.0, 1.0f);
const glm::vec3 Transform::WORLD_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

Transform::Transform()
	: position(glm::vec3(0.0f)), rotation(glm::quat()), scale(glm::vec3(1.0f))
{
}


Transform::~Transform()
{
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
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::toMat4(rotation) * transform;
	transform = glm::translate(transform, position);
	transform = glm::scale(transform, scale);

	return transform;

	/*glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
	return glm::lookAt(position, position + GetForward(), GetUp()) * s;*/
}

glm::mat4 Transform::GetMatrixWithoutScale() const
{
	//return glm::lookAt(position, position + GetForward(), GetUp());

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::toMat4(rotation) * transform;
	transform = glm::translate(transform, position);

	return transform;
}