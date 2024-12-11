extends Node

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var n = get_parent()
	#n.execution_timeout = 0
	var script = n.get_script() as ELFScript
	print(script.get_sandbox_for(n))
	print(n.test(1, 2, "3"))
