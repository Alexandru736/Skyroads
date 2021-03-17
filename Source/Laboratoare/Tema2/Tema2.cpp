#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <Core/Engine.h>

#include "Obiecte3D.h"

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;
	projectionType = true;

	right = 10.f;
	left = .01f;
	bottom = .01f;
	top = 10.f;
	fov = 40.f;

	fuel = 10;
	scale_factor = 1;
	platform_speed = 0;
	jump_step = 0;

	jump = false;
	up = false;
	onPlatform = true;
	left = false;
	right = false;
	gameOver = false;
	turbo = false; 

	srand(time(0));

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2.5f, -4.5f), glm::vec3(0, 2.5f, 0), glm::vec3(0, 3, 0));

	{
		for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {
			array_of_platforms[i].name = "platform" + to_string(i);
			array_of_platforms[i].color = getRandomColour();

			array_of_platforms[i].object = Obiecte3D::CreatePlatform(array_of_platforms[i].name, glm::vec3(0, 0, 0), PLATFORM_LENGTH, PLATFORM_WIDTH, PLATFORM_HEIGHT, array_of_platforms[i].color, true);
			
			int x_coord, y_coord, z_coord;
			x_coord = INIT_PLATFORM_X - i % 3 * SPACE_BETWEEN_COLUMNS - i % 3 * PLATFORM_WIDTH;
			y_coord = INIT_PLATFORM_Y;
			if(i % 3 == 0)
				z_coord = INIT_PLATFORM_Z + i / 3 * PLATFORM_LENGTH + i / 3 * SPACE_BETWEEN_PLATFORMS + FIRST_COLUMN_OFFSET;
			else if(i % 3 == 1)
				z_coord = INIT_PLATFORM_Z + i / 3 * PLATFORM_LENGTH + i / 3 * SPACE_BETWEEN_PLATFORMS + SECOND_COLUMN_OFFSET;
			else  if(i % 3 == 2)
				z_coord = INIT_PLATFORM_Z + i / 3 * PLATFORM_LENGTH + i / 3 * SPACE_BETWEEN_PLATFORMS + THIRD_COLUMN_OFFSET;

			array_of_platforms[i].center = glm::vec3(x_coord, y_coord, z_coord);

			array_of_platforms[i].current_x = array_of_platforms[i].center.x;
			array_of_platforms[i].current_y = array_of_platforms[i].center.y;
			array_of_platforms[i].current_z = array_of_platforms[i].center.z;
			array_of_platforms[i].modelMatrix = glm::mat4(1);

			AddMeshToList(array_of_platforms[i].object);
		}
	}
	
	//creare sfera
	{
		sphere.object = new Mesh("sphere");
		sphere.object->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");

		sphere.current_x = 0;
		sphere.current_y = SPHERE_LAND_LIMIT;
		sphere.current_z = -1;
		sphere.center = glm::vec3(sphere.current_x, sphere.current_y, sphere.current_z);

		AddMeshToList(sphere.object);
	}

	//shader sfera
	{
		Shader* shader = new Shader("Tema2Shader");
		shader->AddShader("Source/Laboratoare/Tema2/Shader/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shader/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//creare interfata cu utilizatorul
	{
		rectangle1.object = Obiecte3D::CreateRectangular("rect1", glm::vec3(0, 0, 0), FUEL_LENGTH, FUEL_WIDTH, glm::vec3(1, 0.5, 0), true);
		rectangle1.current_x = FUEL_X;
		rectangle1.current_y = FUEL_Y;
		AddMeshToList(rectangle1.object);

		rectangle2.object = Obiecte3D::CreateRectangular("rect2", glm::vec3(0, 0, 0), FUEL_LENGTH, FUEL_WIDTH, glm::vec3(1, 1, 1), true);
		rectangle2.current_x = FUEL_X;
		rectangle2.current_y = FUEL_Y;
		AddMeshToList(rectangle2.object);

	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
}	

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{

	if (!gameOver) {
		{
			sphere.modelMatrix = glm::mat4(1);
			if (isCollision() == -1 && jump == false) { 
				jump_step = -1.1f;
				SphereLand(deltaTimeSeconds); 
			}
			else if (isCollision() != -1) {
				platform_index = isCollision();				
				array_of_platforms[platform_index].object = Obiecte3D::CreatePlatform(array_of_platforms[platform_index].name, 
					glm::vec3(0, 0, 0), PLATFORM_LENGTH, PLATFORM_WIDTH, PLATFORM_HEIGHT, glm::vec3(1, 0, 1), true);
				AddMeshToList(array_of_platforms[platform_index].object);
			}
			if (jump == true) { SphereJump(deltaTimeSeconds); }

			sphere.modelMatrix = glm::translate(sphere.modelMatrix, glm::vec3(sphere.current_x, sphere.current_y, sphere.current_z));

			RenderMesh(meshes["sphere"], shaders["Tema2Shader"], sphere.modelMatrix);
		}

		{
			rectangle1.modelMatrix = glm::mat4(1);
			rectangle1.modelMatrix = glm::translate(rectangle1.modelMatrix, glm::vec3(rectangle1.current_x, rectangle1.current_y, 3.0f));
			rectangle1.modelMatrix = glm::scale(rectangle1.modelMatrix, glm::vec3(scale_factor, 1, 1));

			RenderMesh(meshes["rect1"], shaders["VertexColor"], rectangle1.modelMatrix);

			rectangle2.modelMatrix = glm::mat4(1);
			rectangle2.modelMatrix = glm::translate(rectangle2.modelMatrix, glm::vec3(rectangle2.current_x, rectangle2.current_y, 3.01f));

			RenderMesh(meshes["rect2"], shaders["VertexColor"], rectangle2.modelMatrix);
		}

		{
			for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {

				// x and y remain constant; only z is constantly changing
				array_of_platforms[i].current_z -= deltaTimeSeconds * PLATFORM_SPEED * platform_speed * 0.5;
				decreasing_factor = platform_speed * 0.1;
				scale_factor -= decreasing_factor * 0.001;
				fuel -= decreasing_factor * 0.01;
				if (fuel < 0.1) {
					gameOver = true;
				}
				if (array_of_platforms[i].current_z < -8.0f) {
					array_of_platforms[i].current_z = PLATFORM_RESPAWN;
					array_of_platforms[i].color = getRandomColour();
					array_of_platforms[i].object = Obiecte3D::CreatePlatform(array_of_platforms[i].name, 
						glm::vec3(0, 0, 0), PLATFORM_LENGTH, PLATFORM_WIDTH, PLATFORM_HEIGHT, array_of_platforms[i].color, true);
					AddMeshToList(array_of_platforms[i].object);

				}
				array_of_platforms[i].modelMatrix = glm::mat4(1);
				array_of_platforms[i].modelMatrix = glm::translate(array_of_platforms[i].modelMatrix,
					glm::vec3(array_of_platforms[i].current_x,
						array_of_platforms[i].current_y,
						array_of_platforms[i].current_z));
				RenderMesh(meshes[array_of_platforms[i].name], shaders["VertexColor"],
					array_of_platforms[i].modelMatrix);
			}
		}
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_LEFT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (window->KeyHold(GLFW_KEY_W) && turbo == false) {
			//change platforms' speed
			if (platform_speed < 2)
				platform_speed += 0.1;
		}
		if (window->KeyHold(GLFW_KEY_S) && turbo == false) {
			//change platforms' speed
			if (platform_speed > 1)
				platform_speed -= 0.1;
		}
		if (window->KeyHold(GLFW_KEY_A)) { 
			//move sphere to left
			sphere.current_x += SPHERE_SIDEWAYS_SPEED;
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			//move sphere to right
			sphere.current_x -= SPHERE_SIDEWAYS_SPEED;
		}
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// Translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			// Translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_A))
		{
			// Translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}
		
		if (window->KeyHold(GLFW_KEY_D))
		{
			// Translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_Q))
		{
			// Translate the camera down
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_E))
		{
			// Translate the camera up
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_Y) {
		platform_speed = 0;
	}
	if (key == GLFW_KEY_U) {
		platform_speed = 1;
	}
	if (key == GLFW_KEY_SPACE && onPlatform == true) {
		jump = true;
		up = true;
		onPlatform = false;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0)
		{
			renderCameraTarget = false;
			// Rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// Use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
			camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL)
		{
			renderCameraTarget = true;
			// Rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// Use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
			camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
		}
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

int Tema2::isCollision() {
	for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {
		float x = max(array_of_platforms[i].current_x, min(sphere.current_x, array_of_platforms[i].current_x + PLATFORM_WIDTH));
		float y = max(array_of_platforms[i].current_y, min(sphere.current_y, array_of_platforms[i].current_y + PLATFORM_HEIGHT));
		float z = max(array_of_platforms[i].current_z, min(sphere.current_z, array_of_platforms[i].current_z + PLATFORM_LENGTH));

		float distance = sqrt(
			(x - sphere.current_x) * (x - sphere.current_x) + 
			(y - sphere.current_y) * (y - sphere.current_y) + 
			(z - sphere.current_z) * (z - sphere.current_z));
		if (distance <= SPHERE_RADIUS)
			return i;
	}
	return -1;
}

void Tema2::SphereJump(float deltatimeseconds) {
	if (jump == true) {
		if (up == true) { // sphere goes up
			if (sphere.current_y + SPHERE_RADIUS <= SPHERE_JUMP_LIMIT) {
				jump_step += deltatimeseconds * JUMPING_RATIO;
				sphere.current_y += jump_step;
			}
			else {
				up = false;
				jump_step = 0;
			}
		}
		else { // sphere goes down
			SphereLand(deltatimeseconds);
		}
	}
}

void Tema2::SphereLand(float deltatimeseconds) {
	if (sphere.current_y > SPHERE_LAND_LIMIT && jump == true) {
		jump_step -= deltatimeseconds * JUMPING_RATIO;
		sphere.current_y += jump_step;
	}
	else {
		platform_index = isCollision();
		if (platform_index != -1 && jump == true) {
			jump = false;
			onPlatform = true;
			jump_step = 0;
			
			//POWER-UPS
			if (array_of_platforms[platform_index].color == glm::vec3(1, 0, 0)) // rosu
				gameOver = true;
			else if (array_of_platforms[platform_index].color == glm::vec3(1, 1, 0)) {// galben
				scale_factor -= 0.1;
				fuel -= 1;
			}
			else if (array_of_platforms[platform_index].color == glm::vec3(1, 0.5, 0)) { //portocaliu
				turbo = true;
				Turbo();
			}
			else if (array_of_platforms[platform_index].color == glm::vec3(0, 1, 0)) { //verde
				scale_factor += 0.1;
				fuel += 1;
			}

		}
		else {
			if (sphere.current_y >= -3)
				sphere.current_y += jump_step * deltatimeseconds;
			else
				gameOver = true;
		}
	}
}

glm::vec3 Tema2::getRandomColour() {
	int i = rand() % 5 + 1;
	if (i == 1) // red
		return glm::vec3(1, 0, 0);
	else if (i == 2) // galben
		return glm::vec3(1, 1, 0);
	else if (i == 3) // portocaliu
		return glm::vec3(1, 0.5, 0);
	else if (i == 4) // verde
		return glm::vec3(0, 1, 0);
	else if (i == 5) // albastru(normal)
		return glm::vec3(0, 0, 1);
	return glm::vec3(1, 0, 1);
}

void Tema2::Turbo() {
	if (platform_speed < 100)
		platform_speed += 10;
	else {
		turbo = false;
		platform_speed = 1.5;
	}
}