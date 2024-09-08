mod godot;
use godot::node::*;
use godot::variant::*;

const SPEED: f32 = 50.0;

#[no_mangle]
pub fn _physics_process(delta: Variant) -> Variant {
	let slime = Node::new_from_path(".");
	let sprite = Node::new_from_path("AnimatedSprite2D");
	let flip_h = sprite.call("is_flipped_h", &[]);
	let direction: f32 = (flip_h.to_bool() as i32 as f32 - 0.5) * -2.0;
	let mut pos = slime.call("get_position", &[]).to_vec2();
	pos.x += direction * SPEED * delta.to_float() as f32;
	slime.call("set_position", &[Variant::new_vec2(pos)]);
	let ray_right = Node::new_from_path("raycast_right");
	let ray_left  = Node::new_from_path("raycast_left");
	if direction > 0.0 && ray_right.call("is_colliding", &[]).to_bool() {
		sprite.call("set_flip_h", &[Variant::new_bool(true)]);
	}
	else
	if direction < 0.0 && ray_left.call("is_colliding", &[]).to_bool() {
		sprite.call("set_flip_h", &[Variant::new_bool(false)]);
	}
	return Variant::new_nil();
}

pub fn main() {
}
