extends Label

var player_sandbox : Sandbox

func _ready() -> void:
	var player = get_node("%Player")
	# Accessing a shared sandbox requires using get_sandbox_for(), which takes a Node instance
	player_sandbox = player.get_script().get_sandbox_for(player)
	# Now we can access the Sandbox used by Player
	self.text = player_sandbox.get("player_name")

func _process(_delta: float) -> void:
	if self.name == "CallLabel":
		# Get any sandbox, and ask for the global number of VM calls made
		var total_calls = player_sandbox.get("global_calls_made")
		self.text = "Virtual Machine\nfunction calls: " + str(total_calls)
