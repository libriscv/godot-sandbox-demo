#include "api.hpp"
static int coins = 0;

static void add_coin(const Node& player) {
	coins ++;
	Label coinlabel = player.get_node<Label>("../Texts/CoinLabel");
	coinlabel.set_text("You have collected "
		+ std::to_string(coins) + ((coins == 1) ? " coin" : " coins"));
}

static Variant reset_game() {
	coins = 0;
	return Nil;
}

static Variant _on_body_entered(CharacterBody2D body) {
	if (body.get_name() != "Player")
		return Nil;

	get_node().queue_free(); // Remove the current coin!
	add_coin(body);
	return Nil;
}

static Variant _ready() {
	if (is_editor()) {
		get_node().set_process_input(false);
	}
	return Nil;
}

static Variant _process(double delta) {
	if (is_editor()) {
		get_node<AnimatedSprite2D>("AnimatedSprite2D").play("idle");
	}
	return Nil;
}

static Variant _input(InputEvent event) {
	if (event.is_action_pressed("jump")) {
		// Node doesn't have set_modulate, but we know this is a CanvasItem
		// so we can still use the property on the Node using .set/.get
		get_node<Node2D>().set_modulate(0xFF6060FF);
	} else if (event.is_action_released("jump")) {
		get_node<Node2D>().set_modulate(0xFFFFFFFF);
	}
	return Nil;
}

int main() {
	ADD_API_FUNCTION(reset_game, "void");
	ADD_API_FUNCTION(_on_body_entered, "void", "CharacterBody2D body");
	ADD_API_FUNCTION(_ready, "void");
	ADD_API_FUNCTION(_process, "void", "double delta");
	ADD_API_FUNCTION(_input, "void", "InputEvent event");
}
