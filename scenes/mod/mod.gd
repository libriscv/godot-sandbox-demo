extends Node2D

func _ready():
	# Fetch the program from somewhere
	var zipfile : PackedByteArray = get_node("../C++ program/Sandbox_TestTest").vmcall("get_embedded_luajit")
	# Decompress as GZIP
	var buffer : PackedByteArray = zipfile.decompress_dynamic(zipfile.size() * 10, 3)

	var s : Sandbox = get_node("LuaVM")
	s.load_buffer(buffer)

	if (s.is_binary_translated()):
		print("LuaJit was binary translated")
	else:
		print("LuaJit IS NOT binary translated")
		var bintr = s.emit_binary_translation()
		var f = FileAccess.open("res://bintr_luajit.cpp", FileAccess.WRITE)
		f.store_string(bintr)
		f.close()
