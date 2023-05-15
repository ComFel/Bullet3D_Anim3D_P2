// Modificaiones del mismo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#include "Transform.hpp"
#include "Entity.hpp"
#include "Rigidbody.hpp"

namespace catapult
{
	// Constructor del componente de myEntity
	Rigidbody::Rigidbody(float masa, float restitution, btVector3 collider_dimensions, Scene* scene, shared_ptr< Entity> entity, string type)
	{
		myEntity = entity;
		
		transform.reset(new Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));

		transform->transform.setIdentity();
		transform->transform.setOrigin(btVector3(myEntity->transform->position.x, myEntity->transform->position.y, myEntity->transform->position.z));

		btVector3 localInertia(0, 0, 0);

		shapes.reset(new btBoxShape(collider_dimensions));

		shapes->calculateLocalInertia(masa, localInertia);

		states.reset(new btDefaultMotionState(transform->transform));
		btRigidBody::btRigidBodyConstructionInfo info(masa, states.get(), shapes.get(), localInertia);
		rigidbody.reset(new btRigidBody(info));

		if (type == "kinematic")
		{
			rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		}

		rigidbody->setRestitution(restitution);
		rigidbody->setActivationState(DISABLE_DEACTIVATION);

		scene->getWorld().addRigidBody(rigidbody.get());

		rigidbody->setUserPointer(myEntity.get());
	};
}