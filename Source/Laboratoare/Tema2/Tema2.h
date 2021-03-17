#pragma once
#include <Component/SimpleScene.h>
#include "Laboratoare\Laborator5\LabCamera.h"
#include <cmath>

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

#define PLATFORM_LENGTH 7.0f
#define PLATFORM_WIDTH 4.0f
#define PLATFORM_HEIGHT 0.25f

#define NUMBER_OF_PLATFORMS 12
#define PLATFORM_SPEED 5

#define INIT_PLATFORM_X 4.0f
#define INIT_PLATFORM_Y 0
#define INIT_PLATFORM_Z 0.0f

#define FIRST_COLUMN_OFFSET 0.0f
#define SECOND_COLUMN_OFFSET -3.0f
#define THIRD_COLUMN_OFFSET 3.0f

#define PLATFORM_RESPAWN 32 

#define SPACE_BETWEEN_COLUMNS 2.5f
#define SPACE_BETWEEN_PLATFORMS 3.0f

#define SPHERE_RADIUS 0.5f
#define SPHERE_LAND_LIMIT 0.75f
#define SPHERE_JUMP_LIMIT 4.5f

#define JUMPING_RATIO 0.4f
#define SPHERE_SIDEWAYS_SPEED 0.1f

#define FUEL_X 4.0f
#define FUEL_Y 4.0f
#define FUEL_WIDTH 0.5f
#define FUEL_LENGTH 2.0f
using namespace std;

class Tema2 : public SimpleScene
	{
	public:
		Tema2();
		~Tema2();

		void Init() override;

		struct Shape {
			Mesh* object;
			glm::vec3 center;
			glm::vec3 color;
			float current_x;
			float current_y;
			float current_z;
			glm::mat4 modelMatrix;
			string name;
		};

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		int isCollision();
		void SphereJump(float deltatimeseconds);
		void SphereLand(float deltatimeseconds);
		glm::vec3 getRandomColour();
		void Turbo();

		/*float max(float a, float b);
		float min(float a, float b);*/

	public:
		Laborator::Camera* camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		bool projectionType;

		GLfloat right;
		GLfloat left;
		GLfloat bottom;
		GLfloat top;
		GLfloat fov;

		bool gameOver;

		Shape platform;
		Shape array_of_platforms[NUMBER_OF_PLATFORMS];
		float platform_speed;
		int platform_index;

		Shape sphere;
		float jump_step;
		bool jump;
		bool up;
		bool onPlatform;
		bool left_side;
		bool right_side;

		Shape rectangle1, rectangle2;
		float fuel;
		float decreasing_factor;
		float scale_factor;
		bool turbo;
	};