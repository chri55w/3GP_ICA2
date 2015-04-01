#pragma once

#include <glm/glm.hpp>
#include <tgl/tgl.h>
#include <vector>
#include "PerlinNoise.h"

class MyUtilities {
	

	public:

		static std::vector<glm::vec3> calculateNormals(std::vector<GLuint> elements, std::vector<glm::vec3> positions);
		static glm::vec3 cross(glm::vec3 V1, glm::vec3 V2);
		static glm::vec3 normalize(glm::vec3 vec);

		static void applyNoiseToTerrain(std::vector<glm::vec3> &positions, int spacingX);
};

