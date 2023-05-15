// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#include "Entity.hpp"

namespace catapult
{
	void Entity::Update()
	{
		btTransform new_transform;

		// Si myEntity tiene rigidbody actualizo su posicion y rotacion 
		if (rigidbody)
		{
			new_transform = rigidbody->getRigidbody().getCenterOfMassTransform();
			transform->position = glm::vec3(new_transform.getOrigin().x(), new_transform.getOrigin().y(), new_transform.getOrigin().z());
			transform->rotation = glm::vec3(new_transform.getRotation().x(), new_transform.getRotation().y(), new_transform.getRotation().z());
			rigidbody->getRigidbody().getMotionState()->getWorldTransform(new_transform);
		}

		// Actualizo la matriz de transformacion
		node->set_transformation(transform->convertToMatrix());
	}

	Entity::Entity(shared_ptr<glt::Node> _node, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* _scene)
	{
		node = _node;
		transform.reset(new Transform(pos, rot, sca));
		scene = _scene;
	}

	shared_ptr< Entity > Entity::modelImport(string id, const char* path, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* _scene)
	{
		shared_ptr < glt::Node > nodeImp(new glt::Model_Obj(path));
		shared_ptr< Entity > entity(new Entity(nodeImp, pos, rot, sca, _scene));

		_scene->getRenderNode().add(id, nodeImp);

		return entity;
	}

	void Entity::Move(btVector3 vec, float impulse)
	{
		rigidbody->getRigidbody().setActivationState(1);
		rigidbody->getRigidbody().applyCentralForce(vec * impulse);
	}

	void Entity::KinematicMovement(btVector3 vec)
	{
		btTransform newBttranform;
		rigidbody->getMotionState()->getWorldTransform(newBttranform);
		newBttranform.getOrigin() += (vec);
		rigidbody->getMotionState()->setWorldTransform(newBttranform);
	}

	shared_ptr< Entity > Entity::CreateCamera(string id, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* _scene)
	{
		// Puntero a la clase Camera que inicializa con los parametros de una camara base
		shared_ptr< glt::Camera > camera(new glt::Camera(20.f, 0.1f, 50000.f, 1920.f / 1080.f));		
		shared_ptr< glt::Node > nodeCam(camera);

		// Creo myEntity de la camara pasandole como atributo el Node inicializado como camara
		shared_ptr< Entity > entityCam(new Entity(nodeCam, pos, rot, sca, _scene));

		// Añado myEntity al renderer de mi clase Scene
		_scene->getRenderNode().add(id, nodeCam);

		return entityCam;
	}

	shared_ptr< Entity > Entity::CreateLight(string id, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* _scene)
	{
		// Puntero a la clase Node que se inicializa con la clase Light
		shared_ptr< glt::Node > nodeLight(new glt::Light());

		// Creo myEntity que lleva como atributo el puntero a Node inicializado como Luz
		shared_ptr< Entity > entityLight(new Entity(nodeLight, pos, rot, sca, _scene));

		// Añado myEntity al renderer de mi clase Scene
		_scene->getRenderNode().add(id, nodeLight);

		return entityLight;
	}

	void Entity::CreateConstraint(Rigidbody* rigidbody_A, Rigidbody* rigidbody_B, btVector3 axis, Scene* s)
	{
		shared_ptr < btHingeConstraint > constraint;

		// Creo un constraint en funcion de los parametros que le pasen
		constraint = make_shared< btHingeConstraint >(rigidbody_A->getRigidbody(), rigidbody_B->getRigidbody(), btVector3(0, 0, 0), axis, btVector3(0, 0, 0), btVector3(0, 0, 0));

		// Los añado al mundo
		s->getWorld().addConstraint(constraint.get());
	}
}