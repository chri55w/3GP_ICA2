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


		faceNormals[elements[i]] += crossProd;
		faceNormals[elements[i + 1]] += crossProd;
		faceNormals[elements[i + 2]] += crossProd;
	}
	for (int i = 0; i < faceNormals.size(); i++) {
		faceNormals[i] = normalize(faceNormals[i]);
	}

	return faceNormals;
}

void MyUtilities::applyNoiseToTerrain(std::vector<glm::vec3> &positions, const std::vector<glm::vec3> *normals, int octaves, float frequency, float lacunarity, float gain, float scale) {

	unsigned int seed = 237;
	PerlinNoise pn(seed);

	for (int i = 0; i < positions.size(); i++) {
		double noise = brownian(positions[i], octaves, frequency, lacunarity, gain, pn);
		
		noise *= scale;

		glm::vec3 noiseVec = glm::vec3(normals->at(i).x * noise, normals->at(i).y * noise, normals->at(i).z * noise);

		positions[i] += noiseVec;
	}
	
}
double MyUtilities::brownian(glm::vec3 position, int octaves, float frequency, float lacunarity, float gain, PerlinNoise &perNoise) {

	double total = 0.0;
	float amplitude = gain;


	for (int i = 0; i < octaves; i++) {
		total += perNoise.noise((float)position.x * frequency, (float)position.y * frequency, (float) position.z * frequency) * amplitude;
		frequency *= lacunarity;
		amplitude *= gain;
	}
	return total;
}
