#include "MyFrustum.h"
#include <math.h>

//Incomplete Frustum Culling based on this tutorial 
//		http://www.rastertek.com/dx11tut16.html

std::vector<MyPlanes> MyFrustum::m_planes{ 6 };

void MyFrustum::generateFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	float zMinimum, r;
	glm::mat4 frustumMatrix;
	
	//Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix[3][2] / projectionMatrix[2][2];
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix[2][2] = r;
	projectionMatrix[3][2] = -r * zMinimum;
	
	// Create the frustum matrix from the view matrix and updated projection matrix.
	frustumMatrix = viewMatrix * projectionMatrix;

	// Calculate near plane of frustum.
	m_planes[0].a = frustumMatrix[0][3] + frustumMatrix[0][2];
	m_planes[0].b = frustumMatrix[1][3] + frustumMatrix[1][2];
	m_planes[0].c = frustumMatrix[2][3] + frustumMatrix[2][2];
	m_planes[0].d = frustumMatrix[3][3] + frustumMatrix[3][2];

	// Calculate far plane of frustum.
	m_planes[1].a = frustumMatrix[0][3] + frustumMatrix[0][2];
	m_planes[1].b = frustumMatrix[1][3] + frustumMatrix[1][2];
	m_planes[1].c = frustumMatrix[2][3] + frustumMatrix[2][2];
	m_planes[1].d = frustumMatrix[3][3] + frustumMatrix[3][2];

	// Calculate left plane of frustum.
	m_planes[2].a = frustumMatrix[0][3] + frustumMatrix[0][0];
	m_planes[2].b = frustumMatrix[1][3] + frustumMatrix[1][0];
	m_planes[2].c = frustumMatrix[2][3] + frustumMatrix[2][0];
	m_planes[2].d = frustumMatrix[3][3] + frustumMatrix[3][0];

	// Calculate right plane of frustum.
	m_planes[3].a = frustumMatrix[0][3] + frustumMatrix[0][0];
	m_planes[3].b = frustumMatrix[1][3] + frustumMatrix[1][0];
	m_planes[3].c = frustumMatrix[2][3] + frustumMatrix[2][0];
	m_planes[3].d = frustumMatrix[3][3] + frustumMatrix[3][0];

	// Calculate top plane of frustum.
	m_planes[4].a = frustumMatrix[0][3] + frustumMatrix[0][1];
	m_planes[4].b = frustumMatrix[1][3] + frustumMatrix[1][1];
	m_planes[4].c = frustumMatrix[2][3] + frustumMatrix[2][1];
	m_planes[4].d = frustumMatrix[3][3] + frustumMatrix[3][1];

	// Calculate bottom plane of frustum.
	m_planes[5].a = frustumMatrix[0][3] + frustumMatrix[0][1];
	m_planes[5].b = frustumMatrix[1][3] + frustumMatrix[1][1];
	m_planes[5].c = frustumMatrix[2][3] + frustumMatrix[2][1];
	m_planes[5].d = frustumMatrix[3][3] + frustumMatrix[3][1];
	return;

	//Normals aught to be calculated.
}

bool MyFrustum::checkCube(float xCenter, float yCenter, float zCenter, float radius) {


	// Check if any one point of the cube is in the view frustum.
	for (int i = 0; i<6; i++) {
		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter - radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter + radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter - radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter + radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter - radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter + radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter - radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if (planeDotCoord(&m_planes[i], &glm::vec3((xCenter + radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

float MyFrustum::planeDotCoord(MyPlanes *plane, glm::vec3 *position) {
	//Calculation to replace directX version 'D3DXPlaneCoord'
	return (plane->a * position->x) + (plane->b * position->y) + (plane->c * position->z) + plane->d;
}