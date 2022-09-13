#include "BMPParser.h"
#include "EZGL/FPSCounter.h"
#include "EZGL/FlyController.h"
#include "EZGL/Math/Matrix.h"
#include "EZGL/Math/Vector.h"
#include "EZGL/Mesh.h"
#include "EZGL/PerspectiveCamera.h"
#include "EZGL/Shader.h"
#include "EZGL/VertexBuffer.h"
#include "EZGL/Window.h"
#include "Logger.h"
#include "ModelController.h"
#include "ObjParser/ObjParser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <memory>

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

void initOpenGl(ezgl::PerspectiveCamera& camera)
{
	try
	{
		Logger::log("Init", "Initializing OpenGL...");
		if (!glfwInit())
		{
			Logger::log("Init", "OpenGL initialization failed: glfwInit error", Logger::MessageType::Error);
			exit(1);
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
		exit(1);
	}
}

ezgl::Texture generateTexture(const std::string& path)
{
	scop::BMPParser textureImg;

	try
	{
		Logger::log("Init", "Loading texture \"" + path + "\"...");
		textureImg.loadFile(path);
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("BMP Parsing failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		exit(1);
	}

	Logger::log("Init", "Texture loaded", Logger::MessageType::Success);

	return textureImg.createTexture();
}

ezgl::Mesh generateModel(const std::string& path)
{
	scop::ObjParser obj;
	ezgl::Mesh model;

	try
	{
		obj.loadFile(path);
		Logger::log("ModelLoader", "Creating the model's mesh...");
		model = obj.createMesh();
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("Model parsing failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		exit(1);
	}

	Logger::log("ModelLoader", std::to_string(model.getPositions().size() / 3) + " triangles loaded!",
				Logger::MessageType::Success);

	return model;
}

ezgl::Shader generateShader()
{
	ezgl::Shader shader;
	try
	{
		Logger::log("Init", "Loading shaders...");
		shader.loadFiles("shaders/Model.vert", "shaders/Model.frag");
		Logger::log("Init", "Shaders loaded successfully...", Logger::MessageType::Success);
	}
	catch (std::exception e)
	{
		Logger::log("Init", std::string("Shader loading failed: ") + e.what(), Logger::MessageType::Error);
		glfwTerminate();
		exit(1);
	}

	return shader;
}

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	if (argc != 3)
	{
		Logger::log("Init", "Usage: ./scop <model_path> <texture_path>", Logger::MessageType::Error);
		return 1;
	}

	ezgl::PerspectiveCamera camera(70, 0.1f, 300);
	ezgl::FlyController flyController(camera);

	ezgl::Material material(ezgl::Vector3<float>(1, 1, 1), 0.1f, 0.5f, 0.6f, 50.0f);
	ezgl::Light light(ezgl::Vector3<float>(-1.0f, -1.0f, -1.0f).normalize(), ezgl::Vector3<float>(1, 1, 1), 1);

	initOpenGl(camera);

	ezgl::Mesh model = generateModel(argv[1]);
	ezgl::Texture texture = generateTexture(argv[2]);
	ezgl::Shader shader = generateShader();

	ezgl::VertexBuffer modelBuffer(shader, model);
	scop::ModelController modelController;
	ezgl::FPSCounter fpsCounter;

	Logger::log("Init", "Starting main loop...");
	while (!ezgl::Window::shouldClose())
	{
		ezgl::Window::clear();
		ezgl::Window::update();
		fpsCounter.update();

		shader.bind();
		texture.bind();
		modelBuffer.bind();

		shader.sendUniform("u_mvp", camera.getMVP());
		shader.sendUniform("u_cameraPos", camera.getPosition());
		shader.sendUniform("u_light", light);
		shader.sendUniform("u_material", material);
		shader.sendUniform("u_rotation", modelController.getRotationMatrix());
		shader.sendUniform("u_translation", modelController.getTranslationMatrix());
		shader.sendUniform("u_centerTranslation", model.getCenterTranslationMatrix());
		shader.sendUniform("u_transitionState", modelController.getTransitionValue());

		modelBuffer.draw();

		modelBuffer.unbind();
		texture.unbind();
		shader.unbind();

		ezgl::Window::display();
		ezgl::Window::pollEvents();
	}

	glfwTerminate();
	return 0;
}
