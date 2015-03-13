#include "FirstPersonMovement.hpp"
#include <SceneModel/Context.hpp>
#include <random>

using namespace SceneModel;

Context::Context()
{
    start_time_ = std::chrono::system_clock::now();
    time_seconds_ = 0.f;

    camera_movement_ = std::make_shared<FirstPersonMovement>();
    camera_movement_->init(glm::vec3(0, 16, 0), -0.785f, -0.25f);

    camera_ = std::make_unique<Camera>();
    camera_->setVerticalFieldOfViewInDegrees(50.f);
    camera_->setNearPlaneDistance(1);
    camera_->setFarPlaneDistance(12000);

    std::default_random_engine rand(0);
    std::uniform_real_distribution<float> x_dis(0, 1);
    std::uniform_real_distribution<float> z_dis(0, 1);
    const size_t shape_count = 10000;
    shape_positions_.resize(shape_count);
    for (size_t i = 0; i < shape_count; ++i)
    {
        const float x = x_dis(rand);
        const float z = z_dis(rand);
        const glm::vec2 pos{ x * x * x * getTerrainSizeX(),
                             z * z * z * getTerrainSizeZ() };
        shape_positions_[i] = pos;
    }
}

std::string Context::getTerrainHeightMapName() const
{
    return "terrain.png";
}

float Context::getTerrainSizeX() const
{
    return 8192;
}

float Context::getTerrainSizeY() const
{
    return 640;
}

float Context::getTerrainSizeZ() const
{
    return 8192;
}

const std::vector<glm::vec2>& Context::getAllShapePositions() const
{
    return shape_positions_;
}

void Context::update()
{
    const auto clock_time = std::chrono::system_clock::now() - start_time_;
    const auto clock_millisecs
        = std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
    const float prev_time = time_seconds_;
    time_seconds_ = 0.001f * clock_millisecs.count();
    const float dt = time_seconds_ - prev_time;

    auto camera_translation_speed = getCamera().getLinearVelocity();
    auto camera_rotation_speed = getCamera().getRotationalVelocity();
    camera_movement_->moveForward(camera_translation_speed.z * dt);
    camera_movement_->moveRight(camera_translation_speed.x * dt);
    camera_movement_->spinHorizontal(camera_rotation_speed.x * dt);
    camera_movement_->spinVertical(camera_rotation_speed.y * dt);
    camera_->setPosition(camera_movement_->position());
    camera_->setDirection(camera_movement_->direction());
}

float Context::getTimeInSeconds() const
{
    return time_seconds_;
}

const Camera& Context::getCamera() const
{
    return *camera_;
}

Camera& Context::getCamera()
{
    return *camera_;
}
