extends Label

@export var player_elf : Sandbox_ScenesPlayerPlayer
@export var slime_elf : Sandbox_ScenesSlimeSlime
@export var coin_elf : Sandbox_ScenesScenes

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	self.text = player_elf.get("player_name")
	pass # Replace with function body.

func _process(delta: float) -> void:
	if self.name == "CallLabel":
		# One player
		var player_calls = player_elf.get("monitor_calls_made")
		# One slime x2 for total estimate
		var slime_calls = slime_elf.get("monitor_calls_made") * 2
		# One coin x6 for total estimate
		var coin_calls = coin_elf.get("monitor_calls_made") * 6
		self.text = "Virtual Machine\nfunction calls: " + str(player_calls + slime_calls + coin_calls)
	pass
