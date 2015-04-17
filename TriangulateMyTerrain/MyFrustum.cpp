#include "MyFrustum.h"
#include <math.h>

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
	//D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].a = frustumMatrix[0][3] + frustumMatrix[0][2];
	m_planes[1].b = frustumMatrix[1][3] + frustumMatrix[1][2];
	m_planes[1].c = frustumMatrix[2][3] + frustumMatrix[2][2];
	m_planes[1].d = frustumMatrix[3][3] + frustumMatrix[3][2];
	//D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].a = frustumMatrix[0][3] + frustumMatrix[0][0];
	m_planes[2].b = frustumMatrix[1][3] + frustumMatrix[1][0];
	m_planes[2].c = frustumMatrix[2][3] + frustumMatrix[2][0];
	m_planes[2].d = frustumMatrix[3][3] + frustumMatrix[3][0];
	//DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].a = frustumMatrix[0][3] + frustumMatrix[0][0];
	m_planes[3].b = frustumMatrix[1][3] + frustumMatrix[1][0];
	m_planes[3].c = frustumMatrix[2][3] + frustumMatrix[2][0];
	m_planes[3].d = frustumMatrix[3][3] + frustumMatrix[3][0];
	//D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].a = frustumMatrix[0][3] + frustumMatrix[0][1];
	m_planes[4].b = frustumMatrix[1][3] + frustumMatrix[1][1];
	m_planes[4].c = frustumMatrix[2][3] + frustumMatrix[2][1];
	m_planes[4].d = frustumMatrix[3][3] + frustumMatrix[3][1];
	//D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].a = frustumMatrix[0][3] + frustumMatrix[0][1];
	m_planes[5].b = frustumMatrix[1][3] + frustumMatrix[1][1];
	m_planes[5].c = frustumMatrix[2][3] + frustumMatrix[2][1];
	m_planes[5].d = frustumMatrix[3][3] + frustumMatrix[3][1];
	//D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
	return;
}

bool MyFrustum::CheckPoint(float x, float y, float z) {

	// Check if the point is inside all six planes of the view frustum.
	for (int i = 0; i<6; i++)
	{
		if ()

		/*
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
		*/
	}

	return true;
}