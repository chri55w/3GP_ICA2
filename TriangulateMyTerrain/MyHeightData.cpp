#include "MyHeightData.h"


int MyHeightData::imageHeight_ = 0;
int MyHeightData::imageWidth_ = 0;
tygra::Image MyHeightData::heightImage_ = tygra::Image();

void MyHeightData::loadHeightMap(std::shared_ptr<const SceneModel::Context> scene_) {
	heightImage_ = tygra::imageFromPNG(scene_->getTerrainHeightMapName());

	const int heightImageWidth = heightImage_.width();
	const int heightImageHeight = heightImage_.height();

	imageHeight_ = heightImageHeight;
	imageWidth_ = heightImageWidth;
}

float MyHeightData::getHeightValue(int xPos, int yPos) {
	//fetch height info from height image and scale it to a 0-1 value.
	float height = ((*(uint8_t*)heightImage_(xPos, yPos)) / 256.f);

	return height;
}