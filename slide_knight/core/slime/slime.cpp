#include "api.hpp"

static float slime_speed = 50.0f;

struct SlimeState {
	int direction = 1;
};
PER_OBJECT(SlimeState);

extern "C" Variant _physics_process(double delta) {
	if (is_editor()) {
		AnimatedSprite2D sprite("AnimatedSprite2D");
		sprite.play("idle");
		Time t = Time::get_singleton();
		sprite.set_flip_h((t.get_ticks_msec() / 2560) % 2 == 0);
		return Nil;
	}

	Node2D slime = get_node();
	AnimatedSprite2D sprite("AnimatedSprite2D");
	auto& state = GetSlimeState(slime);

	Vector2 spd = Vector2(slime_speed, 0.0f) * float(delta) * state.direction;
	slime.set_position(slime.get_position() + spd);
	// Change direction if rays collide
	if (state.direction > 0 && RayCast2D("raycast_right").is_colliding()) {
		state.direction = -1;
		sprite.set_flip_h(true);
		sprite.play("spawn");
	} else if (state.direction < 0 && RayCast2D("raycast_left").is_colliding()) {
		state.direction = 1;
		sprite.set_flip_h(false);
		sprite.play("idle");
	}
	return Nil;
}

int main() {
	ADD_PROPERTY(slime_speed, Variant::FLOAT);

	ADD_API_FUNCTION(_physics_process, "void", "double delta");
}
