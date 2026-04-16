/****************************************************
 * 2016-2026 Eric Bachmann and Mike Zmuda and Norm Krumpe
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include <ctime>
#include <iostream>
#include "defs.h"
#include "colorandmaterials.h"
#include "framebuffer.h"
#include "utilities.h"
#include "rasterization.h"
#include "io.h"

const int SZ = 500;
FrameBuffer frameBuffer(SZ, SZ);
vector<dvec3> poly;

// returns a new polygon, transformed by the provided matrix
vector<dvec3> transformPoly(vector<dvec3> verts, const dmat3& mat) {
	vector<dvec3> result;
	return result;
}

// draws polygon shifted because we want (0, 0) in the center
void drawPoly(vector<dvec3> verts, const color& C) {
	for (unsigned int i = 0; i < verts.size(); i++) {
		verts[i].x += SZ/2;
		verts[i].y += SZ/2;
	}
	// implemented in rasterization.cpp
	drawWirePolygon(frameBuffer, verts, C);
}

// called repeatedly
void render(GLFWwindow* window) {
	frameBuffer.clearColorAndDepthBuffers();
	drawAxisOnWindow(frameBuffer);

	// we will write code here

	drawPoly(poly, red);

	frameBuffer.showColorBuffer();
}


int main(int argc, char* argv[]) {


	// matrix from slide 5:
	dmat3 m(1, 4, 7, 2, 5, 8, 3, 6, 9);
	//cout << m << endl;
	//cout << glm::transpose(m) << endl;
	//cout << m[0][0] << endl;

	dmat3 i(1.0);
	//cout << i << endl;


	// think of this as a column matrix (3 x 1, 1 column)
	dvec3 pt(3, 7, 1);


	// here is a transformation matrix (3x3 matrix designed to translate, scale, or rotate)
	//dmat3 t(0.5, 0, 0, 0, 1.5, 0, 0, 0, 1); // << equivalent to s(0.5, 1.5)
	
	
	
	// equivalent to t(2, -1)
	//dmat3 t(1, 0, 0, 0, 1, 0, 2, -1, -1);

	dmat3 t(1, 0, 0, 0, 1, 0, 0, 0, 1);

	cout << t << endl;
	cout << t * pt << endl;
	







	// make a shape. drawPoly() will automatically draw it
	// 3 vertices of a polygon, using homogeneous coordinates
	poly.push_back(dvec3(0, 0, 1));
	poly.push_back(dvec3(0, 100, 1));
	poly.push_back(dvec3(200, 100, 1));
	poly.push_back(dvec3(200, 0, 1));

	//for (int i = 0; i < poly.size(); i++) {
	//	poly[i] = t * poly[i];
	//}

	cout << poly << endl;

	frameBuffer.setClearColor(white);
	initGraphics(SZ, SZ, __FILE__, render, nullptr, nullptr, nullptr);
	return 0;
}