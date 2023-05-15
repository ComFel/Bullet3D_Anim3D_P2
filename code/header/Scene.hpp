// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#pragma once

#include "Entity.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <Render_Node.hpp>
#include <SFML\Graphics.hpp>
#include <btBulletDynamicsCommon.h>

using namespace sf;
using namespace std;	

namespace catapult
{
	class Entity;
	
	// La clase Scene se encarga de inicializar todas la entidades que tendra la escena y de renderizarlas. 	
	class Scene
	{
	private:

		// Numero de balas que instancia vehicle
		int numberOfBalls = 0;

		// Se pasa al id de la bala, la cual se iguala en el Update a la conversion a string de la variable numberOfBalls (int)		
		string num_str;

		// Booleano para mover la door del centerFloor		
		bool isActive = false;

		// Comprueba hasta cuando puede moverse la plataforma hacia la derecha		
		bool rightDirection = true;

		// Comprueba cuando un vehicle este sobre el para iniciar el movimiento
		bool CatapultOnPlatform = false;

		// Esta a true solo una vez para crear una pelota que dispara		
		bool createBall = true;

		// Puntero de btDefaultCollisionConfiguration		
		btDefaultCollisionConfiguration* collisionConfiguration;

		// Puntero de btCollisionDispatcher		
		btCollisionDispatcher* collisionDispatcher;
		
		// Puntero de btDbvtBroadphase		
		btDbvtBroadphase* overlappingPairCache;

		// Puntero de btSequentialImpulseConstraintSolver		
		btSequentialImpulseConstraintSolver* constraintSolver;

		// Puntero de btDiscreteDynamicsWorld, mundo de la escena		
		unique_ptr < btDiscreteDynamicsWorld > dynamicsWorld;

		// Puntero de Render_Node		
		shared_ptr < glt::Render_Node > renderer;

	public:

		// Mapa donde se guardan todas las entidades		
		map < string, shared_ptr < Entity > > entityMap;
		
		// Getters del mundo y render
		btDiscreteDynamicsWorld& getWorld() const { return *dynamicsWorld; }
		
		glt::Render_Node& getRenderNode() const { return *renderer; }

	public:
		// Constructor		
		Scene(btVector3 gravedad, Window* window);

		// Funcion que llama al renderer_node para renderizar la escena
		inline void Render() { renderer->render(); }
				
		void Update();		
		
		void DeleteEntity(string id);

		// Funcion para comprobar colisiones
		void Trigger();

		// Carga los modelos para el constructor
		void ChargeModelsToScene();
				 

		// Funcion para crear la municion del vehicle
		void InstantiateBalls();
	};
}