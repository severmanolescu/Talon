#include "Rigidbody.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "CollisionManager.h"

#include "iostream"

/// @todo Fix the collision with child objects

void Rigidbody::Awake() {
	if (game_object_) {
		transform_ = game_object_->GetComponent<Transform>();

		if (!transform_) {
			std::cerr << "[Rigidbody2D] Error: No transform found on object: " << game_object_->name_;
		}
	}
}

void Rigidbody::AddForce(const Vector2& force) {
	acceleration_.x += force.x / mass_;
	acceleration_.y += force.y / mass_;
}

void Rigidbody::SetVelocity(const Vector2& new_velocity) {
	velocity_ = new_velocity;
}

void Rigidbody::AddVelocity(const Vector2& delta_velocity) {
	velocity_.x += delta_velocity.x;
	velocity_.y += delta_velocity.y;
}

void Rigidbody::ApplyImpulse(const Vector2& impulse) {
	velocity_.x += impulse.x / mass_;
	velocity_.y += impulse.y / mass_;
}

void Rigidbody::OnCollision() {
	velocity_ = { 0.0f, 0.0f };
	acceleration_ = { 0.0f, 0.0f };
}

void Rigidbody::CheckAndResolveCollision() {
	auto transform = game_object_->GetTransform();
	auto collider = game_object_->GetComponent<BoxCollider>();
	if (!transform || !collider) return;

	int stepsX = static_cast<int>(std::abs(velocity_.x));
	int signX = (velocity_.x > 0) ? 1 : -1;

	for (int i = 0; i < stepsX; ++i) {
		transform->position_.x += signX;

		SDL_Rect testRect = collider->GetBounds();
		if (CollisionManager::CheckCollision(game_object_, testRect)) {
			transform->position_.x -= signX;
			velocity_.x = 0;
			break;
		}
	}

	int stepsY = static_cast<int>(std::abs(velocity_.y));
	int signY = (velocity_.y > 0) ? 1 : -1;

	for (int i = 0; i < stepsY; ++i) {
		transform->position_.y += signY;

		SDL_Rect testRect = collider->GetBounds();
		if (CollisionManager::CheckCollision(game_object_, testRect)) {
			transform->position_.y -= signY;
			velocity_.y = 0;
			break;
		}
	}
}

void Rigidbody::Update() {
	if (!transform_ || is_kinematic_) return;

	if (use_gravity_) {
		acceleration_.y += gravity_;
	}

	velocity_.x += acceleration_.x;
	velocity_.y += acceleration_.y;

	velocity_.x *= linear_drag_.x;
	velocity_.y *= linear_drag_.y;

	if (std::abs(velocity_.x) < 0.01f) velocity_.x = 0.0f;
	if (std::abs(velocity_.y) < 0.01f) velocity_.y = 0.0f;

	velocity_.Clamp(-max_velocity_, max_velocity_);

	CheckAndResolveCollision();

	acceleration_ = { 0.0f, 0.0f };
}

void Rigidbody::DrawUI(){
	BeginDraw("Rigidbody");

	DrawVector2Control("Velocity", velocity_);
	DrawVector2Control("Acceleration", acceleration_);
	DrawVector2Control("Liniar Drag", linear_drag_);

	DrawFloatControl("Gravity", &gravity_);
	DrawFloatControl("Mass", &mass_);
	DrawFloatControl("Max Velocity", &max_velocity_);

	DrawCheckbox("Is Kinematic", &is_kinematic_);
	DrawCheckbox("Use Gravity", &use_gravity_);
       
	EndDraw();
}

void Rigidbody::Serialize(nlohmann::json& json){
	nlohmann::json rigidbody;

	rigidbody["type"] = "Rigidbody";

	rigidbody["data"]["mass"] = mass_;
	rigidbody["data"]["gravity"] = gravity_;
	rigidbody["data"]["max velocity"] = max_velocity_;
	rigidbody["data"]["use gravity"] = use_gravity_;
	rigidbody["data"]["is kinematic"] = is_kinematic_;

	rigidbody["active"] = active_;

	json.push_back(rigidbody);
}

void Rigidbody::Deserialize(const nlohmann::json& json){
	if (json.contains("mass")) {
		mass_ = json["mass"];
	}

	if (json.contains("gravity")) {
		gravity_ = json["gravity"];
	}

	if (json.contains("max velocity")) {
		max_velocity_ = json["max velocity"];
	}

	if (json.contains("use gravity")) {
		use_gravity_ = json["use gravity"];
	}

	if (json.contains("kinematic")) {
		is_kinematic_ = json["kinematic"];
	}
}
