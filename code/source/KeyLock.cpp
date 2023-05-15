
// Modificaiones del codigo por:
// Felipe Vallejo Molina
// felipevm07@gmail.com
// 2022.06

#include "keyLock.hpp"
#include "Scene.hpp"

namespace catapult
{
	// Detectar el id de myEntity para desbloquear el evento siguiente
	void keyLock::IsCollision(Entity* entity)
	{
		if (entity->getId() == "vehicle")
		{
			// Destroy la llave si colisiona el jugador
			scene->DeleteEntity("keyLock");
		}
	}
}
