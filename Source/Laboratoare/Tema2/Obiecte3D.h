#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

#include "Tema2.h"

namespace Obiecte3D
{
		Mesh* CreatePlatform(std::string name, glm::vec3 center, float length, float width, float height, glm::vec3 color, bool fill);
		Mesh* CreateRectangular(std::string name, glm::vec3 center, float lenght, float width, glm::vec3 color, bool fill);
}