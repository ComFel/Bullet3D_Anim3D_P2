// 
// Basado en un código de dominio público de:
// angel.rodriguez@esne.edu
// 2021.03+
// 
// Modificaiones del mismo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#include "Scene.hpp"
#include "Entity.hpp"
#include <Light.hpp>
#include <Model.hpp>
#include <OpenGL.hpp>
#include <SFML/Window.hpp>
#include <btBulletDynamicsCommon.h>

using namespace sf;
using namespace std;
using namespace glt;
using namespace catapult;

int main()
{	
	// Se crea la ventana y el contexto de OpenGL asociado a ella:
	// Se usa OpenGL 3.2 core profile
	Window window(VideoMode(1376, 768),"Practica_2 Catapult",Style::Titlebar | Style::Close,ContextSettings(24, 0, 0, 3, 2, ContextSettings::Core));


	// Se determinan las caracteristicas de OpenGL disponibles en la maquina:
	if (!glt::initialize_opengl_extensions())
	{
		exit(-1);
	}

	// Se activa la sincronización vertical:
	window.setVerticalSyncEnabled(true);
	
	// Variables fisicas para el control de las entidades de la escena
	float directionForce  =   30.f;
	float vehicleImpulse =  1.f;
	float gravity = -10.f;

	// Se crea y se configura la escena:
	Scene scene(btVector3(0.f, gravity, 0.f), &window);

	// Añadimos un color a la ventana para contraste
	glClearColor(0.3, 0.15f, 0.1f, 0.2f);

	bool running = true;
	int  frame = 0;

	do
	{
		
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				running = false;
			}

			// Control del input y movimiento del jugador
			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case Keyboard::W:
						scene.entityMap["vehicle"]->Move(btVector3(0.f, 0.f, -directionForce), vehicleImpulse);
						break;

					case Keyboard::S:
						scene.entityMap["vehicle"]->Move(btVector3( 0.f, 0.f, directionForce), vehicleImpulse);
						break;

					case Keyboard::A:
						scene.entityMap["vehicle"]->Move(btVector3(-directionForce, 0.f, 0.f), vehicleImpulse);
						break;

					case Keyboard::D:
						scene.entityMap["vehicle"]->Move(btVector3(directionForce, 0.f, 0.f), vehicleImpulse);
						break;

					case Keyboard::Space:
						scene.InstantiateBalls();
						break;
				}
			}
		}

		// Perform the simulation:
		scene.getWorld().stepSimulation(1.f / 60.f);

		// Apply the physics transform to the graphics model:
		scene.Update();

		// Render the scene:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene.Render();

		window.display();
	} while (running);

	return EXIT_SUCCESS;
}