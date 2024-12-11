#include "player.hpp"
#include <cmath>

static float jump_velocity = -300.0f;
static float player_speed = 150.0f;
static float direction = 0.0f;
static std::string player_name = "Slight Knight";

SANDBOXED_PROPERTIES(3, {
	.name = "player_speed",
	.type = Variant::FLOAT,
	.getter = []() -> Variant { return player_speed; },
	.setter = [](Variant value) -> Variant { return player_speed = value; },
	.default_value = Variant{player_speed},
}, {
	.name = "player_jump_vel",
	.type = Variant::FLOAT,
	.getter = []() -> Variant { return jump_velocity; },
	.setter = [](Variant value) -> Variant { return jump_velocity = value; },
	.default_value = Variant{jump_velocity},
}, {
	.name = "player_name",
	.type = Variant::STRING,
	.getter = []() -> Variant { return player_name; },
	.setter = [](Variant value) -> Variant { return player_name = value.as_std_string(); },
	.default_value = Variant{"Slight Knight"},
});

PUBLIC Variant _physics_process(double delta) {
	if (is_editor()) {
		if (is_part_of_tree(get_node())) {
			Dictionary d = Dictionary::Create();
			d["test"] = Node("AnimatedSprite2D");
			d["test"]("play", "idle");
		}
		return Nil;
	}

	CharacterBody2D player = get_node();
	Input input = Input::get_singleton();
	Vector2 velocity = player.velocity();

	// Add the gravity.
	if (!player.is_on_floor()) {
		velocity += player.get_gravity() * delta;
	}

	AnimatedSprite2D animated_sprite("AnimatedSprite2D");
	const bool has_died = animated_sprite.animation() == "died";
	if (has_died)
		goto velocity_calculations;

	// Get the input direction and handle the movement/deceleration.
	direction = input.get_axis("move_left", "move_right");

	// Handle jump.
	if (input.is_action_just_pressed("jump") && player.is_on_floor())
		velocity.y = jump_velocity;

	if (direction != 0)
		animated_sprite.set_flip_h(direction < 0);

	if (player.is_on_floor()) {
		if (direction == 0) {
			animated_sprite.play("idle");
		} else {
			animated_sprite.play("run");
		}
	} else {
		animated_sprite.play("jump");
	}

velocity_calculations:
	if (direction != 0)
		velocity.x = direction * player_speed;
	else
		velocity.x = fmin(velocity.x, player_speed);
	player.set_velocity(velocity);

	return player.move_and_slide();
}

PUBLIC Variant _process() {
	AnimatedSprite2D animated_sprite("AnimatedSprite2D");
	if (is_editor()) {
		animated_sprite.set_modulate(0xFFFFFFFF);
		return Nil;
	}

	const bool has_died = animated_sprite.animation() == "died";
	if (has_died) {
		static constexpr float period = 3.0f;
		static float x = 0.0f;
		// Death color modulation
		const int redness = Math::sin(x * period) * 127 + 128;
		const int r = 255;
		const int g = redness;
		const int b = redness;

		const uint32_t mod = 255 | b << 8 | g << 16 | r << 24;
		animated_sprite.set_modulate(mod);
		x += 0.1f;
	}
	return Nil;
}

int main() {
	ADD_API_FUNCTION(_physics_process, "void", "double delta");
	ADD_API_FUNCTION(_process, "void");

	add_property("player_speed", Variant::FLOAT, player_speed,
		[]() -> Variant { return player_speed; },
		[](Variant value) -> Variant { return player_speed = value; });
	add_property("player_jump_vel", Variant::FLOAT, jump_velocity,
		[]() -> Variant { return jump_velocity; },
		[](Variant value) -> Variant { return jump_velocity = value; });
	add_property("player_name", Variant::STRING, player_name,
		[]() -> Variant { return player_name; },
		[](Variant value) -> Variant { return player_name = value.as_std_string(); });

	halt();
}
