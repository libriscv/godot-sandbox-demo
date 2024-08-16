#include "api.hpp"
#include <cmath>

static const float SPEED = 150.0f;
static const float JUMP_VELOCITY = -300.0f;

extern "C" Variant _physics_process(Variant delta) {
	Node2D player(".");
	Object input("Input");
	Vector2 velocity = player.get("velocity").v2();

	// Add the gravity.
	if (!player("is_on_floor")) {
		velocity += player("get_gravity").v2() * delta;
	}

	// Handle jump.
	if (input("is_action_just_pressed", "jump") && player("is_on_floor"))
		velocity.y = JUMP_VELOCITY;

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
		velocity.x = direction * SPEED;
	else
		velocity.x = std::fmin(velocity.x, SPEED);
	player.set("velocity", velocity);

	return player("move_and_slide");
}
