/****************************************************
 * 2016-2026 Eric Bachmann, Mike Zmuda, Norm Krumpe
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted..
 ****************************************************/

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

using namespace std::chrono;

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT); 
RayTracer rayTrace(paleGreen);
IScene scene;

dvec3 cameraPos(8.5, 5, 21);
dvec3 cameraFocus(8.5, 10, 0);
dvec3 cameraUp = Y_AXIS;
double cameraFOV = PI_2;

void buildScene() {
	// After you implement the needed code, you can
	// change these to IClosedConeY:
	IShape* coneY = new IConeY(dvec3(0.0, 12.0, 0.0), 5.0, 6.0);
	IShape* coneY2 = new IConeY(dvec3(17.0, 12.0, 0.0), 5.0, 6.0);

	// sample bowl:
	IShape* bowl = new IBowl(dvec3(0, 0, 0), 5);

	//scene.addOpaqueObject(new VisibleIShape(coneY, redPlastic));
	//scene.addOpaqueObject(new VisibleIShape(coneY2, greenPlastic));
	scene.addOpaqueObject(new VisibleIShape(bowl, greenPlastic));

	scene.addLight(new PositionalLight(dvec3(20, 0, 0), white)); // changed x to 20 from -3
	scene.addLight(new PositionalLight(dvec3(20, 0, 0), white));
	scene.addLight(new PositionalLight(dvec3(0, 50, 0), white));
	scene.addLight(new PositionalLight(ORIGIN3D, white));
}

void render(GLFWwindow* window) {
	milliseconds frameStartTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);
	
	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();

	scene.camera = new PerspectiveCamera(cameraPos, cameraFocus, cameraUp, PI / 2, width, height);
	rayTrace.raytraceScene(frameBuffer, 0, scene);
	frameBuffer.showColorBuffer(); // <-- !!! This line was missing here, but WAS here in FirstRayTraceScene's render(GLFWwindow* window)!!!

	milliseconds frameEndTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);
	milliseconds totalTime = frameEndTime - frameStartTime;
	cout << "Render time: " << totalTime.count() << " ms." << endl;


}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_A:
		cameraPos.x++;
		break;
	case GLFW_KEY_D:
		cameraPos.x--;
		break;
	case GLFW_KEY_W:
		cameraPos.z--;
		break;
	case GLFW_KEY_S:
		cameraPos.z++;
		break;
	case GLFW_KEY_J:
		cameraPos.y++;
		break;
	case GLFW_KEY_M:
		cameraPos.y--;
		break;
	case GLFW_KEY_ESCAPE:
		exit(0);
		break;
	default:
		cout << (int)key << "unmapped key pressed." << endl;
	}
}

int main(int argc, char* argv[]) {
	buildScene();
	initGraphics(WINDOW_WIDTH, WINDOW_HEIGHT, username.c_str(), render, mouseUtility, keyboard, nullptr);
	return 0;
}