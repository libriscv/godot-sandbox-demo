extends Label

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var player = get_node("%Player")
	self.text = player.get("player_name")
	pass # Replace with function body.

func _process(_delta: float) -> void:
	if self.name == "CallLabel":
		# Get any sandbox, and ask for the global number of VM calls made
		var player = get_node("%Player")
		var total_calls = player.get_global_calls_made()
		self.text = "Virtual Machine\nfunction calls: " + str(total_calls)
	pass
