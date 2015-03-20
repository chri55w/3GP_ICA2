#include "MyView.hpp"
#include <tygra/FileHelper.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    

	const float sizeX = scene_->getTerrainSizeX();
	const float sizeY = scene_->getTerrainSizeY();
	const float sizeZ = scene_->getTerrainSizeZ();

	tygra::Image height_image = tygra::imageFromPNG(scene_->getTerrainHeightMapName());

	std::vector<glm::vec3> positions;
	std::vector<GLuint> elements;

	const int subDivisionsZ = 819;
	const int subDivisionsX = 819;
	
	const int spacingZ = sizeZ / subDivisionsZ;
	const int spacingX = sizeX / subDivisionsX;

    // Indices uses number of subdivisions plus one for the final line of points for the quads due to each quad sharing
	//                                                                 one point along each axis except the final point
	const int zIndices = subDivisionsZ + 1;
	const int xIndices = subDivisionsX + 1;

    //                    Height Image Width and Height are LESS one to put values in range of 0-height-1 and 0-width-1
	const float heightImageWidth = 255;
	const float heightImageHeight = 255;

    std::vector<double> noiseValues;

	for (int z = 0; z < zIndices; z++) {

		for (int x = 0; x < xIndices; x++) { 

            //      Modified X and Modified Z are PLUS one to return values to 1-height and 1-width after the division.
            
			int modifiedX = (heightImageWidth / subDivisionsX) * x + 1;
			int modifiedZ = (heightImageHeight / subDivisionsZ) * z + 1;

            double noiseValue = noise(x, 1, z);
            noiseValues.push_back(noiseValue);

            //   Position height fetches a height from the height image using the modified X coordinate and the flipped
            //                                                    modified Z which accounts for direction of the cubes.
            //                                         The Z value is also flipped in the creation of the new position.
            float posHeight = static_cast<float>(*(uint8_t*)height_image(-modifiedZ, modifiedX));

			glm::vec3 new_pos = glm::vec3(spacingX * x, posHeight, -spacingZ * z);

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

	glGenBuffers(1, &terrain_mesh_.element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_mesh_.element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	    elements.size() * sizeof(unsigned int),
	    elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	terrain_mesh_.element_count = elements.size();
	
	glGenBuffers(1, &terrain_mesh_.position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, terrain_mesh_.position_vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),
	             positions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &terrain_mesh_.vao);
	glBindVertexArray(terrain_mesh_.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_mesh_.element_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, terrain_mesh_.position_vbo);
	glEnableVertexAttribArray(kVertexPosition);
	glVertexAttribPointer(kVertexPosition, 3, GL_FLOAT, GL_FALSE,
	                      sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));
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
double MyView::grad(int hash, double x, double y, double z) {
    int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
        v = h<4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
double MyView::noise(double x, double y, double z) {
    int X = (int)ceil(x) & 255,                  // FIND UNIT CUBE THAT
        Y = (int)ceil(y) & 255,                  // CONTAINS POINT.
        Z = (int)ceil(z) & 255;
    x -= ceil(x);                                // FIND RELATIVE X,Y,Z
    y -= ceil(y);                                // OF POINT IN CUBE.
    z -= ceil(z);
    double u = fade(x),                                // COMPUTE FADE CURVES
        v = fade(y),                                // FOR EACH OF X,Y,Z.
        w = fade(z);
    int A = permutation[X] + Y, AA = permutation[A] + Z, AB = permutation[A + 1] + Z,      // HASH COORDINATES OF
        B = permutation[X + 1] + Y, BA = permutation[B] + Z, BB = permutation[B + 1] + Z;      // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z),  // AND ADD
        grad(permutation[BA], x - 1, y, z)), // BLENDED
        lerp(u, grad(permutation[AB], x, y - 1, z),  // RESULTS
        grad(permutation[BB], x - 1, y - 1, z))),// FROM  8
        lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1),  // CORNERS
        grad(permutation[BA + 1], x - 1, y, z - 1)), // OF CUBE
        lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1),
        grad(permutation[BB + 1], x - 1, y - 1, z - 1))));
}
int MyView::permutation[512] = { 151, 160, 137, 91, 90, 15,
131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};
/*
static {
    for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i]; 
    }*/ 