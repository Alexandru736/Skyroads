#include "Obiecte3D.h"
#include "Tema2.h"
#include <GL\glew.h>

Mesh* Obiecte3D::CreatePlatform(std::string name, glm::vec3 center, float length, float width, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, 0, length), color),
		VertexFormat(corner + glm::vec3(0, 0, length), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(width, height, length), color),
		VertexFormat(corner + glm::vec3(0, height, length), color),

	};

	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1, 4,   5, 4, 1,
										   0, 3, 4,   7, 4, 3,
										   0, 3, 1,   2, 1, 3,
										   6, 1, 7,   4, 3, 1,
	                                       6, 7, 2,   3, 2, 7,
	                                       6, 5, 2,   1, 2, 5};

	if (fill != false) {
		platform->SetDrawMode(GL_TRIANGLES);
	}

	platform->InitFromData(vertices, indices);
	return platform;
}

Mesh* Obiecte3D::CreateRectangular(std::string name, glm::vec3 center, float length, float width, glm::vec3 color, bool fill)
{
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(-length, 0, 0), color),
		VertexFormat(corner + glm::vec3(-length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color),
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
											3, 2, 0};

	if (fill != false) {
		rectangle->SetDrawMode(GL_TRIANGLES);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}
