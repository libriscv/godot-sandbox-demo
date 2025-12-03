#include "player.hpp"

static float jump_velocity = -300.0f;
static float player_speed = 150.0f;
static float direction = 0.0f;
static std::string player_name = "Slight Knight";

static Variant _physics_process(double delta) {
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
	Vector2 velocity = player.get_velocity();

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
		velocity.x = std::fmin(velocity.x, player_speed);
	player.set_velocity(velocity);

	return player.move_and_slide();
}

static float x = 0.0f;
static float z = 0.0f;

static Variant _process(double delta) {
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
	ADD_PROPERTY(player_speed, Variant::FLOAT);
	ADD_PROPERTY(jump_velocity, Variant::FLOAT);
	ADD_PROPERTY(player_name, Variant::STRING);

	ADD_API_FUNCTION(_physics_process, "void", "double delta");
	ADD_API_FUNCTION(_process, "void", "double delta");
}
