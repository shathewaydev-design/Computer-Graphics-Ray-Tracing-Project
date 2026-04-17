/****************************************************
 * 2016-2025 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission has been granted.
 ****************************************************/

#include "light.h"
#include "io.h"
#include "ishape.h"

 /**
  * @fn	color ambientColor(const color& mat, const color& lightColor)
  * @brief	Computes the ambient color produced by a single light at a single point.
  * @param	mat  	Ambient material property.
  * @param	lightColor	Light's color.
  * @return	Ambient color.
   */

color ambientColor(const color& mat, const color& lightColor) {
	/* CSE 386 - todo  */

	color newCol = mat * lightColor;

	return newCol;
}

/**
 * @fn	color diffuseColor(const color &mat, const color &lightColor, const dvec3 &l, const dvec3 &n)
 * @brief	Computes diffuse color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	lightColor	 	The light's color.
 * @param	l		 	Light vector.
 * @param	n		 	Normal vector.
 * @return	Diffuse color.
 */

color diffuseColor(const color& mat, const color& lightColor,
	const dvec3& l, const dvec3& n) {
	/* CSE 386 - todo  */

	color newCol = mat * lightColor * glm::max(0.0, glm::dot(l, n));

	return newCol;
}

/**
 * @fn	color specularColor(const color &mat, const color &lightColor, double shininess,
 *							const dvec3 &r, const dvec3 &v)
 * @brief	Computes specular color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	lightColor	 	The light's color.
 * @param	shininess	Material shininess.
 * @param	r		 	Reflection vector.
 * @param	v		 	Viewing vector.
 * @return	Specular color.
 */

color specularColor(const color& mat, const color& lightColor,
	double shininess,
	const dvec3& r, const dvec3& v) {
	/* CSE 386 - todo  */

	color newCol = mat * lightColor * std::pow(glm::max(0.0, glm::dot(v, r)), shininess);

	return newCol;
}

/**
 * @fn	color totalColor(const Material &mat, const color &lightColor,
 *						const dvec3 &viewingDir, const dvec3 &normal,
 *						const dvec3 &lightPos, const dvec3 &intersectionPt,
 *						bool attenuationOn, const LightAttenuationParameters &ATparams)
 * @brief	Color produced by a single light at a single point. This includes the summation
 * of the ambient, diffuse, and specular components. Attenuation is also applied, if applicable.
 * @param	mat			  	Material.
 * @param	lightColor	  	The light's color.
 * @param	v	  			The v vector.
 * @param	n   		  	Normal vector.
 * @param	lightPos	  	Light position.
 * @param	intersectionPt	(x,y,z) of intersection point.
 * @param	attenuationOn 	true if attenuation is on.
 * @param	ATparams	  	Attenuation parameters.
 * @return	Color produced by a single light at a single point. This includes the summation
 * of the ambient, diffuse, and specular components. Attenuation is also applied, if applicable.
 */

color totalColor(const Material& mat, const color& lightColor,
	const dvec3& v, const dvec3& n,
	const dvec3& lightPos, const dvec3& intersectionPt,
	bool attenuationOn,
	const LightATParams& ATparams) {
	/* CSE 386 - todo  */

	color producedCol;

	dvec3 lightVec = lightPos - intersectionPt;
	double dist = glm::length(lightVec); // need unnormalized dist for attenuation
	dvec3 l = glm::normalize(lightVec);

	dvec3 n_norm = glm::normalize(n);
	dvec3 v_norm = glm::normalize(v);

	dvec3 r = glm::normalize(2 * glm::dot(n_norm, l) * n_norm - l);

	color producedAmb = ambientColor(mat.ambient, lightColor);
	color producedDiff = diffuseColor(mat.diffuse, lightColor, 
		l, n_norm);
	color producedSpec(0.0, 0.0, 0.0);
	if (glm::dot(l, n_norm) > 0.0)
		producedSpec = specularColor(mat.specular, lightColor,
			mat.shininess, r, v_norm);
	

	if (attenuationOn) {

		double att = ATparams.factor(dist);
		producedCol = glm::clamp(producedAmb + 
			att * (producedDiff + producedSpec), 0.0, 1.0);

	}
	else {
		producedCol = glm::clamp(producedAmb + producedDiff + producedSpec, 
			0.0, 1.0);
	}

	

	return producedCol;
}

/**
 * @fn	color PositionalLight::illuminate(const dvec3 &interceptWorldCoords,
 *										const dvec3 &normal, const Material &material,
 *										const dvec3& viewerPos, bool inShadow) const
 * @brief	Computes the color this light produces in RAYTRACING applications.
 * @param	interceptWorldCoords	(x, y, z) at the intercept point.
 * @param	normal				The normal vector.
 * @param	material			The object's material properties.
 * @param	viewerPos			Location of camera/viewer.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color PositionalLight::illuminate(const dvec3& interceptWorldCoords,
	const dvec3& normal,
	const Material& material,
	const dvec3& viewerPos,
	bool inShadow) const {
	/* CSE 386 - todo  */

	if (!isOn) {
		return black;
	}
	else if (isOn && inShadow) {
		return ambientColor(material.ambient, lightColor);
	}
	else if (isOn && !inShadow) {

		dvec3 v = viewerPos - interceptWorldCoords;

		return totalColor(material, lightColor, v, normal, pos, 
			interceptWorldCoords, attenuationIsTurnedOn, atParams);
	}

	

}

/**
* @fn	bool PositionalLight::pointIsInAShadow(const dvec3& intercept, const dvec3& normal, const vector<VisibleIShapePtr>& objects, const dvec3& viewerPos) const
* @brief	Determines if an intercept point falls in a shadow.
* @param	intercept	the position of the intercept.
* @param	normal		the normal vector at the intercept point
* @param	objects		the collection of opaque objects in the scene
*/

bool PositionalLight::pointIsInAShadow(const dvec3& intercept,
	const dvec3& normal,
	const vector<VisibleIShapePtr>& objects) const {
	/* CSE 386 - todo  */
	// call movePointOffSurface to help avoid surface acne.
	
	dvec3 liftPt = IShape::movePointOffSurface(intercept, normal);

	// now get shadow feeler ray
	 
	Ray shadowFeeler = getShadowFeeler(liftPt, normal);

	// and check to see if it intersects any shapes
	double lightDist = glm::distance(pos, intercept);

	for (VisibleIShape* object : objects) {
		OpaqueHitRecord hit;
		object->findClosestIntersection(shadowFeeler, hit);
		if (hit.t < FLT_MAX 
			&& glm::distance(hit.interceptPt, intercept) < lightDist) {
			return true;
		}
	}

	return false;
}

/**
* @fn	Ray PositionalLight::getShadowFeeler(const dvec3& interceptWorldCoords, const dvec3& normal, const Frame &eyeFrame) const
* @brief	Returns the shadow feeler for this light.
* @param	interceptWorldCoords	the position of the intercept.
* @param	normal		The normal vector at the intercept point
*/

Ray PositionalLight::getShadowFeeler(const dvec3& interceptWorldCoords,
	const dvec3& normal) const {
	/* 386 - todo */
	//dvec3 origin(0, 0, 0);
	dvec3 origin = interceptWorldCoords;

	//dvec3 dir(1, 1, 1);
	dvec3 dir = glm::normalize(pos - interceptWorldCoords);

	Ray shadowFeeler(origin, dir);
	return shadowFeeler;
}

/**
 * @fn	color SpotLight::illuminate(const dvec3 &interceptWorldCoords,
 *									const dvec3 &normal, const Material &material,
 *									const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in raytracing applications.
 * @param	interceptWorldCoords				The surface properties of the intercept point.
 * @param	normal					The normal vector.
 * @param	material			The object's material properties.
 * @param	viewerPos			Location of camera/viewer.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color SpotLight::illuminate(const dvec3& interceptWorldCoords,
	const dvec3& normal,
	const Material& material,
	const dvec3& viewerPos,
	bool inShadow) const {
	/* CSE 386 - todo  */

	if (isInSpotlightCone(pos, spotDir, fov, interceptWorldCoords)) {
		return PositionalLight::illuminate(interceptWorldCoords, 
			normal, material, viewerPos, inShadow);
	}
	
	return black;


}

/**
* @fn	void SpotLight::setDir (double dx, double dy, double dz)
* @brief	Sets the direction of the spotlight.
* @param	dx		x component of the direction
* @param	dy		y component of the direction
* @param	dz		z component of the direction
*/

void SpotLight::setDir(double dx, double dy, double dz) {
	spotDir = glm::normalize(dvec3(dx, dy, dz));
}

/**
* @fn	SpotLight::isInSpotlightCone(const dvec3& spotPos, const dvec3& spotDir, double spotFOV, const dvec3& intercept)
* @brief	Determines if an intercept point falls within a spotlight's cone.
* @param	spotPos		where the spotlight is positioned
* @param	spotDir		normalized direction of spotlight's pointing direction
* @param	spotFOV		spotlight's field of view, which is 2X of the angle from the viewing axis
* @param	intercept	the position of the intercept.
*/

bool SpotLight::isInSpotlightCone(const dvec3& spotPos,
	const dvec3& spotDir,
	double spotFOV,
	const dvec3& intercept) {
	/* CSE 386 - todo  */

	double cutOff = spotFOV / 2.0;

	dvec3 negL = glm::normalize(intercept - spotPos);

	double pointCos = glm::dot(negL, glm::normalize(spotDir));

	if (pointCos > glm::cos(cutOff)) {
		return true;
	}


	return false;
}
