#include <iostream>
#include "defs.h"
#include "io.h"
#include "light.h"
#include "camera.h"

int colormain(int argc, char* argv[]) {

	// This example is the one used in the first handout example we covered in class.
	dvec3 eye_pos(3.0, 0.0, 1.0);

	dvec3 intercept_pt(5.0, 2.0, -3.0);
	dvec3 pointToRight(4.0, 4.0, 0.0);
	dvec3 pointToLeft(0.0, 0.0, 0.0);

	color light_color(0.6, 0.6, 0.7);
	dvec3 light_pos(6.0, 1.0, 0.0);

	color mat_amb(0.4, 0.5, 0.6);
	color mat_diff(0.9, 1.0, 0.9);
	color mat_spec(0.9, 0.8, 0.7);

	dvec3 V1 = pointToRight - intercept_pt;
	dvec3 V2 = pointToLeft - intercept_pt;
	dvec3 n = glm::normalize(glm::cross(V1, V2));
	dvec3 v = glm::normalize(eye_pos - intercept_pt);
	dvec3 l = glm::normalize(light_pos - intercept_pt);
	dvec3 r = glm::normalize(2.0 * glm::dot(l, n) * n - l);

	cout << "vectors, all normalized: " << endl;
	cout << "n: " << n << endl;
	cout << "v: " << v << endl;
	cout << "l: " << l << endl;
	cout << "r: " << r << endl;
	cout << endl;

	cout << "Ambient: " << ambientColor(mat_amb, light_color) << endl;
	cout << "Diffuse: " << diffuseColor(mat_diff, light_color, l, n) << endl;
	cout << "Specular: " << specularColor(mat_spec, light_color, 1.0, r, v) << endl;

	Material mat(mat_amb, mat_diff, mat_spec, 1.0);
	
	// attenuation parameters
	LightATParams atParams(1.0, 2.0, 0.0);
	bool useAttenuation = false;

	cout << "Total: " << totalColor(mat, light_color, v, n, light_pos, intercept_pt, useAttenuation, atParams) << endl;
	cout << endl;

	cout << "Tests involving negative dot products" << endl;
	dvec3 offL = glm::normalize(dvec3(-1.0, -1.0, -1.0));
	cout << "Diffuse with negative l dot n: " << diffuseColor(mat_diff, light_color, offL, n) << endl;
	dvec3 offV = glm::normalize(dvec3(-1.0, -1.0, -1.0));
	cout << "Specular with negative r dot v shininess = 1.5: " << specularColor(mat_spec, light_color, 1.5, r, offV) << endl;
	cout << "Specular with negative r dot v shininess = 4.0: " << specularColor(mat_spec, light_color, 4.0, r, offV) << endl;
	cout << "Specular with positive r dot v shininess = 1.5: " << specularColor(mat_spec, light_color, 1.5, r, v) << endl;
	cout << endl;

	vector<VisibleIShapePtr> objs;

	cout << "illuminate tests" << endl << endl;
	PositionalLight light(light_pos, atParams, light_color);

	PerspectiveCamera pCamera(eye_pos, intercept_pt, Y_AXIS, PI_2, WINDOW_WIDTH, WINDOW_HEIGHT);
	dvec3 camera_pos = pCamera.getFrame().origin;

	cout << "Attenuation factor: " << atParams.factor(glm::distance(light_pos, intercept_pt)) << endl;
	cout << endl;

	cout << "Light on: FALSE   Shadow: FALSE    Attenuation: FALSE" << endl;
	light.isOn = false; 
	light.attenuationIsTurnedOn = false;
	bool inShadow = false;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: FALSE   Shadow: FALSE    Attenuation: TRUE " << endl;
	light.isOn = false; 
	light.attenuationIsTurnedOn = true;
	inShadow = false;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: FALSE   Shadow: TRUE     Attenuation: FALSE" << endl;
	light.isOn = false; 
	light.attenuationIsTurnedOn = false;
	inShadow = true;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: FALSE   Shadow: TRUE     Attenuation: TRUE" << endl;
	light.isOn = false; 
	light.attenuationIsTurnedOn = true;
	inShadow = true;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: TRUE   Shadow: FALSE    Attenuation: FALSE" << endl;
	light.isOn = true; 
	light.attenuationIsTurnedOn = false;
	inShadow = false;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: TRUE   Shadow: FALSE    Attenuation: TRUE " << endl;
	light.isOn = true; 
	light.attenuationIsTurnedOn = true;
	inShadow = false;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: TRUE   Shadow: TRUE     Attenuation: FALSE" << endl;
	light.isOn = true; 
	light.attenuationIsTurnedOn = false;
	inShadow = true;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	cout << "Light on: TRUE   Shadow: TRUE     Attenuation: TRUE" << endl;
	light.isOn = true; 
	light.attenuationIsTurnedOn = true;
	inShadow = true;
	cout << light.illuminate(intercept_pt, n, mat, camera_pos, inShadow) << endl;

	return 0;
}
/*
vectors, all normalized:
n: [ 0.5773502692 -0.5773502692 0.5773502692 ]
v: [ -0.4082482905 -0.4082482905 0.8164965809 ]
l: [ 0.3015113446 -0.3015113446 0.9045340337 ]
r: [ 0.7035264707 -0.7035264707 0.1005037815 ]

Ambient: [ 0.24 0.3 0.42 ]
Diffuse: [ 0.4700096711 0.5222329679 0.5483446163 ]
Specular: [ 0.04431293675 0.03938927711 0.04020988705 ]
Total: [ 0.7543226078 0.861622245 1 ] <-- X AND Y DON'T MATCH UP, CHECK

Tests involving negative dot products <-- SO FAR CORRECT
Diffuse with negative l dot n: [ 0 0 0 ]
Specular with negative r dot v shininess = 1.5: [ 0 0 0 ]
Specular with negative r dot v shininess = 4.0: [ 0 0 0 ]
Specular with positive r dot v shininess = 1.5: [ 0.01269401244 0.01128356661 0.01151864092 ]

illuminate tests

Attenuation factor: 0.1310058042

Light on: FALSE   Shadow: FALSE    Attenuation: FALSE
[ 0 0 0 ]
Light on: FALSE   Shadow: FALSE    Attenuation: TRUE
[ 0 0 0 ]
Light on: FALSE   Shadow: TRUE     Attenuation: FALSE
[ 0 0 0 ]
Light on: FALSE   Shadow: TRUE     Attenuation: TRUE
[ 0 0 0 ]
Light on: TRUE   Shadow: FALSE    Attenuation: FALSE
[ 0.7543226078 0.861622245 1 ] <-- CORRECT
Light on: TRUE   Shadow: FALSE    Attenuation: TRUE
[ 0.3073792469 0.3735757739 0.497104056 ] <-- INCORRECT
Light on: TRUE   Shadow: TRUE     Attenuation: FALSE
[ 0.24 0.3 0.42 ] <-- CORRECT
Light on: TRUE   Shadow: TRUE     Attenuation: TRUE
[ 0.24 0.3 0.42 ] <-- CORRECT
*/