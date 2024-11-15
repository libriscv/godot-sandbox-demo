extends Sandbox

const program: ELFScript = preload("../json_diff.elf")


func _ready() -> void:
	set_program(program)
	var source: String = JSON.stringify(JSON.from_native(self, true), "  ")
	print("Godot source json: %s" % [source])
	var reference: String = """
	{
			"__gdtype": "Object",
			"properties": {
				"_import_path": {
				"__gdtype": "NodePath",
				"path": ""
				},
				"auto_translate_mode": 0,
				"editor_description": "",
				"execution_timeout": 8000,
				"memory_max": 16,
				"monitor_accumulated_startup_time": 0.0044,
				"monitor_calls_made": 0,
				"monitor_exceptions": 0,
				"monitor_execution_timeouts": 0,
				"monitor_global_calls_made": 0,
				"monitor_global_exceptions": 0,
				"monitor_global_execution_timeouts": 0,
				"monitor_global_instance_count": 1,
				"monitor_heap_usage": 1152,
				"physics_interpolation_mode": 0,
				"process_mode": 0,
				"process_physics_priority": 0,
				"process_priority": 0,
				"process_thread_group": 0,
				"references_max": 100,
				"restrictions": false,
				"unique_name_in_owner": false,
				"use_precise_simulation": false,
				"use_unboxed_arguments": false
			},
			"type": "Sandbox"
		}
	"""
	print("Godot reference json: %s" % [reference])
	print("Json diff: %s", vmcall("generate_json_diff", source, reference))
