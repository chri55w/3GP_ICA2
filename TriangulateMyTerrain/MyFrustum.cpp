#include "MyFrustum.h"
#include <math.h>

MyFrustum *MyFrustum::default_frustum_;

MyFrustum *MyFrustum::defaultFrustum() {
	if (default_frustum_ == nullptr) {
		default_frustum_ = new MyFrustum;

	}
	return default_frustum_;
}

void MyFrustum::createFrustum() {

}