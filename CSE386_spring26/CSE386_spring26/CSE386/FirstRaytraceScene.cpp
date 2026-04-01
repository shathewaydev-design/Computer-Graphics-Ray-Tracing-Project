/****************************************************
 * 2016-2026 Eric Bachmann and Mike Zmuda and Norm Krumpe
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include <ctime>
#include <chrono>
#include "defs.h"
#include "io.h"
#include "ishape.h"
#include "framebuffer.h"
#include "raytracer.h"
#include "iscene.h"
#include "light.h"
#include "image.h"
#include "camera.h"
#include "rasterization.h"

using namespace std::chrono; // for timing

// this is where the raytracing algorothm lives, in
// raytraceScene() function
// that function needs  scene (shapes, camera, lights)
// a frameBuffer for the pixels
RayTracer rayTracer(paleGreen);

// for the pixels
FrameBuffer frameBuffer(300, 300); // want speed? make these smaller! Prety image? Make bigger.

// for the shapes, camera, lights
IScene scene;

void buildScene() {

	/*IEllipsoid* e = new IEllipsoid(dvec3(0, 0, 0), dvec3(15, 5, 10));
	scene.addOpaqueObject(new VisibleIShape(e, polishedGold));*/

	// make a plane to represent the "floor" that the spheres sit on:
	// (won't see until we implement findClosestIntersection() for IPlane)
	//IPlane* myFloor = new IPlane(dvec3(0, -2, 0), dvec3(0, 1, 0));
	//scene.addOpaqueObject(new VisibleIShape(myFloor, mysteryPlastic));

	// circular backdrop for my spheres:
	//IDisk* myDisk = new IDisk(dvec3(0, 0, -20), dvec3(0, 0, 1), 10);
	//scene.addOpaqueObject(new VisibleIShape(myDisk, greenPlastic));



	// add one or more IShapes to the scene as VisibleIShapes
	// where we specify the material the shape is made of
	//ISphere* sphere1 = new ISphere(dvec3(0.0, 1.0, 0.0), 2.0);
	//ISphere* sphere2 = new ISphere(dvec3(-12.0, 0.0, 0.0), 2.0);


	// see colorandmaterials.h for some possible material choices
	// Visible ISHape contains a regular IShape plus material and texture
	//scene.addOpaqueObject(new VisibleIShape(sphere2, cyanPlastic));
	//scene.addOpaqueObject(new VisibleIShape(sphere2, greenPlastic));

	//ICylinderY* cylTest = new ICylinderY(dvec3(0.0, 3.0, 7.0), 3.0, 6.0);
	//scene.addOpaqueObject(new VisibleIShape(cylTest, chrome));

	IConeY* conTest = new IConeY(dvec3(10.0, 8.0, 1.0), 3.0, 6.0);
	scene.addOpaqueObject(new VisibleIShape(conTest, chrome));




	//double posx = 8.0;
	//double posz = 8.0;
	//int currColor = 0;
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 5; j++) {
	//		ISphere* sphereTest = new ISphere(dvec3(posx, 0.0, posz), 2.0);
	//		if (currColor == 0) {
	//			scene.addOpaqueObject(new VisibleIShape(sphereTest, cyanPlastic));
	//			currColor = 1;
	//		}
	//		else if (currColor == 1) {
	//			scene.addOpaqueObject(new VisibleIShape(sphereTest, chrome));
	//			currColor = 0;
	//		}
	//		posx -= (2.0 * 2);

	//	}
	//	posz -= (2.0 * 2);
	//	posx = 8.0;

	//}



	// add a camera to the scene
	dvec3 cameraPos(10, 18, 5);
	dvec3 cameraFocus(10, 0, 1);
	dvec3 cameraUp = Y_AXIS;
	double cameraFOV = PI_2;
	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();		

	scene.camera = new PerspectiveCamera(cameraPos, cameraFocus, cameraUp, cameraFOV, width, height);
	//scene.camera = new OrthographicCamera(cameraPos, cameraFocus, cameraUp, width, height, 0.04);

	cout << scene.camera->getFrame() << endl;
	
	// add one or more lights to the scene
	PositionalLightPtr posLight = new PositionalLight(dvec3(-10, 10, 0), white);
	scene.addLight(posLight);
	
}

void render(GLFWwindow* window) {
	// *** time keeping
	milliseconds frameStartTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	frameBuffer.clearColorBuffer();

	// the zero below is "depth" - 0 means no reflections, and 
	// increasing gives richer reflections
	rayTracer.raytraceScene(frameBuffer, 0, scene);
	frameBuffer.showColorBuffer();

	// *** time keeping
	milliseconds frameEndTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	milliseconds totalTime = frameEndTime - frameStartTime;
	cout << "Render time: " << totalTime.count() << " ms." << endl;
}

void checkEm(const char* name, const IShape& shape) {
	Ray ray1(dvec3(0, 0, 0), glm::normalize(dvec3(0, 0.5, -1)));	// directions will be normalized
	Ray ray2(dvec3(0, 0, 0), glm::normalize(dvec3(0, 0, -1)));
	Ray ray3(dvec3(0, 0, 0), dvec3(0, -0.5, -1));

	HitRecord hit1;
	HitRecord hit2;
	HitRecord hit3;

	shape.findClosestIntersection(ray1, hit1);
	shape.findClosestIntersection(ray2, hit2);
	shape.findClosestIntersection(ray3, hit3);

	cout << name << endl;
	cout << "==============" << endl;
	cout << hit1.t << ' ' << hit1.interceptPt << ' ' << hit1.normal << endl;
	cout << hit2.t << ' ' << hit2.interceptPt << ' ' << hit2.normal << endl;
	cout << hit3.t << ' ' << hit3.interceptPt << ' ' << hit3.normal << endl;
	cout << endl;
}

int fakemain() {
	checkEm("Plane", IPlane(dvec3(0, -1, 0), dvec3(0, 1, 0)));
	checkEm("Disk1", IDisk(dvec3(0, 0, -1), dvec3(0, 0, 1), 1.0));
	checkEm("Disk2", IDisk(dvec3(0, 0, -10), dvec3(0, 0, 1), 1.0));

	return 0;
}

/*
Plane
==============
3.40282e+38[0 0 0][0 0 0]
3.40282e+38[0 0 0][0 0 0]
2.23607[0 -1 -2][0 1 0]

Disk1
==============
1.11803[0 0.5 -1][0 0 1]
1[0 0 - 1][0 0 1]
1.11803[0 -0.5 -1][0 0 1]

Disk2
==============
3.40282e+38[0 5 -10][0 0 1]
10[0 0 -10][0 0 1]
3.40282e+38[0 -5 -10][0 0 1]
*/



// FIRST OG MAIN, DON'T DELETE

int main(int argc, char* argv[]) {
	buildScene();
	frameBuffer.setClearColor(rayTracer.defaultColor);
	initGraphics(frameBuffer.getWindowWidth(), frameBuffer.getWindowHeight(), 
				__FILE__, render, mouseUtility, keyboardUtility, nullptr);

	return 0;
}