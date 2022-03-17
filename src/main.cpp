#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <ctime>
#include "../inc/Logger.h"
#include "../inc/ObjParser/ObjParser.h"
#include "../inc/EZGL/Mesh.h"
#include "../inc/EZGL/VertexBuffer.h"
#include "../inc/EZGL/Math/Matrix.h"
#include "../inc/EZGL/Math/Vector.h"
#include "../inc/EZGL/Shader.h"
#include "../inc/EZGL/PerspectiveCamera.h"
#include "../inc/EZGL/FlyController.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "../inc/EZGL/Window.h"
#include "../inc/ModelController.h"
#include "../inc/BMPParser.h"

void updateViewport(GLFWwindow* window, int width, int height)
{
	ezgl::Window::setWidth(width);
	ezgl::Window::setHeight(height);
	ezgl::Window::updateCameraProjection();
	glViewport(0, 0, width, height);
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(-1.f, 1.f);
}

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	ezgl::PerspectiveCamera camera(70, 0.1f, 300);
	ezgl::FlyController flyController(camera);

	ezgl::Material material(ezgl::Vector3<float>(1, 1, 1), 0.1f, 0.5f, 0.6f, 50.0f);
	ezgl::Light light(ezgl::Vector3<float>(-1.0f, -1.0f, -1.0f).normalize(), ezgl::Vector3<float>(1, 1, 1), 1);

	try
	{
		Logger::log("Init", "Initializing OpenGL...");
		if (!glfwInit())
		{
			Logger::log("Init", "OpenGL initialization failed: glfwInit error", Logger::MessageType::Error);
			return 1;
		}

		ezgl::Window::create(800, 800, "scop", camera);
		updateViewport(nullptr, ezgl::Window::getWidth(), ezgl::Window::getHeight());

		glewInit();

		glfwSetWindowSizeCallback(ezgl::Window::getPtr(), updateViewport);

		Logger::log("Init", "OpenGL initialized!", Logger::MessageType::Success);
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("OpenGL initialization failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		return 1;
	}

	scop::BMPParser textureImg;

	try
	{
		textureImg.loadFile("../../resources/textures/concrete.bmp");
		Logger::log("Init", "Texture loaded", Logger::MessageType::Success);
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("BMP Parsing failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		return 1;
	}

	ezgl::Shader shader;
	try
	{
		Logger::log("Init", "Loading shaders...");
		shader.loadFiles("../../resources/shaders/Model.vert", "../../resources/shaders/Model.frag");
		Logger::log("Init", "Shaders loaded successfully...", Logger::MessageType::Success);
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("Shader loading failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		return 1;
	}

	scop::ObjParser obj;
	ezgl::Mesh model;
	try
	{
		obj.loadFile("../../resources/models/teapot.obj");
		Logger::log("ModelLoader", "Creating the model's mesh...");
		model = obj.createMesh();
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("Model parsing failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		return 1;
	}
	
	ezgl::VertexBuffer modelBuffer(shader, model);
	scop::ModelController modelController;

	Logger::log("ModelLoader", std::to_string(model.getPositions().size() / 3) + " triangles loaded!", Logger::MessageType::Success);

	while (!ezgl::Window::shouldClose())
	{
		ezgl::Window::clear();

		ezgl::Window::update();

		shader.bind();
		modelBuffer.bind();
		shader.sendUniform("u_mvp", camera.getMVP());
		shader.sendUniform("u_cameraPos", camera.getPosition());
		shader.sendUniform("u_light", light);
		shader.sendUniform("u_material", material);
		shader.sendUniform("u_rotation", modelController.getRotationMatrix());

		ezgl::Vector3<float> pos(50, 0, 0);

		ezgl::Matrix<float, 4, 4> translation;
		translation.at(0, 0) = 1;
		translation.at(1, 1) = 1;
		translation.at(2, 2) = 1;
		translation.at(3, 3) = 1;
		translation.at(3, 0) = pos.x;
		translation.at(3, 1) = pos.y;
		translation.at(3, 2) = pos.z;

		shader.sendUniform("u_translation", translation);

		modelBuffer.draw();
		modelBuffer.unbind();
		shader.unbind();

		ezgl::Window::display();
		ezgl::Window::pollEvents();
	}

	glfwTerminate();
	return 0;
}
