extends Label

@export var player_elf : Sandbox_ScenesPlayerPlayer

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	self.text = player_elf.get("player_name")
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
