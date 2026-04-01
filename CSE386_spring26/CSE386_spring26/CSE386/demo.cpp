// Basics of c++;
#include "io.h" // includes most of what we need for our code
#include "utilities.h" // bc we are coding in utilities.cpp
#include "camera.h"
//using namespace std;

// computes and returns the volume of a sphere
// of a given radius
// pass-by-value function: 
double sphereVolume(double r) {
	return ((4. / 3) * PI * (r * r * r));

}
// tells whether an int is evenv
bool isEven(int num) {
	
	return num % 2 == 0;
}

// function that limits a given value to a given range of values
// shrinking nums that are to big and increasing nums that are too small
// since this will change the value, pass-by-REFERENCE
void (limit(double& val, double lo, double hi)) {
	if (val < lo) val = lo;
	if (val > hi) val = hi;

}


int mainDEMO() {

	PerspectiveCamera per(dvec3(4, 7, -5), dvec3(2, 1, 9),
		dvec3(0, 1, 0), PI/3, 700, 500);

	OrthographicCamera ort(dvec3(5, 5, 0), dvec3(11, 5, -8),
		dvec3(0, 1, 0), 5, 10, 1);

	cout << glm::normalize(glm::cross(dvec3(0, 1, 0), dvec3(0.8, 0.6, 0))) << endl;
	//cout << per.getFrame() << endl;
	//cout << per.getDistToPlane() << endl;

	// once we have the camera, we can convert pixels into 
	// viewing rays:
	Ray r = per.getRay(400, 300);
	//cout << r.origin << endl;
	//cout << "FOR QUIZ DIRECTION->" << r.dir << endl;

	// now that we have a ray, see if it hits a sphere
	ISphere s(dvec3(-2, -12, 40), 10);
	// see slides about these next two lines
	HitRecord myHit; // where the hit info gets stored
	s.findClosestIntersection(r, myHit);
	cout << "FOR QUIZ->" << myHit.t << endl; // if t < FLT_MAX, then it's a hit
	// if t == FLT_MAX (3.4e38), then it is NOT a hit
	



	//// Ray and ISphere and HitRecord are not glm
	//// They are made by 386 instructors
	//// sphere at (10, 0, 0), r = 2
	//ISphere s(dvec3(10, 0, 0), 2);

	////cout << s << endl;

	//// ray with origin (3, 0, 0) and direction <1, 0, 0>
	//Ray r(dvec3(3, 2, 0), dvec3(1, 0, 0)); // constructor automatically normalizes direction

	//// find how many times and where the ray hits the sphere
	//// the sphere has the function we need, findIntersections()
	//// findIntersections retunrs the number of places ray hits sphere ()0, 1, 2)
	//// and it fills in a HitRecord objects with details about 
	//// where those hits are
	//HitRecord myHits[2]; // stores the hit data (t and the intersection point)
	//int hitCount = s.findIntersections(r, myHits);

	//cout << "number of hits = " << hitCount << endl;

	//// let's inspect the hitRecord objects myHits[0] and [1]
	//cout << "t of first hit = " << myHits[0].t << endl;
	//cout << "t of second hit = " << myHits[1].t << endl;

	//cout << "location of first hit = " << myHits[0].interceptPt << endl;
	//cout << "location of second hit = " << myHits[1].interceptPt << endl;

	//// let's create a perspective camera that can view the sphere:
	//// need location and direction and up <0, 1, 0>
	//// see handout for details - location, looking at, up vector, field of view, x, y grid
	//PerspectiveCamera per(dvec3(0, 0, 0), dvec3(0, 0, -1), dvec3(0, 1, 0), 
	//	PI/4, 12, 6);
	//cout << per.getFrame() << endl;

	//Ray myRay = per.getRay(6, 3);
	//cout << myRay.origin << endl;
	//cout << myRay.dir << endl;

	//cout << s.findIntersections(myRay, myHits) << endl;

	//double num = 0;
	//double x = 2 / num;

	//cout << "huh" << glm::atan(x) << endl;
	//cout << "huh" << PI / 2 << endl;






	// Monday question: how do I figure out if and where
	// r intersects s




	return 0;





}