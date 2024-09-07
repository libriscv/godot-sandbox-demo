#include "api.hpp"

extern "C" Variant _on_body_entered(Variant bodyVar) {
	Object engine("Engine");
	engine.set("time_scale", 0.5f);

	Node2D body = cast_to<Node2D> (bodyVar);
	body.set("velocity", Vector2(0.0f, -120.0f));
	body.get_node("CollisionShape2D").queue_free();
	print("Playing dead");
	body.get_node("AnimatedSprite2D")("play", std::u32string(U"died"));

	Timer::oneshot(1.0f, [] (Variant timer) -> Variant {
		timer.as_node().queue_free();
		Object engine("Engine");
		engine.set("time_scale", 1.0f);

		get_tree().call_deferred("reload_current_scene");
		return {};
	});
	return {};
}
