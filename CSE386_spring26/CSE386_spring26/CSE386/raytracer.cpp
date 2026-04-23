/****************************************************
 * 2016-2025 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission has been granted.
 ****************************************************/
#include "raytracer.h"
#include "ishape.h"
#include "io.h"

 /**
  * @fn	RayTracer::RayTracer(const color &defa)
  * @brief	Constructs a raytracers.
  * @param	defa	The clear color.
  */

RayTracer::RayTracer(const color& defa)
	: defaultColor(defa) {
}

/**
 * @fn	void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth, const IScene &theScene) const
 * @brief	Raytrace scene
 * @param [in,out]	frameBuffer	Framebuffer.
 * @param 		  	depth	   	The current depth of recursion.
 * @param 		  	theScene   	The scene.
 */

void RayTracer::raytraceScene(FrameBuffer& frameBuffer, int depth,
	const IScene& theScene) const {
	const RaytracingCamera& camera = *theScene.camera;
	const vector<VisibleIShapePtr>& objs = theScene.opaqueObjs;
	const vector<LightSourcePtr>& lights = theScene.lights;
	color defaultColor = frameBuffer.getClearColor();
	int N = 1;     // NxN anti-aliasing. Normal approach when N = 1


	// this is THE ratracing algorithm
	// loops through each pixel because this is an IMAGE-ORDER algorithm
	for (int y = 0; y < frameBuffer.getWindowHeight(); ++y) {
		for (int x = 0; x < frameBuffer.getWindowWidth(); ++x) {
			DEBUG_PIXEL = (x == xDebug && y == yDebug);
			if (DEBUG_PIXEL) {
				cout << "";
			}

			color sum = black;
			// Sum all rays from NxN grid (requires another nested loop)
			// Display sum/N2

			for (int j = 0; j < N; j++) {
				for (int i = 0; i < N; i++) {

					double offsetX = (i + 0.5) / N;
					double offsetY = (j + 0.5) / N;
					Ray ray = camera.getRay(x + offsetX, y + offsetY);

					OpaqueHitRecord theHit;
					VisibleIShape::findIntersection(ray, objs, theHit);

					if (theHit.t < FLT_MAX) {
						color c = black;

						c += traceIndividualRay(ray, theScene, 3);

						c = glm::clamp(c, 0.0, 1.0);

						sum += c;

					}
					else {
						sum += paleGreen;
					}

					
				}

			}

			color finalColor = sum / (double)(N * N);
			finalColor = glm::clamp(finalColor, 0.0, 1.0);

			frameBuffer.setColor(x, y, finalColor);

			Ray centerRay = camera.getRay(x + 0.5, y + 0.5);
			frameBuffer.showAxes(x, y, centerRay, 0.25); // Displays R/x, G/y, B/z axes

			/* CSE 386 - todo  */
			// for each pixel, generate the ray for that pixel
			// loop through all objects to see their t-value for that
			// ray. The shape with the lowest t-value determines the color of the pixel.

			//Ray ray = camera.getRay(x, y);
			//OpaqueHitRecord theHit;
			//VisibleIShape::findIntersection(ray, objs, theHit);

			

			// theHit is set to the t-value of the shape that was closest
			//if (theHit.t < FLT_MAX) {
				//color c = black;


				// use 1 light's illuminate() function to set
				// the color. lights[0]

				// there was a hit!
				// change this to a loop to go through all the lights
				// and add their illuminate results together (clamp as needed)

				// see if point is in shadow
				//bool inShadow = lights[0]->pointIsInAShadow(theHit.interceptPt, theHit.normal, objs);
				 


				// NOW CALLED IN HELPER METHOD!!!
				//for (int i = 0; i < lights.size(); i++) {
				//	bool inShadow = lights[i]->pointIsInAShadow(theHit.interceptPt, theHit.normal, objs);
				//	c += lights[i]->illuminate(theHit.interceptPt, theHit.normal, theHit.material, // WHEN MAKING LOOP, CLAMP C BEFORE COLORING PIXEL
				//		camera.getFrame().origin, inShadow);
				//	//glm::clamp(c, 0.0, 1.0);
				//	//frameBuffer.setColor(x, y, c);
				//}

				//c += traceIndividualRay(ray, theScene, 3);

				//c = glm::clamp(c, 0.0, 1.0);
				//frameBuffer.setColor(x, y, sum);

				//color C = theHit.material.diffuse; // basic way to get color
				//frameBuffer.setColor(x, y, c);
			
			//}

				//frameBuffer.setColor(x, y, sum);
			//else {
			//	frameBuffer.setColor(x, y, paleGreen);
			//}
			

			//frameBuffer.showAxes(x, y, ray, 0.25);			// Displays R/x, G/y, B/z axes
			
		}
	}
}

/**
 * @fn	color RayTracer::traceIndividualRay(const Ray &ray,
 *											const IScene &theScene,
 *											int recursionLevel) const
 * @brief	Trace an individual ray.
 * @param	ray			  	The ray.
 * @param	theScene	  	The scene.
 * @param	recursionLevel	The recursion level.
 * @return	The color to be displayed as a result of this ray.
 */

color RayTracer::traceIndividualRay(const Ray& ray, const IScene& theScene, int recursionLevel) const {
	/* CSE 386 - todo  */
	// This might be a useful helper function.

	if (recursionLevel <= 0) {
		return color(0, 0, 0);
	}

	OpaqueHitRecord closestHit;
	closestHit.t = FLT_MAX;

	for (VisibleIShape* object : theScene.opaqueObjs) {
		OpaqueHitRecord tempHit;
		object->findClosestIntersection(ray, tempHit);
		
		if (tempHit.t < closestHit.t) {
			closestHit = tempHit;

		}

	}

	if (closestHit.t == FLT_MAX) {
		return color(0, 0, 0);
	}


	color c = black;
	
	// loop through all lights, 
    // adding each lights effect (color) to the point's color
	for (int i = 0; i < theScene.lights.size(); i++) { 

		bool inShadow = theScene.lights[i]-> // is point in a shadow?
			pointIsInAShadow(closestHit.interceptPt, closestHit.normal, theScene.opaqueObjs);

		c += theScene.lights[i]->illuminate(closestHit.interceptPt, closestHit.normal, // add the light's effect to the color
			closestHit.material, theScene.camera->getFrame().origin, inShadow); 
		
	}

	dvec3 n = glm::normalize(closestHit.normal);

	dvec3 reflectDir = ray.dir - 2 * glm::dot(ray.dir, n) * n;

	dvec3 origin = IShape::movePointOffSurface(closestHit.interceptPt, closestHit.normal);

	Ray nextRay(origin, reflectDir);

	return c + 0.2 * traceIndividualRay(nextRay, theScene, recursionLevel - 1);
}
