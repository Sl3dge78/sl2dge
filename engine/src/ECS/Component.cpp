#include "Component.h"

#include "ECS/Entity.h"

namespace sl2dge {

Transform *Component::transform() {
	if (transform_ == nullptr)
		transform_ = entity()->transform();

	return transform_;
}

} // namespace sl2dge
