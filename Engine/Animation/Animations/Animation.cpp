#include "Animation.h"
#include "../Interpolation.h"
#include "../../Mxm/Consts.h"

#include "../../Core/Time.h"

Animation::Animation(float duration, InterpolationType type, const CompleteCallback& callback, const Mxm::Vec2& point0, const Mxm::Vec2& point1)
	: _duration(duration), _type(type), _onComplete(callback), _point0(point0), _point1(point1) {

}

bool Animation::updateState() noexcept {
	if (_isFinished || _duration < Mxm::Consts::EPS) {
		return false;
	}

	_elapsed += Time::deltaTime();

	float t = _elapsed / _duration;
	if (t > 1.0f) t = 1.0f;

	switch (_type)
	{
	case InterpolationType::LINEAR:
		_progress = Interpolation::Linear(t) / Interpolation::Linear(1.0f);
		break;
	case InterpolationType::COS:
		_progress = Interpolation::Cos(t) / Interpolation::Cos(1.0f);
		break;
	case InterpolationType::COS_BOUNCE:
		_progress = Interpolation::CosBounce(t) / Interpolation::CosBounce(1.0f);
		break;
	case InterpolationType::ABS_COS_BOUNCE:
		_progress = Interpolation::AbsCosBounce(t) / Interpolation::AbsCosBounce(1.0f);
		break;
	case InterpolationType::EASY_OUT:
		_progress = Interpolation::EasyOut(t) / Interpolation::EasyOut(1.0f);
		break;
	case InterpolationType::BEZIER:
		_progress = Interpolation::Bezier(t, _point0, _point1);
		break;
	case InterpolationType::CUSTOM:
		_progress = Interpolation::Custom(t) / Interpolation::Custom(1.0f);
		break;
	}

	_deltaprogress = _progress - _lastprogress;
	_lastprogress = _progress;

	update();

	if (t >= 1.0f) {
		_isFinished = true;
		if (_onComplete) {
			_onComplete();
		}
	}

	return !_isFinished;
}