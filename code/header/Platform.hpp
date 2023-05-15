// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#pragma once

#include "Entity.hpp"

namespace catapult
{
	// Clase que gestiona la platform horizontal que se desplaza por la escena
	class Platform : public Entity
	{
		// Control del estado de movimiento de myEntity (plataforma)		
		bool moving = false;
			
	public:

		// Funcion que ejecuta cada frame las condiciones para los eventos de la paltaforma		
		void Update() override;
	};
}