#include "api.hpp"
static int coins = 0;

extern "C" Variant reset_game() {
	coins = 0;
	return Nil;
}

static void add_coin(const Node& player) {
	coins ++;
	// In our demo project we can access the coin label from the player
	// using a node path: Player -> get_parent() -> Texts -> CoinLabel
	Label coinlabel = player.get_node<Label>("../Texts/CoinLabel");
	coinlabel.set_text("You have collected "
		+ std::to_string(coins) + ((coins == 1) ? " coin" : " coins"));
}

extern "C" Variant _on_body_entered(CharacterBody2D body) {
	// This function is called when a body enters the coin
	// Most likely it's the player, but we still check!
	if (body.get_name() != "Player")
		return Nil;

	Node coin = get_node();
	/*Array meta_list = coin("get_meta_list").as_array();
	for (auto str : meta_list) {
		print(str); // Print all meta keys
	}*/
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

extern "C" Variant _ready() {
	if (is_editor_hint()) {
		get_node().set_process_input(false);
	}
	return Nil; //
}

extern "C" Variant _process(double delta) {
	if (is_editor_hint()) {
		AnimatedSprite2D sprite("AnimatedSprite2D");
		sprite.play("idle");
		sprite.set_speed_scale(1.0f);
	}
	return Nil;
}

extern "C" Variant _input(InputEvent event) {
	if (event.is_action_pressed("jump")) {
		get_node<Node2D>().set_modulate(0xFF6060FF);
	} else if (event.is_action_released("jump")) {
		get_node<Node2D>().set_modulate(0xFFFFFFFF);
	}
	return Nil;
}
