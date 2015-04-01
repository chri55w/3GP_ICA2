#pragma once

#include <SceneModel/Context.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <glm/glm.hpp>

class MyView : public tygra::WindowViewDelegate {
	public:

		MyView();

		~MyView();

		void setScene(std::shared_ptr<const SceneModel::Context> scene);

		void toggleShading();

	private:

		void windowViewWillStart(std::shared_ptr<tygra::Window> window) override;

		void windowViewDidReset(std::shared_ptr<tygra::Window> window, int width, int height) override;

		void windowViewDidStop(std::shared_ptr<tygra::Window> window) override;

		void windowViewRender(std::shared_ptr<tygra::Window> window) override;

	private:

		std::shared_ptr<const SceneModel::Context> scene_;

		GLuint terrain_sp_{ 0 };
		GLuint shapes_sp_{ 0 };

		bool shade_normals_{ false };

		struct MeshGL {
			GLuint position_vbo{ 0 };
			GLuint normal_vbo{ 0 };
			GLuint element_vbo{ 0 };
			GLuint vao{ 0 };
			int element_count{ 0 };
		};
		MeshGL terrain_mesh_;

		enum {
			kVertexPosition = 0,
			kVertexNormal = 1,
		};

		GLuint cube_vao_{ 0 };
		GLuint cube_vbo_{ 0 };

		
		const int subDivisionsZ = 256;
		const int subDivisionsX = 256;

		// Indices uses number of subdivisions plus one for the final line of points for the quads due to each quad sharing
		//                                                                 one point along each axis except the final point
		const int zIndices = subDivisionsZ + 1;
		const int xIndices = subDivisionsX + 1;


};
