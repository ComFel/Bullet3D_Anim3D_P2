// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#include "Transform.hpp"

namespace catapult
{
	Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) : position(pos), rotation(rot), scale(sca)
	{
		position = pos;
		rotation = rot;
		scale	 = sca;
	}

	glm::mat4 Transform::convertToMatrix()
	{
		glm::mat4 matrix(1);

		matrix = glm::translate(matrix, position);
		matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
		matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
		matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
		matrix = glm::scale(matrix, scale);

		return matrix;
	}

	void Transform::SetPosition(glm::vec3 pos)
	{
		position = pos;
	}
}
