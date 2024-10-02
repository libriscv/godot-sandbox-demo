extends Node

@export var my_program : Sandbox_ZigZig

static func zig_called_me():
	print("Zig called me!")
	return 123

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	my_program.memory_max = 11
	my_program.execution_timeout = 10
	my_program.references_max = 12
	print(my_program.get_current_instruction())
	
	my_program.use_unboxed_arguments = true
	print("Zig computed: ", my_program.add(123, 456))
	
	my_program.use_unboxed_arguments = false
	print("Zig w/callable returned: ", my_program.test_call(Callable(zig_called_me)))
