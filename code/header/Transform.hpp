// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <btBulletDynamicsCommon.h>

using namespace std;

namespace catapult
{
	// Clase que encapsula la matriz de transformacion de las entidades	
	class Transform
	{
	public:
		// transform de Bullet		
		btTransform transform;

		// Vector para la posicion
		glm::vec3 position;

		// Vector para la rotacion		
		glm::vec3 rotation;

		// Vector parla escala		
		glm::vec3 scale;

		// Contructor		
		Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
		
		// Retorna la matriz transformada de myEntity		
		glm::mat4 convertToMatrix();

		// Controlamos la posicion de myEntity manualmente con una posicion concreta
		void SetPosition(glm::vec3 pos);
	};
}