#include "Engine/Core/Application.h"

#include "Engine/ComponentSystem/Components/Camera.h"
#include "Engine/ComponentSystem/Components/MeshComponent.h"
#include "Engine/ComponentSystem/Components/RigidBody.h"
#include "Engine/ComponentSystem/Components/Collider.h"
#include "Engine/Mxm/Vec3.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"
#include "Engine/Core/ResourceManager.h"
#include "Engine/Core/AudioManager.h"
#include "Engine/Core/TextureManager.h"
#include "Engine/Animation/Animator.h"
#include "Engine/Animation/Animations/TransformAnim.h"
#include "Engine/Animation/Animations/SetColorAnim.h"
#include "Engine/Animation/Animations/WaitAnim.h"

#include "Engine/UI/UISystem.h"
#include "Engine/UI/UIButton.h"

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
	Mxm::Vec3 _gunBulletOffset = Mxm::Vec3(0.0f, 0.0f, 1.0f);

	Mxm::Vec2 _inputDir{};

	float _gunShootSpeed = 0.9f;
	float _gunTimer = _gunShootSpeed;

	float _gunRecoil = 8.0f;

	float _groundSpeed = 70.0f;
	float _airSpeed = 5.0f;

	bool _menu = true;
	bool _prevMenu = false;

	void createObject(const std::string& name, const std::string& modelName, const Mxm::Vec3& pos, const Mxm::Vec3& scale, const Mxm::Vec3& rotation, Color color) {
		auto obj = _activeScene->createObject(name, "map");
		obj->addComponent<MeshComponent>(ResourceManager::getInstance().getModel(modelName), color);
		obj->transform().setPosition(pos);
		obj->transform().setScale(scale);
		obj->transform().setRotation(rotation * Mxm::Consts::DEG2RAD);

		obj->addComponent<Collider>()->generateFromMesh();
	}

	void createMap() {
		createObject("path", "cube", Mxm::Vec3(0.0f, 0.0f, 0.1f), Mxm::Vec3(5.0f, 0.1f, 30.0f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(150, 150, 150));

		createObject("main_building", "cube", Mxm::Vec3(0.0f, 3.0f, 8.0f), Mxm::Vec3(6.0f, 3.0f, 4.0f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(200, 80, 60));
		createObject("roof", "frustum", Mxm::Vec3(0.0f, 9.0f, 8.0f), Mxm::Vec3(6.5f, 3.0f, 4.5f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(110, 50, 50));

		createObject("tree1_trunk", "cube", Mxm::Vec3(-8.0f, 1.5f, 5.0f), Mxm::Vec3(0.5f, 2.0f, 0.5f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(120, 80, 40));
		createObject("tree1_top", "frustum", Mxm::Vec3(-8.0f, 3.5f, 5.0f), Mxm::Vec3(2.0f, 1.5f, 2.0f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(40, 150, 40));

		createObject("tree2_trunk", "cube", Mxm::Vec3(10.0f, 1.5f, -3.0f), Mxm::Vec3(0.5f, 2.0f, 0.5f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(120, 80, 40));
		createObject("tree2_top", "frustum", Mxm::Vec3(10.0f, 3.5f, -3.0f), Mxm::Vec3(2.0f, 1.5f, 2.0f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(40, 150, 40));

		for (int i = -4; i <= 4; i++) {
			createObject("fence_left_" + std::to_string(i), "cube", Mxm::Vec3(-12.0f, 0.8f, i * 3.0f), Mxm::Vec3(0.2f, 1.0f, 0.2f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(200, 200, 200));
			createObject("fence_right_" + std::to_string(i), "cube", Mxm::Vec3(12.0f, 0.8f, i * 3.0f), Mxm::Vec3(0.2f, 1.0f, 0.2f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(200, 200, 200));
		}

		createObject("rock1", "cube", Mxm::Vec3(-12.0f, 0.3f, -20.0f), Mxm::Vec3(1.5f, 1.5f, 1.5f), Mxm::Vec3(45.0f, 45.0f, 0.0f), Color(150, 150, 160));
		createObject("rock2", "cube", Mxm::Vec3(-14.5f, 1.0f, -17.0f), Mxm::Vec3(3.0f, 3.0f, 3.0f), Mxm::Vec3(20.0f, 20.0f, 0.0f), Color(140, 140, 150));

		createObject("small_house", "cube", Mxm::Vec3(7.0f, 2.0f, -9.5f), Mxm::Vec3(2.0f, 2.0f, 2.5f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(80, 120, 200, 20));
		createObject("small_roof", "frustum", Mxm::Vec3(7.0f, 5.5f, -9.5f), Mxm::Vec3(2.5f, 1.5f, 3.0f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(200, 60, 50, 20));

		createObject("box", "monkey", Mxm::Vec3(0.0f, 3.0f, -4.0f), Mxm::Vec3(0.8f, 0.8f, 0.8f), Mxm::Vec3(0.0f, 0.0f, 0.0f), Color(255, 60, 50, 255));

		auto obj = _activeScene->createObject("ground", "map");
		obj->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("plane"), "texture");
		obj->transform().setPosition(Mxm::Vec3(0.0f, 0.0f, 0.0f));
		obj->transform().setScale(Mxm::Vec3(30.0f, 1.0f, 30.0f));

		obj->addComponent<Collider>()->generateFromMesh();
	}

	void prepareGame() {
		ResourceManager::getInstance().loadModelFromFile("cube", "models/cube.obj");
		ResourceManager::getInstance().loadModelFromFile("textureCube", "models/textureCube.obj");
		ResourceManager::getInstance().loadModelFromFile("plane", "models/plane.obj");
		ResourceManager::getInstance().loadModelFromFile("frustum", "models/frustum.obj");
		ResourceManager::getInstance().loadModelFromFile("gun1", "models/gun1.obj");
		ResourceManager::getInstance().loadModelFromFile("monkey", "models/gener.obj");

		AudioManager::getInstance().loadSound("shoot", "sounds/shoot.mp3", false);
		AudioManager::getInstance().loadSound("click", "sounds/click.mp3", false);
		AudioManager::getInstance().loadSound("background", "sounds/background.mp3", true);

		AudioManager::getInstance().setVolume("shoot", 0.5f);

		TextureManager::getInstance().loadTexture("texture", "textures/wall.jpg");

		_activeScene = SceneManager::getInstance().createScene();

		_mainCamera = _activeScene->createObject("camera");
		_mainCamera->addComponent<Camera>();
		_mainCamera->transform().translate(_cameraOffset);

		_activeScene->setMainCamera(_mainCamera);
		createMap();

		//player body
		_object = _activeScene->createObject("body");
		_object->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("cube"), Color(255, 200, 70, 255));
		_object->transform().setScale(Mxm::Vec3(0.7f, 2.0f, 0.7f));
		_object->transform().setPosition(Mxm::Vec3(0.0f, 15.0f, 0.0f));
		_object->transform().translate(Mxm::Vec3(0.0f, 8.0f, 0.0f));

		_object->addComponent<RigidBody>();
		_object->getComponent<RigidBody>()->setGravity(Mxm::Vec3(0.0f, -15.0f, 0.0f));
		_object->getComponent<RigidBody>()->setLinearDamping(7.0f);
		_object->getComponent<RigidBody>()->setAirDamping(0.7f);

		_object->addComponent<Collider>()->generateSimpleFromMesh();

		_gun = _activeScene->createObject("gun1");
		_gun->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("gun1"));
		_gun->transform().setScale(Mxm::Vec3(0.5f, 0.5f, 0.5f));
		_gun->transform().translate(_gunOffset);

		_gun->transform().setParent(&_mainCamera->transform());
	}

	void start() override {
		prepareGame();
		setBackgroundColor(Color(100, 120, 100, 255));

		auto menuScene = SceneManager::getInstance().createScene();
		SceneManager::getInstance().setActiveScene(menuScene);

		UISystem& ui = UISystem::getInstance();
		auto* screen = ui.addScreen("game");
		auto* startScreen = ui.addScreen("startScreen");
		ui.setCurrentScreen("game");

		auto* rect = startScreen->add<UIRect>(Mxm::Vec2i(398, 298), Mxm::Vec2i(4, 4), Color(255, 255, 255));

		auto* button = screen->add<UIButton>(Mxm::Vec2i(50, 50), Mxm::Vec2i(300, 40), "Play game", Mxm::Vec2i(10, 10), 2,
			Color(255, 255, 255), Color(100, 200, 50), Color(0, 0, 0), Color(50, 50, 50));
		button->setOnPress([this]() { 
			SceneManager::getInstance().setActiveScene(_activeScene);
			UISystem::getInstance().setCurrentScreen("startScreen");
			AudioManager::getInstance().playSound("background");
			setBackgroundColor(Color(100, 90, 240, 255));
			_menu = false;
			});
		button->setOnHover([this]() { AudioManager::getInstance().playSound("click"); });
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
		_activeScene->getAnimator().add<SetColorAnim>("trace_" + std::to_string(traceCounter++), trace->getComponent<MeshComponent>(), Color(100, 100, 100, 0),
			2.0f, Animation::InterpolationType::LINEAR, [trace, this]() {
				_activeScene->removeObject(trace);
			});
	}

	float anim_time = 0.0f;
	bool in_move = false;
	bool is_gun_animating = false;

	float yaw{}, pitch{};

	void update() override {
		if (_menu) { _prevMenu = true; return; }
		Input::setMouseLockState(true);

		if (Input::isKeyPressed(Key::F1)) setDrawFrame(false);
		if (Input::isKeyPressed(Key::F2)) setDrawFrame(true);

		Mxm::Vec2 mouseDelta = Input::getMouseDelta() * 0.003f;
		pitch += mouseDelta.y;
		yaw += mouseDelta.x;

		pitch = fmaxf(-90.0f * Mxm::Consts::DEG2RAD, fminf(90.0f * Mxm::Consts::DEG2RAD, pitch));

		auto& transform = _mainCamera->transform();
		auto& obj_transform = _object->transform();
		auto& gun_transform = _gun->transform();

		auto obj_rigid = _object->getComponent<RigidBody>();

		transform.setRotation(Mxm::Vec3(pitch, 0.0f, 0.0f));
		_object->transform().setRotation(Mxm::Vec3(0.0f, yaw, 0.0f));

		transform.setRotation(Mxm::Vec3(transform.getRotation().x, obj_transform.getRotation().y, 0.0f));
		transform.setPosition(obj_transform.getPosition() + _cameraOffset);

		in_move = false;
		float speed = obj_rigid->isCollision() ? _groundSpeed : _airSpeed;

		Mxm::Vec2 input;
		if (Input::isKeyDown(Key::W)) {
			input.y += 1.0f;
			in_move = true;
		}
		if (Input::isKeyDown(Key::S)) {
			input.y -= 1.0f;
			in_move = true;
		}
		if (Input::isKeyDown(Key::D)) {
			input.x += 1.0f;
			in_move = true;
		}
		if (Input::isKeyDown(Key::A)) {
			input.x -= 1.0f;
			in_move = true;
		}
		input = input.normalized();
		obj_rigid->addImpulse((obj_transform.getRight() * input.x + obj_transform.getForward() * input.y) * speed * Time::deltaTime());

		if (in_move && obj_rigid->isCollision()) {
			anim_time += Time::deltaTime();

			_gun->transform().translate(Mxm::Vec3(sinf(anim_time * 7.0f) * 0.009f, sinf(anim_time * 7.0f) * 0.008f, 0.0f));

			is_gun_animating = false;
		}
		else {
			if (!is_gun_animating) {
				_activeScene->getAnimator().add<TranslateToAnim>("gun_back_anim", _gun, _gunOffset, 0.1f, Animation::InterpolationType::LINEAR, [this]() { is_gun_animating = false; });
				anim_time = 0.0f;
				is_gun_animating = true;
			}
		}

		if (Input::isKeyPressed(Key::Space) && obj_rigid->isCollision()) {
			obj_rigid->addImpulse(obj_transform.getUp() * 9.0f);
		}

		if (Input::isKeyPressed(Key::R)) {
			_activeScene->getAnimator().add<RotateByAnim>("rotate", _activeScene->getFirstObjectWithName("box"),
				Mxm::Vec3(Mxm::Consts::DEG2RAD * 90.0f, Mxm::Consts::DEG2RAD * 90.0f, 0.0f), 5.0f, Animation::InterpolationType::ELASTIC_COS_BOUNCE);
		}

		_gunTimer += Time::deltaTime();
		if (Input::isMouseButtonPressed(MouseButton::MOUSE0) && _gunTimer > _gunShootSpeed && !_prevMenu) {
			AudioManager::getInstance().playSound("shoot");

			obj_rigid->addImpulse(-transform.getForward() * _gunRecoil);

			Mxm::Vec3 gunWorldPos = gun_transform.getWorldPosition();
			Mxm::Vec3 gunForward = gun_transform.getForward();

			Mxm::Vec3 bulletStartPos = gunWorldPos + gunForward * _gunBulletOffset.z
				+ gun_transform.getUp() * _gunBulletOffset.y
				+ gun_transform.getRight() * _gunBulletOffset.x;

			IntersectionInfo info;
			if (SceneManager::getInstance().getActiveScene()->rayCast(transform.getPosition(), transform.getForward(), info, { "map" })) {
				createFireTrace(bulletStartPos, info.point, (info.point - bulletStartPos).length());

				std::shared_ptr<GameObject> bulletDot = _activeScene->createObject("bullet_dot");
				bulletDot->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("cube"), Color(0, 0, 0));
				bulletDot->transform().setScale(Mxm::Vec3(0.08f));
				bulletDot->transform().setPosition(info.point);

				static int anim_id = 0;
				_activeScene->getAnimator().add<WaitAnim>("wait_to_delete" + std::to_string(anim_id++), 3.0f, Animation::InterpolationType::EASY_OUT,
					[bulletDot, this]() { _activeScene->removeObject(bulletDot); });
			}
			else {
				Mxm::Vec3 bulletEndPos = bulletStartPos + gunForward * 100.0f;
				createFireTrace(bulletStartPos, bulletEndPos, 100.0f);
			}

			_activeScene->getAnimator().add<RotateByAnim>("gun_rotate", _gun, Mxm::Vec3(-Mxm::Consts::PI * 2.0f, 0.0f, 0.0f),
			_gunShootSpeed, Animation::InterpolationType::EASY_OUT);

			_gunTimer = 0.0f;
		}

		if (obj_transform.getPosition().y < -100.0f) {
			obj_transform.setPosition(Mxm::Vec3(0.0f, 5.0f, 0.0f));
		}

		_prevMenu = false;
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