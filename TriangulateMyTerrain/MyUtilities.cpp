#include "MyUtilities.h"


glm::vec3 MyUtilities::cross(glm::vec3 V1, glm::vec3 V2) {
	glm::vec3 normal;

	double vx = (V1.y * V2.z) - (V1.z * V2.y);

	double vy = (V1.z * V2.x) - (V1.x * V2.z);

	double vz = (V1.x * V2.y) - (V1.y * V2.x);

	normal = glm::vec3(vx, vy, vz);

	return normal;
}

glm::vec3 MyUtilities::normalize(glm::vec3 vec) {
	glm::vec3 normalizedVec;

	double length = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));

	double nvx = vec.x / length;
	double nvy = vec.y / length;
	double nvz = vec.z / length;

	normalizedVec = glm::vec3(nvx, nvy, nvz);

	return normalizedVec;
}

std::vector<glm::vec3> MyUtilities::calculateNormals(std::vector<GLuint> elements, std::vector<glm::vec3> positions) {

	std::vector<glm::vec3> faceNormals;
	faceNormals.resize(positions.size());

	for (int i = 0; i < elements.size(); i += 3) {

		glm::vec3 coplanarBA = positions[elements[i + 1]] - positions[elements[i]];
		glm::vec3 coplanarCA = positions[elements[i + 2]] - positions[elements[i]];

		glm::vec3 crossProd = cross(coplanarBA, coplanarCA);

		//glm::vec3 norm = normalize(crossProd);

		faceNormals[elements[i]] += crossProd;
		faceNormals[elements[i + 1]] += crossProd;
		faceNormals[elements[i + 2]] += crossProd;
	}
	for (int i = 0; i < faceNormals.size(); i++) {
		faceNormals[i] = normalize(faceNormals[i]);
	}

	return faceNormals;
}

void MyUtilities::applyNoiseToTerrain(std::vector<glm::vec3> &positions, int spacingX) {
	unsigned int seed = 237;
	PerlinNoise pn(seed);

	for (int i = 0; i < positions.size(); i++) {
		double noise = pn.noise(positions[i].r, positions[i].g, 0.8);

		positions[i].g *= noise;
	}
}