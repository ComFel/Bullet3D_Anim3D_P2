// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#pragma once

#include "Transform.hpp"
#include <memory>
#include <btBulletDynamicsCommon.h>
#include <string.h>

using namespace std;

namespace catapult
{
	class Entity;
	class Scene;

	// Encargada de crear y añadir rigidbodies a las entidades	
	class Rigidbody
	{
		// Puntero a myEntity
		shared_ptr< Entity> myEntity;

		// Puntero al Transform	de myEntity
		shared_ptr< Transform > transform;

		//Puntero al rigidbody de myEntity

		shared_ptr< btRigidBody > rigidbody;

		// Puntero a la clase btCollisionShape que guarda las formas de myEntity		
		shared_ptr< btCollisionShape > shapes;

		// Puntero a la clase btMotionState que guarda los estados de myEntity		
		shared_ptr< btMotionState > states;

		// Etiqueta de las entidades sobre sus rigidbodies para diferenciarlos entre dinamicos o kinematicos		
		string type;

	public:
		// Constructor de rigidbody, donde estableces masa, restitucion, dimensiones, referencias a la escena, myEntity y tipo de esta
		Rigidbody(float masa, float restitution, btVector3 collider_dimensions, Scene* scene, shared_ptr < Entity > entity, string type);

		// Getters de la clase para obtener sus componentes
		btRigidBody& getRigidbody() { return *rigidbody; }

		shared_ptr< btCollisionShape> getCollisionShape() { return shapes; }

		shared_ptr< btMotionState> getMotionState() { return states; }

		string& getType() { return type; };
	};
}
