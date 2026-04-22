/****************************************************
 * 2016-2026 Eric Bachmann and Mike Zmuda and Norm Krumpe
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include <iostream>
#include "defs.h"
#include "framebuffer.h"
#include "utilities.h"
#include "rasterization.h"
#include "io.h"

const int WINDOW_SZ = 500;
FrameBuffer frameBuffer(WINDOW_SZ, WINDOW_SZ);
const int N = 50;

vector<dvec3> triangleVertices = { dvec3(-2 * N,2 * N,1), dvec3(-N,2 * N,1), dvec3(-1.5 * N,3 * N,1) };
vector<dvec3> square1Vertices = { dvec3(-N,-N,1), dvec3(N,-N,1),
											dvec3(N,N,1), dvec3(-N,N,1) };
vector<dvec3> square2Vertices = { dvec3(3 * N,-2 * N,1), dvec3(3 * N,-3 * N,1),
											dvec3(2 * N,-3 * N,1), dvec3(2 * N,-2 * N,1) };

int displayedProblem = 0;

vector<dvec3> transformVertices(const dmat3& transMatrix, const vector<dvec3>& vertices) {
	vector<dvec3> transformedVertices;

	for (size_t i = 0; i < vertices.size(); i++) {
		dvec3 vt(transMatrix * vertices[i]); // multiply a matrix by a point to get a point 
		transformedVertices.push_back(vt);
	}

	return transformedVertices;
}

void drawWirePolygonWithShift(vector<dvec3> verts, const color& C) {
	int W2 = frameBuffer.getWindowWidth() / 2;
	int H2 = frameBuffer.getWindowHeight() / 2;
	for (unsigned int i = 0; i < verts.size(); i++) {
		verts[i].x += W2;
		verts[i].y += H2;
	}
	drawWirePolygon(frameBuffer, verts, C);
}

void drawObjectOnly(const vector<dvec3>& verts, bool drawAxis = true) {
	if (drawAxis) {
		drawAxisOnWindow(frameBuffer);
	}
	drawWirePolygonWithShift(verts, black);
}

void drawObjAndOneTransformation(const dmat3& TM, const vector<dvec3>& verts, bool drawAxis = true) {
	vector<dvec3> vertsTransformed = transformVertices(TM, verts);
	if (drawAxis) {
		drawAxisOnWindow(frameBuffer);
	}
	drawWirePolygonWithShift(verts, black);
	drawWirePolygonWithShift(vertsTransformed, red);
}

void drawObjectAndAllTransformations(const dmat3& TM) {
	drawObjAndOneTransformation(TM, triangleVertices, false);
	drawObjAndOneTransformation(TM, square1Vertices, false);
	drawObjAndOneTransformation(TM, square2Vertices, false);
	drawAxisOnWindow(frameBuffer);
}

// 0. Draw all the shapes, transformed by S(2, 0.5)
void doScaleBy2xOneHalf() {
	//dmat3 TM(2, 0, 0, 0, 0.5, 0, 0, 0, 1);
	dmat3 TM = S(2, 0.5);

	drawObjectAndAllTransformations(TM);
}

// 1. Draw all the shapes, translated by 50 in both the x- and y-directions
void doTranslate50_50() {
	//dmat3 TM(1, 0, 0, 0, 1, 0, 50, 50, 1);
	//static double dx = 0; // static local variable, so it's value is maintained between calls
	dmat3 TM = T(50, 50);
	//dx += 0.1;

	drawObjectAndAllTransformations(TM);
}

// 2. Draw all the shapes, rotated 45 degrees about origin
void doRotate45Degrees() {
	//double n = glm::cos(glm::radians(45.0));
	//dmat3 TM(n, n, 0, -n, n, 0, 0, 0, 1);
	dmat3 TM = R(glm::radians(45.0));
	drawObjectAndAllTransformations(TM);
}

// 3. Draw all shapes, reflected across the Y axis
void doReflectAcrossYaxis() {
	dmat3 TM(-1, 0, 0, 0, 1, 0, 0, 0, 1);
	drawObjectAndAllTransformations(TM);
}

// 4. Draw all shapes, rotated 180 degrees about the origin
void doReflectAcrossOrigin() {
	dmat3 TM = R(glm::radians(180.0));
	drawObjectAndAllTransformations(TM);
}

// 5. Draw all shapes, first rotated by 45 degrees, then scaled by 1.5 in x-direction
void doRotateThenScale() {
	dmat3 TM = S(1.5, 1) * R(glm::radians(45.0));
	//dmat3 RM = R(glm::radians(45.0));
	//dmat3 F = TM * RM;
	drawObjectAndAllTransformations(TM);
}

// 6. Draw all shapes, first scaled by 1.5 in x-direction, then rotated by 45
void doScaleThenRotate() {
	dmat3 TM = R(glm::radians(45.0)) * S(1.5, 1);
	drawObjectAndAllTransformations(TM);
}

// 7. Draw all shapes, reflected across y=x
void doReflectAcrossLineYeqX() {
	dmat3 TM;
	dmat3 TT(1, 0, 0, 0, -1, 0, 0, 0, 1);
	dmat3 RM = R(glm::radians(90.0));
	//dmat3 FM(1, 0, 0, 0, 1, 0, 0, 0, 1);
	TM = RM * TT;
	drawObjectAndAllTransformations(TM);
}

// 8. Render square1 so that it rotates about its own center
//    and slowly shrinks. Once it disappears, you can either let it stay
//    disappeared, or you can let it grow again.
//    Animation should be slow enough that it's easy to watch.
void doRotatingPlanet() {
	static double angleInDegs = 0;
	angleInDegs += 0.1;
	double angleInRads = glm::radians(angleInDegs);
	static double scale = 1;
	
	dmat3 TM = S(scale, scale) * R(angleInRads);

	scale -= 0.001;

	vector<dvec3> square1VerticesTransformed = transformVertices(TM, square1Vertices);
	drawWirePolygonWithShift(square1VerticesTransformed, red);
	drawAxisOnWindow(frameBuffer);
}

// 9. Render square1 so that it rotates about its own axis, and then orbits the origin.
//    Rotation about the origin should be slower than rotation about its own axis, and
//    both should be slow enough that it's easy to watch.
void doRotatingAndOrbitingPlanet() {
	static double angleInDegs = 0;
	angleInDegs += 0.25;
	double angleInRads = glm::radians(angleInDegs);
	static double trans = 5;
	dmat3 TM = T(trans, trans) * R(angleInRads);

	trans -= 1;

	vector<dvec3> square1VerticesTransformed = transformVertices(TM, square1Vertices);
	drawWirePolygonWithShift(square1VerticesTransformed, red);
	drawAxisOnWindow(frameBuffer);
}

typedef void(*TRANS)();

struct DisplayFunc {
	TRANS func;
	string name;
	DisplayFunc(TRANS t, string n) : func(t), name(n) {}
};

vector<DisplayFunc> funcs = {
								DisplayFunc(doScaleBy2xOneHalf, "0. Scale by 2 and 1/2"),
								DisplayFunc(doTranslate50_50, "1. Trans by 50 in x and y"),
								DisplayFunc(doRotate45Degrees, "2. Rotate 45"),
								DisplayFunc(doReflectAcrossYaxis, "3. Reflect across Y"),
								DisplayFunc(doReflectAcrossOrigin, "4. Rotate 180"),
								DisplayFunc(doRotateThenScale, "5. Rotate 45, then scale (1.5, 1)"),
								DisplayFunc(doScaleThenRotate, "6. Scale (1.5, 1), then Rotate 45"),
								DisplayFunc(doReflectAcrossLineYeqX, "7. Reflect across y=x"),
								DisplayFunc(doRotatingPlanet, "8. Rotating planet"),
								DisplayFunc(doRotatingAndOrbitingPlanet, "9. Rotating and orbiting planet"),
};

GLFWwindow* window = nullptr;

void render(GLFWwindow* window) {
	if (window != nullptr) {
		glfwSetWindowTitle(window, funcs[displayedProblem].name.c_str());
	}
	frameBuffer.clearColorAndDepthBuffers();
	(*funcs[displayedProblem].func)();				// call the correct function
	frameBuffer.showColorBuffer();
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)
		return;

	if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
		displayedProblem = key - GLFW_KEY_0;
	}
	else if (key == GLFW_KEY_ESCAPE) {
		exit(0);
	}
}

int main(int argc, char* argv[]) {
	for (int i = 0; i <= 9; i++) {
		cout << funcs[i].name << endl;
	}
	frameBuffer.setClearColor(white);
	initGraphics(WINDOW_SZ, WINDOW_SZ, __FILE__, render, nullptr, keyboard, nullptr);

	return 0;
}