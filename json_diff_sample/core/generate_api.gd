@tool
extends EditorScript

func _run():
	var sandbox_scene = Sandbox.new()
	var api: String = sandbox_scene.generate_api("cpp")
	var file: FileAccess = FileAccess.open("res://json_diff_sample/json_diff/generated_api.hpp", FileAccess.WRITE)
	file.store_string(api)
	file.close()
