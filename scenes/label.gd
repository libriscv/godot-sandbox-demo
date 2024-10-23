extends Label

var player_elf : Sandbox

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var player = get_node("%Player")
	player_elf = player.get_script().get_sandbox_for(player)
	self.text = player_elf.get("player_name")
	pass # Replace with function body.

func _process(_delta: float) -> void:
	if self.name == "CallLabel":
		# Get any sandbox, and ask for the global number of VM calls made
		var total_calls = player_elf.monitor_global_calls_made
		self.text = "Virtual Machine\nfunction calls: " + str(total_calls)
	pass
