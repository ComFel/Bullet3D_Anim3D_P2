// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#pragma once

#include "Entity.hpp"

using namespace std;

namespace catapult
{
	class Scene;
	// Clase que gestiona los eventos de la llave	
	class keyLock : public Entity
	{
	public:
		// Puntero a la escena
		shared_ptr < Scene > scene;

		// Función que detecta si hay colision entre la llave y otra myEntity que se le pase por parametro para desbloquear los eventos pertinentes		
		void IsCollision(Entity* entitySel) override;
	};
}