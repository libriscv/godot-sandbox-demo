#include "api.hpp"
#include <cmath>

static float jump_velocity = -300.0f;
static float player_speed = 150.0f;

extern "C" const Property prop0 {
	.name = "player_speed",
	.type = Variant::FLOAT,
	.getter = []() -> Variant { return player_speed; },
	.setter = [](Variant value) -> Variant { return player_speed = value; },
	.default_value = Variant{player_speed},
};
extern "C" const Property prop1 {
	.name = "player_jump_vel",
	.type = Variant::FLOAT,
	.getter = []() -> Variant { return jump_velocity; },
	.setter = [](Variant value) -> Variant { return jump_velocity = value; },
	.default_value = Variant{jump_velocity},
};

extern "C" Variant _physics_process(Variant delta) {
	if (is_editor())
		return {};

	Node2D player(".");
	Object input("Input");
	Vector2 velocity = player.get("velocity").v2();

	// Add the gravity.
	if (!player("is_on_floor")) {
		velocity += player("get_gravity").v2() * delta;
	}

	// Handle jump.
	if (input("is_action_just_pressed", "jump") && player("is_on_floor"))
		velocity.y = jump_velocity;

	// Get the input direction and handle the movement/deceleration.
	float direction = input("get_axis", "move_left", "move_right");

	Node animated_sprite("AnimatedSprite2D");
	if (direction != 0)
		animated_sprite.set("flip_h", (direction < 0));
	if (player("is_on_floor")) {
		if (direction == 0) {
			animated_sprite("play", "idle");
		} else {
			animated_sprite("play", "run");
		}
	} else {
		animated_sprite("play", "jump");
	}

	if (direction != 0)
		velocity.x = direction * player_speed;
	else
		velocity.x = std::fmin(velocity.x, player_speed);
	player.set("velocity", velocity);

	return player("move_and_slide");
}
