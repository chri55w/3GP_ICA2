#include "MyView.hpp"
#include <tygra/FileHelper.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#include "MyUtilities.h"
#include "MyFrustum.h"
#include "MyHeightData.h"

MyView::MyView() {
}

MyView::~MyView() {
}

void MyView::setScene(std::shared_ptr<const SceneModel::Context> scene) {
	scene_ = scene;
}

void MyView::toggleShading() {
	shade_normals_ = !shade_normals_;
}

void MyView::windowViewWillStart(std::shared_ptr<tygra::Window> window) {
	assert(scene_ != nullptr);

	GLint compile_status = 0;
	GLint link_status = 0;

	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_string = tygra::stringFromFile("terrain_vs.glsl");
	const char *vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vertex_shader, 1, (const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(vertex_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_string = tygra::stringFromFile("terrain_fs.glsl");
	const char *fragment_shader_code = fragment_shader_string.c_str();
	glShaderSource(fragment_shader, 1, (const GLchar **)&fragment_shader_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(fragment_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	terrain_sp_ = glCreateProgram();
	glAttachShader(terrain_sp_, vertex_shader);
	glDeleteShader(vertex_shader);
	glAttachShader(terrain_sp_, fragment_shader);
	glDeleteShader(fragment_shader);
	glLinkProgram(terrain_sp_);

	glGetProgramiv(terrain_sp_, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetProgramInfoLog(terrain_sp_, string_length, NULL, log);
		std::cerr << log << std::endl;
	}


	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	vertex_shader_string = tygra::stringFromFile("shapes_vs.glsl");
	vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vertex_shader, 1, (const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(vertex_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	fragment_shader_string = tygra::stringFromFile("shapes_fs.glsl");
	fragment_shader_code = fragment_shader_string.c_str();
	glShaderSource(fragment_shader, 1,
		(const GLchar **)&fragment_shader_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(fragment_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	shapes_sp_ = glCreateProgram();
	glAttachShader(shapes_sp_, vertex_shader);
	glDeleteShader(vertex_shader);
	glAttachShader(shapes_sp_, fragment_shader);
	glDeleteShader(fragment_shader);
	glLinkProgram(shapes_sp_);

	glGetProgramiv(shapes_sp_, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetProgramInfoLog(terrain_sp_, string_length, NULL, log);
		std::cerr << log << std::endl;
	}


	glGenVertexArrays(1, &cube_vao_);
	glBindVertexArray(cube_vao_);
	glGenBuffers(1, &cube_vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);
	float cube_vertices[] = {
		-0.5f, 0.f, -0.5f, 0.5f, 0.f, -0.5f, 0.5f, 0.f, 0.5f,
		-0.5f, 0.f, -0.5f, 0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f,
		-0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 1.f, 0.5f,
		-0.5f, 0.f, 0.5f, 0.5f, 1.f, 0.5f, -0.5f, 1.f, 0.5f,
		0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, 0.5f, 1.f, -0.5f,
		0.5f, 0.f, 0.5f, 0.5f, 1.f, -0.5f, 0.5f, 1.f, 0.5f,
		0.5f, 0.f, -0.5f, -0.5f, 0.f, -0.5f, -0.5f, 1.f, -0.5f,
		0.5f, 0.f, -0.5f, -0.5f, 1.f, -0.5f, 0.5f, 1.f, -0.5f,
		-0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 1.f, 0.5f,
		-0.5f, 0.f, -0.5f, -0.5f, 1.f, 0.5f, -0.5f, 1.f, -0.5f,
		-0.5f, 1.f, 0.5f, 0.5f, 1.f, 0.5f, 0.5f, 1.f, -0.5f,
		-0.5f, 1.f, 0.5f, 0.5f, 1.f, -0.5f, -0.5f, 1.f, -0.5f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(kVertexPosition);
	glVertexAttribPointer(kVertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

	glEnableVertexAttribArray(kVertexNormal);
	glVertexAttribPointer(kVertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	const float sizeX = scene_->getTerrainSizeX();
	const float sizeY = scene_->getTerrainSizeY();
	const float sizeZ = scene_->getTerrainSizeZ();
	
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> terrainNormals;
	std::vector<GLuint> elements;

	MyHeightData::loadHeightMap(scene_);

	xIndices = (MyHeightData::getDataWidth() * levelOfDetail);
	zIndices = (MyHeightData::getDataHeight() * levelOfDetail);

	int subDivisionsZ = zIndices - 1;
	int subDivisionsX = xIndices - 1;

	const float spacingZ = sizeZ / subDivisionsZ;
	const float spacingX = sizeX / subDivisionsX;
	//Generate spaced positions
	for (int z = 0; z < zIndices; z++) {

		for (int x = 0; x < xIndices; x++) { 
			
			glm::vec3 new_pos = glm::vec3(spacingX * x, 0, -spacingZ * z);


			positions.push_back(new_pos);
		}
	}
	//Generate triangle elements
	int quadOrigin = 0;
	for (int z = 0; z < subDivisionsZ; z++) {

        for (int x = 0; x < subDivisionsX; x++) {

            if (((x % 2) == 0 && (z % 2) == 0) || ((x % 2) != 0 && (z % 2) != 0)) {

				elements.push_back(quadOrigin);
				elements.push_back(quadOrigin + 1);
				elements.push_back(quadOrigin + xIndices);

				elements.push_back(quadOrigin + 1);
				elements.push_back(quadOrigin + xIndices + 1);
				elements.push_back(quadOrigin + xIndices);

			} else {

				elements.push_back(quadOrigin);
				elements.push_back(quadOrigin + 1);
				elements.push_back(quadOrigin + xIndices + 1);

				elements.push_back(quadOrigin);
				elements.push_back(quadOrigin + xIndices + 1);
				elements.push_back(quadOrigin + xIndices);
				
			}
			quadOrigin++;
		}
		quadOrigin++;
	}

	applyHeightMap(sizeY, positions);

	terrainNormals = MyUtilities::calculateNormals(elements, positions);
	
	//Bezier is broken but included to demonstrate position reached
	//applyBezier(positions);

	//frustrum culling is not working correctly, frustrum may be being constructed wrong.
	//frustrumEnabled = true;

	MyUtilities::applyNoiseToTerrain(positions, &terrainNormals, 8, 1.f / (zIndices / 1.5), 2.0, 0.5);

	terrainNormals = MyUtilities::calculateNormals(elements, positions);

	//Populate VBO's

	glGenBuffers(1, &terrain_mesh_.element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_mesh_.element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	    elements.size() * sizeof(unsigned int),
	    elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	terrain_mesh_.element_count = elements.size();

	glGenBuffers(1, &terrain_mesh_.position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, terrain_mesh_.position_vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glGenBuffers(1, &terrain_mesh_.normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, terrain_mesh_.normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, terrainNormals.size() * sizeof(glm::vec3), terrainNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//Populate VAO's

	glGenVertexArrays(1, &terrain_mesh_.vao);
	glBindVertexArray(terrain_mesh_.vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_mesh_.element_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, terrain_mesh_.position_vbo);
	glEnableVertexAttribArray(kVertexPosition);
	glVertexAttribPointer(kVertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));
	
	glBindBuffer(GL_ARRAY_BUFFER, terrain_mesh_.normal_vbo);
	glEnableVertexAttribArray(kVertexNormal);
	glVertexAttribPointer(kVertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MyView::windowViewDidReset(std::shared_ptr<tygra::Window> window, int width, int height) {
	glViewport(0, 0, width, height);
}

void MyView::windowViewDidStop(std::shared_ptr<tygra::Window> window) {
	glDeleteProgram(terrain_sp_);
	glDeleteProgram(shapes_sp_);

	glDeleteBuffers(1, &terrain_mesh_.position_vbo);
	glDeleteBuffers(1, &terrain_mesh_.normal_vbo);
	glDeleteBuffers(1, &terrain_mesh_.element_vbo);
	glDeleteVertexArrays(1, &terrain_mesh_.vao);
}

void MyView::windowViewRender(std::shared_ptr<tygra::Window> window) {
	assert(scene_ != nullptr);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	const float aspect_ratio = viewport[2] / (float)viewport[3];

	const auto& camera = scene_->getCamera();
	glm::mat4 projection_xform = glm::perspective(camera.getVerticalFieldOfViewInDegrees(), aspect_ratio, camera.getNearPlaneDistance(), camera.getFarPlaneDistance());
	glm::vec3 camera_pos = camera.getPosition();
	glm::vec3 camera_at = camera.getPosition() + camera.getDirection();
	glm::vec3 world_up{ 0, 1, 0 };
	glm::mat4 view_xform = glm::lookAt(camera_pos, camera_at, world_up);


	/* TODO: you are free to modify any of the drawing code below */

	//generate the frustrum for this renderloop.
	MyFrustum::generateFrustum(screenDepth_, projection_xform, view_xform);


	glClearColor(0.f, 0.f, 0.25f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, shade_normals_ ? GL_FILL : GL_LINE);

	glUseProgram(terrain_sp_);

	GLuint shading_id = glGetUniformLocation(terrain_sp_, "use_normal");
	glUniform1i(shading_id, shade_normals_);

	glm::mat4 world_xform = glm::mat4(1);
	glm::mat4 view_world_xform = view_xform * world_xform;

	GLuint projection_xform_id = glGetUniformLocation(terrain_sp_, "projection_xform");
	glUniformMatrix4fv(projection_xform_id, 1, GL_FALSE, glm::value_ptr(projection_xform));

	GLuint view_world_xform_id = glGetUniformLocation(terrain_sp_, "view_world_xform");
	glUniformMatrix4fv(view_world_xform_id, 1, GL_FALSE, glm::value_ptr(view_world_xform));

	glBindVertexArray(terrain_mesh_.vao);
	glDrawElements(GL_TRIANGLES, terrain_mesh_.element_count, GL_UNSIGNED_INT, 0);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shapes_sp_);

	projection_xform_id = glGetUniformLocation(shapes_sp_, "projection_xform");
	glUniformMatrix4fv(projection_xform_id, 1, GL_FALSE, glm::value_ptr(projection_xform));

	glBindVertexArray(cube_vao_);
	int cubesRendered = 0;
	for (const auto& pos : scene_->getAllShapePositions()) {
		//If frustrum culling is enabled check if cube position is within frustrum before drawing, otherwise just draw.
		if (MyFrustum::checkCube(pos.x, 64.f, -pos.y, 1) || !frustrumEnabled) {
			world_xform = glm::translate(glm::mat4(1), glm::vec3(pos.x, 64, -pos.y));
			view_world_xform = view_xform * world_xform;

			view_world_xform_id = glGetUniformLocation(shapes_sp_, "view_world_xform");
			glUniformMatrix4fv(view_world_xform_id, 1, GL_FALSE, glm::value_ptr(view_world_xform));

			glDrawArrays(GL_TRIANGLES, 0, 36);
			cubesRendered++;
		}
	}

	std::cout << "Cubes rendered this draw call: " + std::to_string(cubesRendered) << std::endl;
}
void MyView::applyHeightMap(float sizeY, std::vector<glm::vec3> &positions) {
	float sizeModificationHeight = MyHeightData::getDataHeight() / (float)zIndices;
	float sizeModificationWidth = MyHeightData::getDataWidth() / (float)xIndices;
	
	for (int z = 0; z < zIndices; z++) {
		for (int x = 0; x < xIndices; x++) {
			//calculate the modified x and z values to take into consideration the size of the map in positions compared to the height map data.
			int modifiedX = sizeModificationWidth * x;
			int modifiedZ = sizeModificationWidth * z;
			//fetch the height map data then apply it to the map.
			float heightMapY = MyHeightData::getHeightValue(modifiedX, modifiedZ) * sizeY;

			positions[x + z * xIndices].y = heightMapY;
		}
	}
}

void MyView::applyBezier(std::vector<glm::vec3> &positions) {
	int heightImageDataWidth = MyHeightData::getDataWidth();
	int heightImageDataHeight = MyHeightData::getDataHeight();

	//calculate the number of positions in each axis per patch (per 16 control points
	int PUVCoordsCount = 4 * levelOfDetail;

	std::vector<glm::vec2> UVs;
	//calculate enough UVs for an entire patch worth of positions.
	for (int v = 0; v < PUVCoordsCount; v++) {
		for (int u = 0; u < PUVCoordsCount; u++) {
			glm::vec2 UVCoord = glm::vec2((float)u / PUVCoordsCount, (float)v / PUVCoordsCount);
			UVs.push_back(UVCoord);
		}
	}
	//process patches from the positions data using the level of detail to increase the resolution of the map.
	int patchesX = 0;
	int patchesZ = 0;
	std::vector<std::vector<std::vector<glm::vec3>>> patches;
	
	//Loop through the points taken from the height image
	for (int z = 0; z < 3; z += 3) {
		patchesX = 0;
		for (int x = 0; x < heightImageDataWidth; x += 3) {
			/*[	C3		P11		P12		C4
				P7		P8		P9		P10
				P3		P4		P5		P6
				C1		P1		P2		C2	]*/
			std::vector < std::vector<glm::vec3>> ctrlPoints;
			int pointOffset = x + (z * heightImageDataWidth);
			for (int zA = 0; zA < 4; zA++) {
				std::vector<glm::vec3> lineOfCtrlPoints;
				lineOfCtrlPoints.push_back(positions[pointOffset * levelOfDetail]);
				lineOfCtrlPoints.push_back(positions[pointOffset * levelOfDetail + (1 * levelOfDetail)]);
				lineOfCtrlPoints.push_back(positions[pointOffset * levelOfDetail + (2 * levelOfDetail)]);
				lineOfCtrlPoints.push_back(positions[pointOffset * levelOfDetail + (3 * levelOfDetail)]);
				pointOffset += heightImageDataWidth * levelOfDetail;
				ctrlPoints.push_back(lineOfCtrlPoints);
			}
			patches.push_back(ctrlPoints);
			patchesX++;
		}
		patchesZ++;
	}
	//Used Whilst debugging, force data into the first patch to check the bezier effect on that patch.
	/*
	std::vector<std::vector<glm::vec3>> controlPoints;

	std::vector<glm::vec3> line;

	line.push_back(glm::vec3(0, 0, 0)*(float)levelOfDetail);
	line.push_back(glm::vec3(1, 0, 0)*(float)levelOfDetail);
	line.push_back(glm::vec3(2, 0, 0)*(float)levelOfDetail);
	line.push_back(glm::vec3(3, 0, 0)*(float)levelOfDetail);

	controlPoints.push_back(line);

	line[0] = glm::vec3(0, 0, -1)*(float)levelOfDetail;
	line[1] = glm::vec3(1, 5, -1)*(float)levelOfDetail;
	line[2] = glm::vec3(2, 5, -1)*(float)levelOfDetail;
	line[3] = glm::vec3(3, 0, -1)*(float)levelOfDetail;

	controlPoints.push_back(line);

	line[0] = glm::vec3(0, 0, -2)*(float)levelOfDetail;
	line[1] = glm::vec3(1, 5, -2)*(float)levelOfDetail;
	line[2] = glm::vec3(2, 5, -2)*(float)levelOfDetail;
	line[3] = glm::vec3(3, 0, -2)*(float)levelOfDetail;

	controlPoints.push_back(line);

	line[0] = glm::vec3(0, 0, -3)*(float)levelOfDetail;
	line[1] = glm::vec3(1, 0, -3)*(float)levelOfDetail;
	line[2] = glm::vec3(2, 0, -3)*(float)levelOfDetail;
	line[3] = glm::vec3(3, 0, -3)*(float)levelOfDetail;

	controlPoints.push_back(line);

	patches.push_back(controlPoints);

	patchesX++;
	patchesZ++;
	*/
	//Increment through all patches, using offsets to calculate the new position of each position inside the patch using the pre-calculated UV's
	std::vector<glm::vec3> tmp;
	
	for (int pZ = 0; pZ < patchesZ; pZ++) {

		for (int pX = 0; pX < patchesX; pX++) {
			int thisPatchOffset = pX + (pZ * patchesX);

			int thisPatchStartingPositionOffset = (pX * levelOfDetail * 3) + ((pZ * levelOfDetail * 3) * (patchesX * levelOfDetail * 3));
			
			for (int v = 0; v < PUVCoordsCount; v++) {
				for (int u = 0; u < PUVCoordsCount; u++) {
					int posOffset = thisPatchStartingPositionOffset + (u + (v*xIndices));

					positions[posOffset] = BezierSurface(patches[thisPatchOffset], UVs[u + (v * PUVCoordsCount)].x, UVs[u + (v * PUVCoordsCount)].y);
					tmp.push_back(positions[posOffset]);
				}
			}
		}
	}
	//Alternate way to increment through all patches calculating the new position inside the patch with the pre-calculated UV's
	/*
	int patchesX = heightImageDataWidth / 3;
	int patchesZ = heightImageDataHeight / 3;
	for (int pZ = 0; pZ < 1; pZ++) {

		for (int pX = 0; pX < 1; pX++) {
			int patchOffset = pX + (pZ * patchesX);
			int patchPositionOffset = patchOffset * (levelOfDetail * 3) * 4;
			int posOffsetZ = 0;

			for (int patchPosZ = 0; patchPosZ < levelOfDetail * 4; patchPosZ++) {
				int posOffsetX = 0;

				for (int patchPosX = 0; patchPosX < levelOfDetail * 4; patchPosX++) {
					int posOffset = posOffsetX + (posOffsetZ * zIndices);
					positions[patchPositionOffset + posOffset] = BezierSurface(patches[patchOffset], UVs[patchPosX + (patchPosZ * (levelOfDetail * 4))].x, UVs[patchPosX + (patchPosZ * levelOfDetail * 4)].y);
					posOffsetX++;
				}
				posOffsetZ++;
			}
		}
	}*/
}

glm::vec3 MyView::BezierSurface(const std::vector<std::vector<glm::vec3>>& control_points, float u, float v) {
	//bezier function for each patch, this calculates the bezier accross the x axis for each z (four by four points) then bezier's the result.
	std::vector<glm::vec3> curve{ control_points.size() };
	for (unsigned int j = 0; j < control_points.size(); j++)
	{
		curve[j] = bezierCurve(control_points[j], u);
	}
	return bezierCurve(curve, v);
}

glm::vec3 MyView::bezierCurve(const std::vector<glm::vec3>& control_points, float step) {
	//standard bezier curve calculations
	float curve1 = (1 - step) * (1 - step) * (1 - step);
	float curve2 = 3 * step * (1 - step) * (1 - step);
	float curve3 = 3 * step * step * (1 - step);
	float curve4 = step * step * step;

	return (control_points[0] * curve1 + control_points[1] * curve2 +
		control_points[2] * curve3 + control_points[3] * curve4);
}