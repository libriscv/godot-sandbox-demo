extends Node

@export var my_program : Sandbox_ZigZig

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	print("Zig computed: ", my_program.add(123, 456))


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass
