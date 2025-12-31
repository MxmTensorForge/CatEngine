#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Component.h"
#include "../../Mxm/Vec3.h"
#include "../../Geometry/AABB.h"
#include "../../Geometry/MeshData.h"
#include "../../Physics/CollisionResult.h"
#include <vector>
#include <memory>
#include <functional>

class Collider final : public Component
{
private:
	std::vector<Mxm::Vec3> _vertices{};
	std::shared_ptr<MeshData> _meshData = nullptr;
	bool _useSimple = false;

	AABB _localAABB{};
	AABB _worldAABB{};

	bool _needsRecalc = true;
	bool _isTrigger = false;

	using TriggerCallback = std::function<void(const std::shared_ptr<GameObject>&)>;
	TriggerCallback _triggerOnCallback;
	TriggerCallback _triggerExitCallback;
	TriggerCallback _triggerStayCallback;

	using CollisionCallback = std::function<void(const std::shared_ptr<GameObject>&, const CollisionResult&)>;
	CollisionCallback _collisionOnCallback;
public:
	void start() override;
	void update() override;

	static AABB calculateAABB(const std::vector<Mxm::Vec3>& verts, float eps) noexcept;
	static bool checkAABB(const Collider& collider1, const Collider& collider2) noexcept;

	void generateLocalAABB() noexcept;
	const AABB& getLocalAABB() const noexcept { return _localAABB; }

	void generateFromMesh();
	void generateSimpleFromMesh();

	const std::vector<Mxm::Vec3>& getVertices() const noexcept;

	void setTriggerOnCallback(const TriggerCallback& c) noexcept { _triggerOnCallback = c; }
	void triggerOnCallback(const std::shared_ptr<GameObject>& obj) noexcept { if (_triggerOnCallback) _triggerOnCallback(obj); }

	void setTriggerExitCallback(const TriggerCallback& c) noexcept { _triggerExitCallback = c; }
	void triggerExitCallback(const std::shared_ptr<GameObject>& obj) noexcept { if (_triggerExitCallback) _triggerExitCallback(obj); }

	void setTriggerStayCallback(const TriggerCallback& c) noexcept { _triggerStayCallback = c; }
	void triggerStayCallback(const std::shared_ptr<GameObject>& obj) noexcept { if (_triggerStayCallback) _triggerStayCallback(obj); }

	void setCollisionOnCallback(const CollisionCallback& c) noexcept { _collisionOnCallback = c; }
	void collisionOnCallback(const std::shared_ptr<GameObject>& obj, const CollisionResult& result) noexcept { if (_collisionOnCallback) _collisionOnCallback(obj, result); }

	bool isTrigger() const noexcept { return _isTrigger; }
	void setTrigger(bool state) noexcept { _isTrigger = state; }
};

#endif // !COLLIDER_H
