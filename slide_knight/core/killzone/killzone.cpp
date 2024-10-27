#include "api.hpp"

extern "C" Variant _on_body_entered(CharacterBody2D body) {
	Engine::get_singleton().set_time_scale(0.5);

	body.set_velocity(Vector2(0.0f, -120.0f));
	body.get_node("CollisionShape2D").queue_free();
	print("Playing dead");
	body.get_node<AnimatedSprite2D>("AnimatedSprite2D").play("died");

	CallbackTimer::native_oneshot(1.0f, [] (Timer timer) -> Variant {
		timer.queue_free();
		Engine::get_singleton().set_time_scale(1.0);

		get_tree().call_deferred("reload_current_scene");
		return Nil;
	});
	return Nil;
}
