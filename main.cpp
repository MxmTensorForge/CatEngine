#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"
#include "Engine/Core/ResourceManager.h"
#include "Engine/Mxm/Vec3.h"

#include "Engine/UI/UISystem.h"
#include "Engine/UI/UIButton.h"

#include "Engine/ComponentSystem/Components/Camera.h"
#include "Engine/ComponentSystem/Components/RigidBody.h"
#include "Engine/ComponentSystem/Components/Collider.h"

#include "Engine/Animation/Animations/TransformAnim.h"
#include "Engine/Animation/Animator.h"

class SimpleCameraGame final : public Application
{
private:
    std::shared_ptr<GameObject> _camera;
    std::shared_ptr<GameObject> _player;
    float _lookSpeed = 0.003f;

    float _yaw = 0.0f;
    float _pitch = 0.0f;
    float _radius = 5.0f;

    float _moveForce = 50.0f;
    float _jumpForce = 10.0f;

    void start() override
    {
        ResourceManager::getInstance().loadModelFromFile("plane", "models/plane.obj");
        ResourceManager::getInstance().loadModelFromFile("cube", "models/cube.obj");
        ResourceManager::getInstance().loadModelFromFile("frustum", "models/frustum.obj");
        ResourceManager::getInstance().loadModelFromFile("rocket", "models/rocket.obj");

        setBackgroundColor(Color(100, 90, 240, 255));
        Input::setMouseLockState(true);

        auto scene = SceneManager::getInstance().createScene();
        SceneManager::getInstance().setActiveScene(scene);

        _camera = scene->createObject("camera");
        _camera->addComponent<Camera>();
        scene->setMainCamera(_camera);

        _player = scene->createObject("player");
        _player->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("cube"), Color(255, 255, 255));
        _player->addComponent<Collider>()->generateSimpleFromMesh();

        auto rb = _player->addComponent<RigidBody>();
        rb->setLinearDamping(5.0f);
        rb->setAirDamping(0.0f);
        rb->setAngularDamping(0.0f);
        rb->setMomentOfInertia(Mxm::Vec3(1.0f, 1.0f, 1.0f));

        _player->transform().translate(Mxm::Vec3(0.0f, 15.0f, 0.0f));
        _player->transform().scale(Mxm::Vec3(1.0f, 1.0f, 2.0f));

        auto plane = scene->createObject("plane");
        plane->addComponent<MeshComponent>(ResourceManager::getInstance().getModel("plane"), Color(200, 0, 130));
        plane->addComponent<Collider>()->generateFromMesh();

        plane->transform().scale(Mxm::Vec3(30.0f, 30.0f, 30.0f));

        _camera->transform().setPosition(Mxm::Vec3(0.0f, 0.0f, -8.0f));
    }

    void update() override
    {
        Mxm::Vec2 mouseDelta = Input::getMouseDelta() * _lookSpeed;

        _yaw -= mouseDelta.x;
        _pitch += mouseDelta.y;

        _pitch = fmaxf(-89.9f * Mxm::Consts::DEG2RAD, fminf(89.9f * Mxm::Consts::DEG2RAD, _pitch));

        Mxm::Vec3 offset(
            cosf(_yaw) * cosf(_pitch),
            sinf(_pitch),
            sinf(_yaw) * cosf(_pitch)
        );
        _camera->transform().setPosition(_player->transform().getPosition() + offset * _radius);
        _camera->transform().setLookAt(_player->transform().getPosition());

        _radius -= Input::getMouseWheel().y;

        auto rb = _player->getComponent<RigidBody>();

        Mxm::Vec2 input;
        if (Input::isKeyDown(Key::W)) input.y += 1.0f;
        if (Input::isKeyDown(Key::S)) input.y -= 1.0f;
        if (Input::isKeyDown(Key::D)) input.x += 1.0f;
        if (Input::isKeyDown(Key::A)) input.x -= 1.0f;

        Mxm::Vec3 inputDir = Mxm::Vec3(input.x, 0.0f, input.y).normalized();
        if (inputDir.length() >= 0.1f) {
            Mxm::Vec3 cameraAngle = _camera->transform().getRotation();
            Mxm::Vec3 playerAngle = _player->transform().getRotation();

            float diffAngle = (cameraAngle.y - playerAngle.y);
            if (diffAngle > Mxm::Consts::PI) diffAngle -= 2 * Mxm::Consts::PI;
            if (diffAngle < -Mxm::Consts::PI) diffAngle += 2 * Mxm::Consts::PI;

            playerAngle.y = playerAngle.y + (diffAngle) * Time::deltaTime() * 5.0f;

            _player->transform().setRotation(playerAngle);

            Mxm::Vec3 playerForward = _player->transform().getForward();
            Mxm::Vec3 playerRight = _player->transform().getRight();
            Mxm::Vec3 moveDirection = (playerForward * inputDir.z + playerRight * inputDir.x).normalized();

            rb->addForce(moveDirection * _moveForce);
        }

        if (Input::isKeyPressed(Key::Space)) {
            rb->addImpulse(Mxm::Vec3(Mxm::Vec3(0.0f, _jumpForce, 0.0f)));
        }
    }

    void fixedUpdate() override {
    }

public:
    SimpleCameraGame() : Application() {}
};

int main()
{
    SimpleCameraGame game;
    game.run();
    return 0;
}