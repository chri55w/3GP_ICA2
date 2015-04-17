#include "MyFrustum.h"
#include <math.h>

void MyFrustum::generateFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	float zMinimum, r;
	glm::mat4 frustumMatrix;


	// Calculate the minimum Z distance in the frustum.
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
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
	*/
	return;
}