#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "MyPlanes.h"
#include <vector>
class MyFrustum {
	public:

		static void generateFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
		static bool checkCube(float xCenter, float yCenter, float zCenter, float radius);

	private:
		static std::vector<MyPlanes> m_planes;
		static float planeDotCoord(MyPlanes *plane, glm::vec3 *position);
};

