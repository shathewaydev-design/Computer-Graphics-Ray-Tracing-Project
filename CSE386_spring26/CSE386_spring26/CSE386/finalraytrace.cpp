/****************************************************
 * 2016-2026 Eric Bachmann and Mike Zmuda and Norm Krumpe
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
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

// You can add more images if you want.
Image im1("usflag.ppm");

// Be careful about making changes to global variables.
// You can (and probably should) make small adjustments to
// some of them in order to help you position lights and
// the camera. However, note that some keyboard functions
// depend on these variables.
int currLight = 0;
double angle = 0.5;
const int MINZ = -10;
const int MAXZ = 4;
double z = MINZ;
double inc = 0.8;
bool isAnimated = false;
int numReflections = 0;
int antiAliasing = 1;

// Direction of spotlight
double spotDirX = 0;
double spotDirY = -1;
double spotDirZ = 0;

// initial camera location and focus. Change as needed
double camX = 5;
double camY = 5;
double camZ = 5;
double cameraFOV = 120.0;
dvec3 cameraFocus = dvec3(0.0, 0.0, 0.0);

dvec3 cameraUp = Y_AXIS;

// keep these two lights (though you may change locations and directions).
// Feel free to add more lights if you want, as long as they do not interfere
// with your being able to show that these two lights are working correctly.
vector<PositionalLightPtr> lights = {
						new PositionalLight(dvec3(0, 15, 0), white), 
						new SpotLight(dvec3(15, 15, 5),
										dvec3(spotDirX,spotDirY,spotDirZ),
										glm::radians(90.0),
										white)
};

PositionalLightPtr posLight = lights[0];
SpotLightPtr spotLight = (SpotLightPtr)lights[1];

// Dimensions of your image. Adjust these as needed.
const int W = 400;
const int H = 300;
FrameBuffer frameBuffer(W, H);

// Change background color as desired
RayTracer rayTrace(paleGreen);
IScene scene;

// add/remove/edit shapes as needed to meet requirements
IPlane* plane = new IPlane(dvec3(0.0, -2.0, 0.0), dvec3(0.0, 1.0, 0.0));
IPlane* clearPlane = new IPlane(dvec3(0.0, 0.0, MINZ), dvec3(0.0, 0.0, 1.0));
ISphere* sphere1 = new ISphere(dvec3(0.0, 0.0, 0.0), 4.0);
IEllipsoid* ellipsoid = new IEllipsoid(dvec3(4, 0, 5), dvec3(1, 1, 2.5));
ICylinderY* cylinderY = new ICylinderY(dvec3(8.0, 3.0, -2.0), 1.5, 3.0);
IClosedCylinderZ* cylinderZ = new IClosedCylinderZ(dvec3(5, 4, -2), 2.0, 3.0);
IClosedConeY* closedConey = new IClosedConeY(dvec3(5, 6, -3), 1.5, 3);
IDisk* disk = new IDisk(dvec3(-8, 0, 10), dvec3(1, 0, 0), 3);

void buildScene() {
	// Choose materials and colors and textures and opacity as needed
	scene.addOpaqueObject(new VisibleIShape(plane, tin));
	scene.addTransparentObject(new TransparentIShape(clearPlane, red, 0.25));

	scene.addOpaqueObject(new VisibleIShape(sphere1, silver));
	scene.addOpaqueObject(new VisibleIShape(ellipsoid, copper));

	scene.addOpaqueObject(new VisibleIShape(cylinderY, gold, &im1));
	scene.addOpaqueObject(new VisibleIShape(closedConey, polishedBronze));
	scene.addOpaqueObject(new VisibleIShape(cylinderZ, cyanPlastic));

	scene.addOpaqueObject(new VisibleIShape(disk, redPlastic));

	scene.addLight(lights[0]);
	scene.addLight(lights[1]);
	// spotlight is initially off, but feel free to make it initially on if you prefer
	lights[1]->isOn = true;
}

void render(GLFWwindow* window) {
	// Animation only matters if you are doing transparency. And the animation
	// is already implemented. If you wish to make any adjustments to animation,
	// do so very carefully, and make sure nothing breaks as a result. You could
	// just leave animation as is.
	if (isAnimated) {
		z += inc;
		if (z <= MINZ) {
			inc = -inc;
		}
		else if (z >= MAXZ) {
			inc = -inc;
		}
	}

	clearPlane->a = dvec3(0, 0, z);

	milliseconds frameStartTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();
	frameBuffer.clearColorBuffer();

	int left = 0;
	int right = frameBuffer.getWindowWidth() - 1;
	int bottom = 0;
	int top = frameBuffer.getWindowHeight() - 1;
	double N = 6.0;
	scene.camera = new PerspectiveCamera(dvec3(camX, camY, camZ), cameraFocus, cameraUp, glm::radians(cameraFOV), width, height);
	rayTrace.raytraceScene(frameBuffer, numReflections, scene);

	frameBuffer.showColorBuffer();
	milliseconds frameEndTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	milliseconds totalTime = frameEndTime - frameStartTime;
	cout << "Render time: " << totalTime.count() << " ms." << endl;

	// If you make any changes to animation, you might want to change this:
	if (isAnimated) {
		cout << "Transparent plane's z value: " << clearPlane->a.z << endl;
	}
}

// these two clamp functions help keep certain variables within a desired range
void incrementClamp(double& v, double delta, double lo, double hi) {
	v = glm::clamp(v + delta, lo, hi);
}

void incrementClamp(int& v, int delta, int lo, int hi) {
	v = glm::clamp(v + delta, lo, hi);
}

// You should not need to make any changes to the code below. 
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)
		return;

	bool isUpperCase = (mods & GLFW_MOD_SHIFT) != 0;

	const double INC = 0.5;
	switch (key) {
	case GLFW_KEY_RIGHT:
		incrementClamp(camX, 0.5, -20, 20);
		cout << scene.camera->getFrame().origin << endl;
		break;
	case GLFW_KEY_LEFT:
		incrementClamp(camX, -0.5, -20, 20);
		cout << scene.camera->getFrame().origin << endl;
		break;
	case GLFW_KEY_UP:
		incrementClamp(camZ, -0.5, -20, 20);
		cout << scene.camera->getFrame().origin << endl;
		break;
	case GLFW_KEY_DOWN:
		incrementClamp(camZ, 0.5, -20, 20);
		cout << scene.camera->getFrame().origin << endl;
		break;
	case GLFW_KEY_O:
		lights[currLight]->isOn = !lights[currLight]->isOn;
		cout << (lights[currLight]->isOn ? "ON" : "OFF") << endl;
		break;
	case GLFW_KEY_T:
		lights[currLight]->attenuationIsTurnedOn = !lights[currLight]->attenuationIsTurnedOn;
		cout << "attenuation " << (lights[currLight]->attenuationIsTurnedOn ? "ON" : "OFF") << endl;
		break;
	case GLFW_KEY_X:
		lights[currLight]->pos.x += (isUpperCase ? INC : -INC);
		cout << lights[currLight]->pos << endl;
		break;
	case GLFW_KEY_Y:
		lights[currLight]->pos.y += (isUpperCase ? INC : -INC);
		cout << lights[currLight]->pos << endl;
		break;
	case GLFW_KEY_Z:
		lights[currLight]->pos.z += (isUpperCase ? INC : -INC);
		cout << lights[currLight]->pos << endl;
		break;
	case GLFW_KEY_J:
		spotDirX += (isUpperCase ? INC : -INC);
		spotLight->setDir(spotDirX, spotDirY, spotDirZ);
		cout << spotLight->spotDir << endl;
		break;
	case GLFW_KEY_K:
		spotDirY += (isUpperCase ? INC : -INC);
		spotLight->setDir(spotDirX, spotDirY, spotDirZ);
		cout << spotLight->spotDir << endl;
		break;
	case GLFW_KEY_L:
		spotDirZ += (isUpperCase ? INC : -INC);
		spotLight->setDir(spotDirX, spotDirY, spotDirZ);
		cout << spotLight->spotDir << endl;
		break;
	case GLFW_KEY_F:
		incrementClamp(spotLight->fov, isUpperCase ? 0.2 : -0.2, 0.1, PI);
		cout << spotLight->fov << endl;
		break;
	case GLFW_KEY_A:
		incrementClamp(cameraFOV, isUpperCase ? 5 : -5, 5, 180);
		cout << cameraFOV << " degrees" << endl;
		break;
	case GLFW_KEY_EQUAL:	
		if (isUpperCase)
			antiAliasing = 3;
		cout << "Anti aliasing: " << antiAliasing << endl;
		break;
	case GLFW_KEY_MINUS:	
		antiAliasing = 1;
		cout << "Anti aliasing: " << antiAliasing << endl;
		break;
	case GLFW_KEY_P:
		isAnimated = !isAnimated;
		cout << "Animation: " << (isAnimated ? "on" : "off") << endl;
		break;
	case GLFW_KEY_SPACE:
		currLight = 1 - currLight;
		cout << *lights[currLight] << endl;
		break;
	case GLFW_KEY_0:
	case GLFW_KEY_1:
	case GLFW_KEY_2:
	case GLFW_KEY_3:
		numReflections = key - '0';
		cout << "Num reflections: " << numReflections << endl;
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
	initGraphics(W, H, username.c_str(), render, mouseUtility, keyboard, nullptr);
	return 0;
}