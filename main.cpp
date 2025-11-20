#include "Engine/Core/Application.h"

#include "Engine/ComponentSystem/Components/Camera.h"
#include "Engine/ComponentSystem/Components/MeshComponent.h"
#include "Engine/ComponentSystem/Components/RigidBody.h"
#include "Engine/ComponentSystem/Components/Collider.h"
#include "Engine/Mxm/Vec3.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"
#include "Engine/Core/ResourceManager.h"
#include "Engine/Animation/Animator.h"
#include "Engine/Animation/Animations/TransformAnim.h"
#include "Engine/Animation/Animations/SetColorAnim.h"
#include "Engine/Animation/Animations/WaitAnim.h"

#include "Engine/Animation/Interpolation.h"

#include <iostream>

class Game final : public Application
{
private:
	std::shared_ptr<GameObject> _mainCamera;
	std::shared_ptr<GameObject> _object;
	std::shared_ptr<GameObject> _gun;

	std::shared_ptr<Scene> _activeScene;

	Mxm::Vec3 _cameraOffset = Mxm::Vec3(0.0f, 1.4f, 0.0f);
	Mxm::Vec3 _gunOffset = Mxm::Vec3(0.55f, -0.35f, 1.0f);
	Mxm::Vec3 _gunBulletOffset = Mxm::Vec3(0.0f, 0.0f, 1.5f);

	float _gunShootSpeed = 0.8f;
	float _gunTimer = _gunShootSpeed;

	float _groundSpeed = 1.1f;
	float _airSpeed = 0.1f;

	void createObject(const std::string& name, const std::string& modelName, const Mxm::Vec3& pos, const Mxm::Vec3& scale, const Mxm::Vec3& rotation, Color color) {
		auto obj = _activeScene->createObject(name);
		obj->addComponent<MeshComponent>(ResourceManager::getInstance().getModel(modelName), color);
		obj->transform().setPosition(pos);
		obj->transform().setScale(scale);
		obj->transform().setRotation(rotation * Mxm::Consts::DEG2RAD);

		obj->addComponent<Collider>()->generateFromMesh();
	}

	void createMap() {
		createObject("floor", "plane", Mxm::Vec3(0.0f, 0.0f, 0.0f), Mxm::Vec3(20.0f, 1.0f, 20.0f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(20, 255, 10));
	}

	void start() override {
		ResourceManager::getInstance().loadModelFromFile("cube", "models/cube.obj");
		ResourceManager::getInstance().loadModelFromFile("plane", "models/plane.obj");
		ResourceManager::getInstance().loadModelFromFile("frustum", "models/frustum.obj");
		ResourceManager::getInstance().loadModelFromFile("gun1", "models/gun1.obj");

		Input::setMouseLockState(true);
		setBackgroundColor(Color(100, 90, 240, 255));

		_activeScene = getSceneManager().createScene();

		_mainCamera = _activeScene->createObject("camera");
		_mainCamera->addComponent<Camera>();
		_mainCamera->transform().translate(_cameraOffset);

		_activeScene->setMainCamera(_mainCamera);
		createMap();

		//player body
		_object = _activeScene->createObject("body");
		_object->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("cube"), Color(255, 200, 70, 255));
		_object->transform().setScale(Mxm::Vec3(0.7f, 2.0f, 0.7f));
		_object->transform().translate(Mxm::Vec3(0.0f, 8.0f, 0.0f));

		_object->addComponent<RigidBody>();
		_object->getComponent<RigidBody>()->setAcceleration(Mxm::Vec3(0.0f, -13.0f, 0.0f));
		_object->getComponent<RigidBody>()->setFriction(7.0f);
		_object->getComponent<RigidBody>()->setAirFriction(0.5f);

		_object->addComponent<Collider>()->generateSimpleFromMesh();

		_gun = _activeScene->createObject("gun1");
		_gun->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("gun1"));
		_gun->transform().setScale(Mxm::Vec3(0.5f, 0.5f, 0.5f));
		_gun->transform().translate(_gunOffset);

		_gun->transform().setParent(&_mainCamera->transform());
	}

	void createFireTrace(const Mxm::Vec3& from, const Mxm::Vec3& to, float length) {
		Mxm::Vec3 middle = (from + to) * 0.5f;
		Mxm::Vec3 direction = (to - from).normalized();

		auto trace = _activeScene->createObject("trace");
		trace->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("cube"), Color(255, 255, 255, 255));

		trace->transform().setPosition(middle);
		trace->transform().setScale(Mxm::Vec3(0.03f, 0.03f, length * 0.5f));
		trace->transform().setLookRotation(direction);

		static int traceCounter = 0;
		Animator::add<SetColorAnim>("trace_" + std::to_string(traceCounter++), trace->getComponent<MeshComponent>(), Color(100, 100, 100),
			0.5f, Animation::InterpolationType::LINEAR, [trace, this]() {
				_activeScene->removeObject(trace);
			});
	}

	float anim_time = 0.0f;
	bool in_move = false;
	bool is_gun_animating = false;

	void update() override {
		if (Input::isKeyPressed(Key::F1)) setDrawFrame(false);
		if (Input::isKeyPressed(Key::F2)) setDrawFrame(true);

		Mxm::Vec2 mouseDelta = Input::getMouseDelta() * 0.003f;

		auto& transform = _mainCamera->transform();
		auto& obj_transform = _object->transform();
		auto& gun_transform = _gun->transform();

		auto obj_rigid = _object->getComponent<RigidBody>();

		transform.rotate(Mxm::Vec3(mouseDelta.y, 0.0f, 0.0f));
		_object->transform().rotate(Mxm::Vec3(0.0f, mouseDelta.x, 0.0f));

		transform.setRotation(Mxm::Vec3(transform.getRotation().x, obj_transform.getRotation().y, 0.0f));
		transform.setPosition(obj_transform.getPosition() + _cameraOffset);

		in_move = false;
		float speed = obj_rigid->isCollision() ? _groundSpeed : _airSpeed;

		if (Input::isKeyDown(Key::W)) {
			obj_rigid->addForce(obj_transform.getForward() * speed);
			in_move = true;
		}
		if (Input::isKeyDown(Key::S)) {
			obj_rigid->addForce(-obj_transform.getForward() * speed);
			in_move = true;
		}
		if (Input::isKeyDown(Key::D)) {
			obj_rigid->addForce(obj_transform.getRight() * speed);
			in_move = true;
		}
		if (Input::isKeyDown(Key::A)) {
			obj_rigid->addForce(-obj_transform.getRight() * speed);
			in_move = true;
		}

		if (in_move && obj_rigid->isCollision()) {
			anim_time += Time::deltaTime();

			transform.translate(Mxm::Vec3(0.0f, sinf(anim_time * 10.0f) * 0.1f, 0.0f));
			_gun->transform().translate(Mxm::Vec3(sinf(anim_time * 7.0f) * 0.008f, sinf(anim_time * 7.0f) * 0.008f, 0.0f));

			is_gun_animating = false;
		}
		else {
			if (!is_gun_animating) {
				Animator::add<TranslateToAnim>("gun_back_anim", _gun, _gunOffset, 0.1f, Animation::InterpolationType::COS, [this]() { is_gun_animating = false; });
				is_gun_animating = true;
			}
		}

		if (Input::isKeyPressed(Key::Space) && obj_rigid->isCollision()) {
			obj_rigid->addForce(obj_transform.getUp() * 8.0f);
		}

		_gunTimer += Time::deltaTime();
		if (Input::isMouseButtonDown(MouseButton::MOUSE0) && _gunTimer > _gunShootSpeed) {
			obj_rigid->addForce(-transform.getForward() * 3.0f);

			Mxm::Vec3 gunWorldPos = gun_transform.getWorldPosition();
			Mxm::Vec3 gunForward = gun_transform.getForward();

			Mxm::Vec3 bulletStartPos = gunWorldPos + gunForward * _gunBulletOffset.z
				+ gun_transform.getUp() * _gunBulletOffset.y
				+ gun_transform.getRight() * _gunBulletOffset.x;

			IntersectionInfo info;
			if (getSceneManager().getActiveScene()->rayCast(transform.getPosition(), transform.getForward(), info, {"cube", "rama", "plane"})) {
				createFireTrace(bulletStartPos, info.point, (info.point - bulletStartPos).length());

				std::shared_ptr<GameObject> bulletDot = _activeScene->createObject("bullet_dot");
				bulletDot->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("cube"), Color(0, 0, 0));
				bulletDot->transform().setScale(Mxm::Vec3(0.08f));
				bulletDot->transform().setPosition(info.point);

				static int anim_id = 0;
				Animator::add<WaitAnim>("wait_to_delete" + std::to_string(anim_id++), 3.0f, Animation::InterpolationType::EASY_OUT,
					[bulletDot, this]() { _activeScene->removeObject(bulletDot); });
			}
			else {
				Mxm::Vec3 bulletEndPos = bulletStartPos + gunForward * 100.0f;
				createFireTrace(bulletStartPos, bulletEndPos, 100.0f);
			}

			Animator::add<RotateByAnim>("gun_rotate", _gun, Mxm::Vec3(-Mxm::Consts::PI * 2.0f, 0.0f, 0.0f),
				_gunShootSpeed, Animation::InterpolationType::EASY_OUT);

			_gunTimer = 0.0f;
		}

		//_activeScene->getFirstObjectWithTag("plane")->getComponent<Transform>()->rotate(Mxm::Vec3(0.001f, 0.001f, 0.0f));
	}
public:
	Game() : Application() {}
};

int main()
{
	Game game;
	game.run();

	return 0;
}