#include "api.hpp"
static int coins = 0;

PUBLIC Variant reset_game() {
	coins = 0;
	return Nil;
}

static inline void add_coin(const Node& player) {
	coins ++;
	// In our demo project we can access the coin label from the player
	// using a node path: Player -> get_parent() -> Texts -> CoinLabel
	Label coinlabel = player.get_node<Label>("../Texts/CoinLabel");
	coinlabel.set_text("You have collected "
		+ std::to_string(coins) + ((coins == 1) ? " coin" : " coins"));
}

PUBLIC Variant _on_body_entered(CharacterBody2D body) {
	// This function is called when a body enters the coin
	// Most likely it's the player, but we still check!
	if (body.get_name() != "Player")
		return Nil;

	Node coin = get_node();
	if (coin.get_meta("secret", false)) {
		print("Was a secret!");
		Node2D scene = load<PackedScene>("res://scenes/mod/mod.tscn").instantiate();
		get_node("../..").add_child(scene);
		scene.set_name("mod");
		scene.set_position(Vector2(250, 180));
	}
	coin.queue_free(); // Remove the current coin!
	add_coin(body);
	return Nil;
}

PUBLIC Variant _ready() {
	if (is_editor_hint()) {
		get_node().set_process_input(false);
	}
	return Nil; //
}

PUBLIC Variant _process(double delta) {
	if (is_editor_hint()) {
		AnimatedSprite2D sprite("AnimatedSprite2D");
		sprite.play("idle");
		sprite.set_speed_scale(1.0f);
	}
	return Nil;
}

PUBLIC Variant _input(InputEvent event) {
	if (event.is_action_pressed("jump")) {
		get_node<Node2D>().set_modulate(0xFF6060FF);
	} else if (event.is_action_released("jump")) {
		get_node<Node2D>().set_modulate(0xFFFFFFFF);
	}
	return Nil;
}

int main() {
	ADD_API_FUNCTION(_on_body_entered, "void", "CharacterBody2D body");
	ADD_API_FUNCTION(_ready, "void");
	ADD_API_FUNCTION(_process, "void", "double delta");
	ADD_API_FUNCTION(_input, "void", "InputEvent event");
	ADD_API_FUNCTION(reset_game, "void");

	halt();
}
