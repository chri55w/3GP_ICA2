#include "MyUtilities.h"


glm::vec3 MyUtilities::cross(glm::vec3 V1, glm::vec3 V2) {

	//identifies the cross product
	glm::vec3 normal;

	double vx = (V1.y * V2.z) - (V1.z * V2.y);

	double vy = (V1.z * V2.x) - (V1.x * V2.z);

	double vz = (V1.x * V2.y) - (V1.y * V2.x);

	normal = glm::vec3(vx, vy, vz);

	return normal;
}
//Developed this then found out openGL has its own normalize funtion.
//		Left this function in use to demonstrate my own function that does the exact same thing. ENJOY! 
glm::vec3 MyUtilities::normalize(glm::vec3 vec) {
	//calculations to normalise a vector.
	glm::vec3 normalizedVec;

	double length = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));

	double nvx = vec.x / length;
	double nvy = vec.y / length;
	double nvz = vec.z / length;

	normalizedVec = glm::vec3(nvx, nvy, nvz);

	return normalizedVec;
}
//Normal Vector Calculation with reference to the following:
//			http://www.lighthouse3d.com/opengl/terrain/index.php3?normals

std::vector<glm::vec3> MyUtilities::calculateNormals(std::vector<GLuint> elements, std::vector<glm::vec3> positions) {

	std::vector<glm::vec3> vertexNormals;
	vertexNormals.resize(positions.size());

	for (int i = 0; i < elements.size(); i += 3) {

		glm::vec3 coplanarBA = positions[elements[i + 1]] - positions[elements[i]];
		glm::vec3 coplanarCA = positions[elements[i + 2]] - positions[elements[i]];

		glm::vec3 crossProd = cross(coplanarBA, coplanarCA);


		vertexNormals[elements[i]] += crossProd;
		vertexNormals[elements[i + 1]] += crossProd;
		vertexNormals[elements[i + 2]] += crossProd;
	}
	for (int i = 0; i < vertexNormals.size(); i++) {
		vertexNormals[i] = normalize(vertexNormals[i]);
	}

	return vertexNormals;
}

//Brownian Motion with Reference to the following:
//			https://code.google.com/p/fractalterraingeneration/wiki/Fractional_Brownian_Motion

void MyUtilities::applyNoiseToTerrain(std::vector<glm::vec3> &positions, const std::vector<glm::vec3> *normals, int octaves, float frequency, float lacunarity, float gain) {

	unsigned int seed = 237;
	PerlinNoise pn(seed);

	for (int i = 0; i < positions.size(); i++) {
		double noise = brownian(positions[i], octaves, frequency, lacunarity, gain, pn);
		
		glm::vec3 noiseVec = glm::vec3(normals->at(i).x * noise, normals->at(i).y * noise, normals->at(i).z * noise);

		positions[i] += noiseVec;
	}
	
}
double MyUtilities::brownian(glm::vec3 position, int octaves, float frequency, float lacunarity, float gain, PerlinNoise &perNoise) {

	double total = 0.0;
	float amplitude = 15;


	for (int i = 0; i < octaves; i++) {
		total += perNoise.noise((float)position.x * frequency, (float)position.y * frequency, (float)position.z * frequency) * amplitude;
		frequency *= lacunarity;
		amplitude *= gain;
	}
	return total;
}
