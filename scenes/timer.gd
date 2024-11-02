extends Timer

func resolver(file, address):
	if file == "luajit.elf":
		return (get_node("../mod/LuaVM") as Sandbox).lookup_address(address)
	return "Unknown"

func _on_timeout() -> void:
	var hotspots : Array = Sandbox.get_hotspots("luajit.elf", resolver)
	print(JSON.stringify(hotspots, "    "))
	var txt = get_node("../TextEdit") as TextEdit
	txt.text = ""
	
	var stats : Dictionary = hotspots[hotspots.size()-1]
	for i in 4:
		var prof : Dictionary = hotspots[i]
		var percent = str((prof["samples"] * 100) / stats["samples_total"]) + "%"
		var path = String(prof["file"]).split("/")
		var elf = path[path.size()-1]
		txt.text += "[" + percent + "] " + prof["function"] + " (" + elf + ")\n"
	
	return
