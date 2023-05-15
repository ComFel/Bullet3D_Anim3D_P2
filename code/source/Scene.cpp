// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#include "Scene.hpp"
#include "keyLock.hpp"

namespace catapult
{
	using namespace glt;

	Scene::Scene(btVector3 gravedad, Window* window)
	{
		renderer.reset(new glt::Render_Node());

		collisionConfiguration = new btDefaultCollisionConfiguration();
		collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		constraintSolver = new btSequentialImpulseConstraintSolver();

		dynamicsWorld.reset(new btDiscreteDynamicsWorld(collisionDispatcher, overlappingPairCache, constraintSolver, collisionConfiguration));
		dynamicsWorld->setGravity(gravedad);

		// Creo los componentes de la escena
		glViewport(0, 0, window->getSize().x, window->getSize().y);

		// Scene Components
		entityMap["camara"] = Entity::CreateCamera("camera", glm::vec3(0, 7, 13), glm::vec3(-0.4f, 0, 0), glm::vec3(1, 1, 1), this);
		entityMap["light"] = Entity::CreateLight("light", glm::vec3(0, 9, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), this);

		ChargeModelsToScene();

		entityMap["leftFloor"]->rigidbody.reset(new Rigidbody( 0.f,  1.f, btVector3( 4.f,1.f, 7.f), this, entityMap["leftFloor"], "dinamic"));
		entityMap["platform"]->rigidbody.reset(new Rigidbody( 0.f, 0.1f, btVector3(2.f, 0.4f, 3.5f), this, entityMap["platform"], "kinematic"));
		entityMap["centerFloor"]->rigidbody.reset(new Rigidbody( 0.f, 1.f, btVector3(3.f, 1.f, 7.f), this, entityMap["centerFloor"], "dinamic"));
		entityMap["door"]->rigidbody.reset(new Rigidbody( 0.f, 0.1f, btVector3(0.5f, 3.f, 7.f), this, entityMap["door"], "kinematic"));
		entityMap["rightFloor"]->rigidbody.reset(new Rigidbody( 0.f, 1.f, btVector3(3.f, 2.f, 7.f), this, entityMap["rightFloor"], "dinamic"));
		entityMap["endKey"]->rigidbody.reset(new Rigidbody(0.1f, 0.1f, btVector3(0.5f, 2.f, 0.5f), this, entityMap["endKey"],"dinamic"));
		entityMap["vehicle"]->rigidbody.reset(new Rigidbody( 2.f, 0.1f, btVector3(0.09f, 0.09f, 0.09f), this, entityMap["vehicle"], "dinamic"));
		entityMap["keyLock"]->rigidbody.reset(new Rigidbody(0.1f, 0.1f, btVector3(1.f, 1.f, 1.f), this, entityMap["keyLock"], "kinematic"));
	}

	void Scene::Update()
	{
		// Recorre el mapa de entidades y llama el Update de cada uno de ellos constantemente
		std::map <std::string, std::shared_ptr<Entity>>::iterator it;
		for (it = entityMap.begin(); it != entityMap.end(); it++)
		{
			it->second->Update();
		};

		// Comprobacion de colisiones
		Trigger();

		// Compruebo por coordenadas la posición de la vehicle para saber cuando activar los eventos		
		if (entityMap["vehicle"]->transform->position.x >= entityMap["platform"]->transform->position.x - 2.5f &&
			entityMap["vehicle"]->transform->position.z > -6.f &&
			entityMap["platform"]->transform->position.z < -4.f)
		{
			CatapultOnPlatform = true;
		}

		if (CatapultOnPlatform && entityMap["vehicle"]->transform->position.x >= -2.5f)
		{
			CatapultOnPlatform = false;
		}

		// Comprobación cuando el jugador esta sobre la plataforma
		if(CatapultOnPlatform)
		{
			if (rightDirection)
			{
				if (entityMap["vehicle"]->transform->position.x < -0.5f)
				{
					entityMap["platform"]->KinematicMovement(btVector3(0.01f, 0.f, 0.f));
				}
				else
				{
					CatapultOnPlatform = false;
				}
			}
			else
			{
				if (entityMap["platform"]->transform->position.x > entityMap["leftFloor"]->transform->position.x + 6)
				{
					entityMap["platform"]->KinematicMovement(btVector3(-0.01f, 0.f, 0.f));
				}
				else
				{
					rightDirection = true;
				}
			}
		}

		// Compruebo por coordenadas la posicion de la vehicle en funcion de la llave para saber cuando debo activar la door

		if (entityMap["vehicle"]->transform->position.x >= entityMap["keyLock"]->transform->position.x  -2 &&
			entityMap["vehicle"]->transform->position.z >= entityMap["keyLock"]->transform->position.z  -2 &&
			entityMap["vehicle"]->transform->position.x <= entityMap["keyLock"]->transform->position.x + 2)
		{
			isActive = true;
			DeleteEntity("keyLock");
		}

		if (isActive)
		{
			entityMap["door"]->KinematicMovement(btVector3(0.f, 0.01f, 0.f));
		}

		/*
		// ToDo: infraccion de acceso de la memoria
		// Comprobacion de caida de las entidades de la escena
		if (entityMap["vehicle"]->transform->position.y <= -5.8f)
		{
			entityMap["vehicle"]->transform->position.x = entityMap["lelfFloor"]->transform->position.x;
			entityMap["vehicle"]->transform->position.y = entityMap["lelfFloor"]->transform->position.y + 1.f;
			entityMap["vehicle"]->transform->position.z = entityMap["lelfFloor"]->transform->position.z;
		}
		*/


		// Cada vez que dispara, aumenta en uno el nombre de la variable y se conviert a String. Ese nunmero es el id cuando se crea myEntity, de esa forma no se va a repetir nunca el id y podrá crearse sin problemas
		if (!createBall)
		{
			numberOfBalls += 1;
			num_str = std::to_string(numberOfBalls);
			createBall = true;
		}
	}

	void Scene::DeleteEntity(string id)
	{
		std::map <std::string, std::shared_ptr<Entity>>::iterator it;
		for (it = entityMap.begin(); it != entityMap.end(); it++)
		{
			if (it->first == id)
			{
				this->getWorld().removeRigidBody(&it->second->getRigidbody()->getRigidbody());
				this->getRenderNode().get(id)->set_visible(false);
			}
		};
	}

	void Scene::Trigger()
	{
		int manifold_count = dynamicsWorld->getDispatcher()->getNumManifolds();

		for (int i = 0; i < manifold_count; i++)
		{
			btPersistentManifold* manifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

			Entity* object_a = static_cast<Entity*>(manifold->getBody0()->getUserPointer());
			Entity* object_b = (Entity*)(manifold->getBody1()->getUserPointer());
			
			object_a->IsCollision(object_b);
			object_b->IsCollision(object_a);
		}
	}

	void Scene::ChargeModelsToScene()
	{
		// Enviroment
		entityMap["leftFloor"] = Entity::modelImport("leftFloor", "../../assets/cubo.obj", glm::vec3(-15, -5, -5), glm::vec3(0, 0, 0), glm::vec3(4, 0.4, 7), this);
		entityMap["centerFloor"] = Entity::modelImport("centerFloor", "../../assets/cubo.obj", glm::vec3(1, -5, -5), glm::vec3(0, 0, 0), glm::vec3(3, 0.4, 7), this);
		entityMap["rightFloor"] = Entity::modelImport("rightFloor", "../../assets/cubo.obj", glm::vec3(15, -5, -5), glm::vec3(0, 0, 0), glm::vec3(3, 0.4, 7), this);

		entityMap["platform"] = Entity::modelImport("platform", "../../assets/cubo.obj", glm::vec3(-9.f, -5, -5), glm::vec3(0, 0, 0), glm::vec3(3,0.1,2.5), this);

		// Interactuables
		entityMap["door"] = Entity::modelImport("door", "../../assets/door.obj", glm::vec3(1, -1, 0), glm::vec3(0, 1.57, 0), glm::vec3(0.01f, 0.01f, 0.01f), this);
		entityMap["endKey"] = Entity::modelImport("endKey", "../../assets/cubo.obj", glm::vec3(15, -1, -1.8), glm::vec3(0, 0, 0), glm::vec3(0.5f, 2, 2), this);
		entityMap["keyLock"] = Entity::modelImport("keyLock", "../../assets/keyLock.obj", glm::vec3(-13, -3, 1), glm::vec3(10.f, 10, 0), glm::vec3(0.5, 0.5, 0.5), this);
		
		// Vehicle
		entityMap["vehicle"] = Entity::modelImport("vehicle", "../../assets/vehicle.obj", glm::vec3(-15, -2, -5), glm::vec3(glm::radians(1.57), 0, 0), glm::vec3(0.1f, 0.1f, 0.1f), this);
	}

	void Scene::InstantiateBalls()
	{
		// Creo myEntity, rigidbody de una esfera y la disparo con cierta velocidad
		if (createBall)
		{
			entityMap[num_str] = Entity::modelImport(num_str, "assets/sphere.obj", glm::vec3(entityMap["vehicle"]->transform->position.x,
			entityMap["vehicle"]->transform->position.y + 2, entityMap["vehicle"]->transform->position.z), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5), this);

			entityMap[num_str]->rigidbody.reset(new Rigidbody(0.1f, 1.f, btVector3(0.5f, 0.5f, 0.5f), this, entityMap[num_str], ""));

			this->getRenderNode().get(num_str)->set_visible(true);
			entityMap[num_str]->Move(btVector3(5.f, 5.f, 0.f), 10.f);

			createBall = false;
		}
	}
}