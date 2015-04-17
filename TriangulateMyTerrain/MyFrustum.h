#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "MyPlanes.h"
#include <vector>
class MyFrustum {
	public:

		static void generateFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
		static bool CheckPoint(float x, float y, float z);

	private:
		static std::vector<MyPlanes> m_planes;
};

