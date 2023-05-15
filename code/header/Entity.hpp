
// Codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "Scene.hpp"
#include "Rigidbody.hpp"
#include "Transform.hpp"
#include <Model.hpp>
#include <Model_Obj.hpp>
#include <Camera.hpp>
#include <Light.hpp>

using namespace std;

namespace catapult
{
	class Scene;
	
	//La clase Entity es la encargado de crear los diferentes componentes necesarios para crear la escena	
	class Entity
	{
	public:
		// Puntero a la escena
		Scene* scene;

		// Puntero al transform de myEntity		
		shared_ptr < Transform > transform;

		// Puntero al rigidbody de myEntity
		shared_ptr < Rigidbody > rigidbody;

		// Puntero  ala btHingeConstraint
		shared_ptr < btHingeConstraint > constraint;

		// Puntero ala Node		
		shared_ptr < glt::Node > node;

		// string identificador de myEntity
		string id;

	public:
		// Constructor
		Entity() {};

		virtual void Update();
		// Constructor en el que se inicializan atributos de myEntity, posicion, rotacion, escala y referencia a la escena donde situarlo
		Entity(shared_ptr < glt::Node > n, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* scene);

		// Funcion en la que se importan modelos para cargarlos a la escena		
		static  shared_ptr< Entity > modelImport(string id, const char* path, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* scene);

		// Funcion donde se crea la camara de la escena		
		static shared_ptr< Entity > CreateCamera(string id, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* scene);
		
		// Funcion en la que se crea la luz de la escena		
		static shared_ptr< Entity > CreateLight(string id, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Scene* scene);
		
		// Funcion que crea constraints de los cuerpos de las entidades
		static void CreateConstraint(Rigidbody* rigidbody_A, Rigidbody* rigidbody_B, btVector3 axis, Scene* scene);

		// Funcion que mueve myEntity de rigidbody dinamico
		void Move(btVector3 vec, float impulse);

		// Funcion que mueve myEntity de rigidbody kinematico		
		void KinematicMovement(btVector3 vec);

		// Función que devuelve el rigidbody de myEntity
		shared_ptr< Rigidbody > getRigidbody() { return rigidbody; }
		
		// Función que retorna el id de una myEntity		
		string getId() const { return id; };

		// Función que detecta si hay colision de myEntity que se indique
		virtual void IsCollision (Entity* entitySel) {};
	};
}