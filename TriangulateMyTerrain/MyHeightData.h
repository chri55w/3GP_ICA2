#pragma once

#include <SceneModel/Context.hpp>
#include <tygra/FileHelper.hpp>
#include <memory>

class MyHeightData {
	public:
		static int getDataHeight() { return imageHeight_; }
		static int getDataWidth() { return imageWidth_; }

		static void loadHeightMap(std::shared_ptr<const SceneModel::Context> scene_);
		static float getHeightValue(int xPos, int yPos);


	private:
		static int imageHeight_;
		static int imageWidth_;
		static tygra::Image heightImage_;

};

