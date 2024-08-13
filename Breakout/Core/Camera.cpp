#include "Core/Camera.h"

#include "Core/Window.h"
#include "Util/Math.h"

#include <cassert>

void Camera::Update(float deltaSeconds)
{
	assert(CurrentWindowProxy::IsValid());

	sf::View view = CurrentWindowProxy::Get().GetView();
	view.setCenter(_center + _offset);
	CurrentWindowProxy::Get().SetView(view);

	_offset *= 1.f - deltaSeconds * SHAKE_RECOVERY_SPEED;
}

void Camera::DoCameraShake()
{
	_offset.x = RandomRange(-1.f, 1.f) >= 0.f ? SHAKE_AMOUNT_X : -SHAKE_AMOUNT_X;
	_offset.y = RandomRange(-1.f, 1.f) >= 0.f ? SHAKE_AMOUNT_Y : -SHAKE_AMOUNT_Y;
}
