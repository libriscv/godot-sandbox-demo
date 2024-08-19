#include "api.hpp"

struct SlimeState {
	int direction = 1;
};
PER_OBJECT(SlimeState);

extern "C" Variant _process(Variant delta) {
	Node2D slime(".");
	Node2D sprite("AnimatedSprite2D");
	auto& state = GetSlimeState(slime);

	Vector2 spd = Vector2(50.0f, 0.0f) * float(delta) * state.direction;
	slime.set_position(slime.get_position() + spd);
	// Change direction if rays collide
	if (state.direction > 0 && Node("raycast_right")("is_colliding")) {
		state.direction = -1;
		sprite.set("flip_h", true);
		sprite("play", "spawn");
	} else if (state.direction < 0 && Node("raycast_left")("is_colliding")) {
		state.direction = 1;
		sprite.set("flip_h", false);
		sprite("play", "idle");
	}
	return {};
}
