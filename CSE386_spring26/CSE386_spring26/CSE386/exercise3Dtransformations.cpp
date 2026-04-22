/****************************************************
 * 2016-2026 Eric Bachmann and Mike Zmuda and Norm Krumpe
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include <iostream>
#include <vector>
#include "eshape.h"
#include "io.h"
#include "light.h"
#include "vertexops.h"

const int W = 800;
const int H = 400;

PositionalLightPtr theLight = new PositionalLight(dvec3(0, 4, 0), yellow);
vector<LightSourcePtr> lights = { theLight };
dvec3 camPosition(1, 2, 2);
FrameBuffer frameBuffer(W, H);

// specify materials with ambient, diffuse, and specular colors, plus shininess
Material allgreen(green, green, green, 1.0);
Material allred(red, red, red, 1.0);

// A shape. We can add more.
EShapeData tri = EShape::createETriangle(allred, dvec4(0, 0, 0, 1), dvec4(1, 0, 0, 1), dvec4(1, 1, 0, 1));
EShapeData square = EShape::createESquare(allgreen);
EShapeData mug = EShape::createEObj("mug.obj");

// save for another time (another course?)
PipelineMatrices pipeMats;
// changing camera location and direction impacts this matrix
dmat4& viewingMatrix = pipeMats.viewingMatrix; 
// these 2 matrices impact projecting the 3d coordinates into 2d coordinates
dmat4& projectionMatrix = pipeMats.projectionMatrix;
dmat4& viewportMatrix = pipeMats.viewportMatrix;

// Some global variables to let us do some animation
bool isMoving = true;
double angle = PI / 4;
double dx = 0.0;

void renderObjects() {
	// framebuffer has pizels to be colored
	// fourth parameter below is the matrix applied to all points in the shape before rendering
	//0.5, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1
	// glm:: has scale, trnaslate, and rotate functions built in
	
	//for (int i = 0; i < 20; i++) {
	//	VertexOps::render(frameBuffer, tri, lights, T(i, 0, -i) * Ry(angle), pipeMats, true);
	//}

	VertexOps::render(frameBuffer, square, lights, Rx(glm::radians(90.0)), pipeMats, true);
	VertexOps::render(frameBuffer, mug, lights, S(8.0, 8.0, 8.0), pipeMats, true);

	dx += 0.01;
	
	angle += 0.01;
	//VertexOps::render(frameBuffer, tri, lights, dmat4(1.0), pipeMats, true);
}

static void render(GLFWwindow* window) {
	frameBuffer.clearColorAndDepthBuffers();
	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();
	double aspectRatio = (double)width / height;

	//camPosition.z -= 0.1;
	viewingMatrix = glm::lookAt(camPosition, ORIGIN3D, Y_AXIS);
	projectionMatrix = glm::perspective(PI_3, aspectRatio, 0.5, 80.0);
	viewportMatrix = VertexOps::getViewportTransformation(0, width, 0, height); 

	renderObjects();

	frameBuffer.showAxes(viewingMatrix, projectionMatrix, viewportMatrix,
		BoundingBoxi(0, width, 0, height));
	frameBuffer.showColorBuffer();
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)
		return;

	switch (key) {
	case GLFW_KEY_P:
		isMoving = !isMoving;
		break;
	case GLFW_KEY_ESCAPE:
		exit(0);
	}
}

int main(int argc, char* argv[]) {
	cout << tri[0].pos << endl;
	cout << tri[0].normal << endl;
	cout << tri[0].material.ambient << endl;
	frameBuffer.setClearColor(paleGreen);
	initGraphics(W, H, "yourname", render, mouseUtility, keyboard, nullptr);
	return 0;
}