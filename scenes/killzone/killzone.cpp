#include "api.hpp"

PUBLIC Variant _on_body_entered(CharacterBody2D body) {
	Engine::get_singleton().set_time_scale(0.5);

	body.set_velocity(Vector2(0.0f, -120.0f));
	body.get_node("CollisionShape2D").queue_free();
	print("Playing dead!");
	body.get_node<AnimatedSprite2D>("AnimatedSprite2D").play("died");

	CallbackTimer::native_oneshot(1.0f, [] (Timer timer) -> Variant {
		timer.queue_free();
		Engine::get_singleton().set_time_scale(1.0f);

		get_tree().call_deferred("reload_current_scene");
		return Nil;
	});
	return Nil;
}

int main() {
	ADD_API_FUNCTION(_on_body_entered, "void", "CharacterBody2D body");

	halt();
}