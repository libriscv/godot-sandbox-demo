extends Node2D

func _ready():
	# Fetch the program from somewhere
	var zipfile : PackedByteArray = get_node("../C++ program/Sandbox_TestTest").vmcall("get_embedded_luajit")
	# Decompress as GZIP
	var buffer : PackedByteArray = zipfile.decompress_dynamic(zipfile.size() * 10, 3)

	var s : Sandbox = get_node("LuaVM")
	s.load_buffer(buffer)
