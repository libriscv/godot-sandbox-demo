extends Node;

@export var rust : Sandbox_RustRust;

func _ready() -> void:
	print(rust.public_function(1234));
