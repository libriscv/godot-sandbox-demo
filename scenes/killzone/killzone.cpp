#include "api.hpp"

extern "C" Variant _on_body_entered(Variant bodyVar) {
	Object engine("Engine");
	engine.set("time_scale", 0.5f);

	Node2D body = bodyVar.as_node2d();
	body.set("velocity", Vector2(0.0f, -120.0f));
	body.get_node("CollisionShape2D").queue_free();
	body.get_node("AnimatedSprite2D")("play", "died");

	auto timer = Node("Timer");
	timer("start");
	return {};
}

extern "C" Variant _on_timer_timeout() {
	Object engine("Engine");
	engine.set("time_scale", 1.0f);

	get_tree().call_deferred("reload_current_scene");
	return {};
}
