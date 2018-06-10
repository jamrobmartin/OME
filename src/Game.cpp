#include "Game.h"

#include <time.h>

namespace OM
{
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLuint vertexCount = 0;
	GLuint verticesPerPrimitive = 0;
	GLuint stride = 0;
	GLuint shader_programme;

	float angle = 0.0f;

	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"out vec4 position;"
		"void main() {"
		"  gl_Position = vec4(vp, 1.0);"
		"  position = vec4(vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"in vec4 position;"
		"void main() {"
		"  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
		"if (position.x < 0.48 && position.x > -0.48 &&  position.y < 0.48 && position.y > -0.48 && position.z < 0.48 && position.z > -0.48)"
		"{"
		"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}"
		"}";

	Game::Game()
	{
		myGameInitParameters.title = "title";
		myGameInitParameters.x = 1;
		myGameInitParameters.y = 1;
		myGameInitParameters.width = 400;
		myGameInitParameters.height = 400;
		myGameInitParameters.fullscreen = false;
	}

	Game::~Game()
	{
	}

	int Game::Init(int argc, char **argv)
	{
		int errorCode = 0;

		glewExperimental = GL_TRUE;

		glutInit(&argc, argv);
		glutInitWindowPosition(myGameInitParameters.x,myGameInitParameters.y);
		glutInitWindowSize(myGameInitParameters.width, myGameInitParameters.height);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow(myGameInitParameters.title.c_str());
		if (myGameInitParameters.fullscreen)
		{
			glutFullScreen();
		}

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		const GLubyte* version = glGetString(GL_VERSION); // version as a string
		printf("Renderer: %s\n", renderer);
		printf("OpenGL version supported %s\n", version);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		glutDisplayFunc(OM::renderScene);
		glutIdleFunc(OM::renderScene);
		glutReshapeFunc(OM::changeSize);

		glutKeyboardFunc(keyDown);

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader, NULL);
		glCompileShader(fs);

		shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);

		GLfloat vertexData[] = {

			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

		};

		GLuint indexData[] = {
			// Front
			0, 1, 3,
			1, 2, 3,
			// Right
			1, 5, 2,
			5, 6, 2,
			// Left
			4, 7, 0,
			0, 7, 5,
			// Top
			0, 4, 5,
			0, 5, 1,
			// Bottom
			3, 7, 2,
			2, 7, 6,
			// Back
			4, 5, 6,
			4, 6, 7
		};
		
		//GLuint 
		vbo = 0;
		vao = 0;

		int primitives = 6 * 2;
		verticesPerPrimitive = 3;
		stride = verticesPerPrimitive * sizeof(float);
		vertexCount = verticesPerPrimitive * primitives;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), 
			vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData),
			indexData, GL_STATIC_DRAW);

		glVertexAttribPointer(0, verticesPerPrimitive, GL_FLOAT, GL_FALSE,
			stride, NULL);
		glEnableVertexAttribArray(0);

		glutMainLoop();

		return errorCode;
	}

	int Game::SetInitParameters(GameInitParameters* gip)
	{
		return 0;
	}

	int Game::Run()
	{
		return 0;
	}

	void renderScene(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		
		gluLookAt(
		0.0f, 1.0f, 20.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
		);
		
		glUseProgram(shader_programme);

		// bind the VAO (the triangle)
		angle += 0.5f;
		glRotatef(angle, -1.0f, 0.0f, 0.0f);
		glBindVertexArray(vao);
		GLenum clearError = glGetError();
		// draw the VAO
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			printf("error: %d: %s\n", err, gluErrorString(err));
		}

		glColor3d(244, 244, 244);

		// unbind the VAO, the program and the texture
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//gProgram->stopUsing();

		// swap the display buffers (displays what was just drawn)
		//glfwSwapBuffers(gWindow);

		glutSwapBuffers();
	}

	void keyDown(unsigned char key, int x, int y)
	{
		if (key == 27)
		{
			glutDestroyWindow(glutGetWindow());
		}
	}

	void changeSize(int w, int h)
	{
		if (h == 0)
			h = 1;
		float ratio = w * 1.f / h;

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		glViewport(0, 0, w, h);

		gluPerspective(45.0f, ratio, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
	}
}