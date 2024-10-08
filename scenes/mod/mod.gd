extends Node2D

var thread

func _thread_function(zipfile):
	# Decompress as GZIP
	var buffer : PackedByteArray = zipfile.decompress_dynamic(zipfile.size() * 10, 3)
	call_deferred("_finish_decompression", buffer)

func _finish_decompression(buffer):
	var s : Sandbox = get_node("LuaVM")

	#s.make_resumable()
	s.load_buffer(buffer)
	#while !s.resume(10000):
	#	await get_tree().process_frame

	if (s.is_binary_translated()):
		print("LuaJit was binary translated")
	else:
		print("LuaJit IS NOT binary translated")
		var bintr = s.emit_binary_translation()
		var f = FileAccess.open("res://bintr_luajit.cpp", FileAccess.WRITE)
		f.store_string(bintr)
		f.close()

	thread.wait_to_finish()

func _ready():
	thread = Thread.new()

	# Fetch the program from somewhere
	var zipfile : PackedByteArray = get_node("../C++ program/Sandbox_TestTest").vmcall("get_embedded_luajit")

	thread.start(_thread_function.bind(zipfile))
