#include "api.hpp"

static int coins = 0;

extern "C" void reset_game() {
	coins = 0;
}

static void add_coin(const Node& player) {
	coins++;
	Node coinlabel = player.get_parent().get_node("Texts/CoinLabel");
	coinlabel.set("text", "You have collected "
		+ std::to_string(coins) + ((coins == 1) ? " coin" : " coins"));
}

extern "C" Variant _on_body_entered(Variant arg) {
	Node player_node = arg.as_node();
	if (player_node.get_name() != "Player") {
		return {};
	}
	Node(".").queue_free(); // Remove the current coin!
	add_coin(player_node);
	return {};
}

extern "C" Variant _process(Variant delta) {
	if (is_editor()) {
		Node("AnimatedSprite2D")("play", "idle");
	}
	return {};
}

extern "C" Variant _input(Variant event) {
	if (is_editor()) {
		return {};
	}
	if (event("is_action_pressed", "jump")) {
		Node2D(".").set("modulate", 0xFF6060FF);
	} else if (event("is_action_released", "jump")) {
		Node2D(".").set("modulate", 0xFFFFFFFF);
	}
	return {};
}
