#include "MyView.hpp"
#include <tygra/FileHelper.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#include "MyUtilities.h"
#include "MyFrustum.h"

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
	
	const int spacingZ = sizeZ / subDivisionsZ;
	const int spacingX = sizeX / subDivisionsX;


	for (int z = 0; z < zIndices; z++) {

		for (int x = 0; x < xIndices; x++) { 
			
			//glm::vec3 new_pos = glm::vec3(spacingX * x, 0, -spacingZ * z);
			glm::vec3 new_pos = glm::vec3(x, 0, -z);

			positions.push_back(new_pos);
		}
	}
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

	//applyHeightMap(sizeY, positions);

	terrainNormals = MyUtilities::calculateNormals(elements, positions);

	applyBezier(positions);

	MyUtilities::applyNoiseToTerrain(positions, &terrainNormals, 4, 1.f / 512.0f, 2.0, 0.5, 4);

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

	//Create frustrum planes
	MyFrustum::defaultFrustum()->createFrustum();

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

	for (const auto& pos : scene_->getAllShapePositions()) {
		world_xform = glm::translate(glm::mat4(1), glm::vec3(pos.x, 64, -pos.y));
		view_world_xform = view_xform * world_xform;

		view_world_xform_id = glGetUniformLocation(shapes_sp_, "view_world_xform");
		glUniformMatrix4fv(view_world_xform_id, 1, GL_FALSE, glm::value_ptr(view_world_xform));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
void MyView::applyHeightMap(float sizeY, std::vector<glm::vec3> &positions) {

	tygra::Image height_image = tygra::imageFromPNG(scene_->getTerrainHeightMapName());

	//      Height Image Width and Height are LESS one to put values in range of 0-height-1 and 0-width-1
	const float heightImageWidth = height_image.width() - 1;
	const float heightImageHeight = height_image.height() - 1;

	//      Modified X and Modified Z are PLUS one to return values to 1-height and 1-width after the division.
	for (int z = 0; z < zIndices; z++) {
		for (int x = 0; x < xIndices; x++) {

			int modifiedX = (heightImageWidth / xIndices) * x + 1;
			int modifiedZ = (heightImageHeight / zIndices) * z + 1;

			//   Position height fetches a height from the height image using the modified X coordinate and the flipped
			//                                                    modified Z which accounts for direction of the cubes.
			//                                         The Z value is also flipped in the creation of the new position.
			float heightMapY = (*(uint8_t*)height_image(modifiedX, modifiedZ));

			//divide height map y value by 255 to push it into a 0-1 value ready to be scaled
			heightMapY /= 255.0f;

			//scale the height map y value up to the provided size y value;
			heightMapY *= sizeY;

			positions[x + z * xIndices].y = heightMapY;
		}
	}
}

void MyView::applyBezier(std::vector<glm::vec3> &positions) {

	int heightImageWidth = xIndices/4;
	int heightImageHeight = zIndices/4;

	std::vector<glm::vec3> ctrlPositions;

	int offsetIntoPositions = 0;
	for (int z = 0; z < heightImageHeight-1; z++) {
		for (int x = 0; x < heightImageWidth; x++) {

			ctrlPositions.push_back(positions[offsetIntoPositions]);
			ctrlPositions.push_back(positions[offsetIntoPositions + 4]);
			ctrlPositions.push_back(positions[offsetIntoPositions + (xIndices * 4)]);
			ctrlPositions.push_back(positions[offsetIntoPositions + (xIndices * 4) + 4]);

			positions[offsetIntoPositions].y += 1;
			positions[offsetIntoPositions + 4].y += 1;
			positions[offsetIntoPositions + (xIndices * 4)].y += 1;
			positions[offsetIntoPositions + (xIndices * 4) + 4].y += 1;

			offsetIntoPositions += 4;
		}
		offsetIntoPositions += (xIndices * 3);
		break;
	}
	int i = 0;
}