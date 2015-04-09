#pragma once
class MyFrustum {
	public:
		static MyFrustum *defaultFrustum();

		static void createFrustum();

	private:
		static float frustumPlanes[6][4];

		static MyFrustum *default_frustum_;
};

