extends Node2D

var thread

func _thread_function(zipfile):
	# Decompress as GZIP
	var buffer : PackedByteArray = zipfile.decompress_dynamic(zipfile.size() * 10, 3)
	call_deferred("_finish_decompression")
	return buffer

func _finish_decompression():
	var buffer = thread.wait_to_finish()
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

func _ready():
	thread = Thread.new()

	# Fetch the program from somewhere
	var zipfile : PackedByteArray = get_node("../C++ program/Sandbox_TestTest").vmcall("get_embedded_luajit")

	thread.start(_thread_function.bind(zipfile))

	var s : Sandbox = get_node("CVM")
	var cjit : ELFScript = load("res://scenes/mod/cjit.elf")
	s.load_buffer(cjit.get_content())

	if (s.is_binary_translated()):
		print("CJIT was binary translated")
	else:
		print("CJIT IS NOT binary translated")
		var bintr = s.emit_binary_translation(true, true)
		var f = FileAccess.open("res://bintr_cjit.cpp", FileAccess.WRITE)
		f.store_string(bintr)
		f.close()

	var compile : Callable = s.vmcallable("compile")

	var t0 = Time.get_ticks_usec()

	var c : Callable = compile.call("""
	void test(struct Variant *res, int input) {
		res->type = 2; // INT
		res->value = 2 * input;
	}
""", "test")

	var t1 = Time.get_ticks_usec()
	var compilation_time = t1 - t0;
	print("Sandbox TinyCC compilation time: ", compilation_time, " micros")

	print("JIT-compiled test() returned: ", c.call(1234))

	var c2 : Callable = compile.call("""
	void test2(struct Variant *res, unsigned packed_floats) {
		// Retrieve 32-bit float array
		float *f_begin = malloc(100000 * sizeof(float));
		float *f_end = f_begin + 100000;
		//sys_vfetch(packed_floats, &vec, 0);
		// Process the array
		float *f = f_begin;
		for (; f + 8 <= f_end; f += 8) {
			f[0] = 1.0f;
			f[1] = 1.0f;
			f[2] = 1.0f;
			f[3] = 1.0f;
		}
		for (; f < f_end; f += 4) {
			f[0] = 1.0f;
			f[1] = 1.0f;
			f[2] = 1.0f;
			f[3] = 1.0f;
		}
		// Store the array back
		sys_vstore(&packed_floats, 32, f_begin, f_end - f_begin);
		free(f_begin);
		// Return the new packed array (modified by vstore)
		res->type = 32; // PackedFloat32Array
		res->value = packed_floats;
	}
""", "test2")

	var p : PackedFloat32Array
	p.resize(10)
	p.fill(2.0)
	print("JIT-compiled test2() returned: ", c2.call(p).size(), " elements")

	measure_pfa_operation(c2)

func measure_pfa_operation(vm_pf32a_callable):
	var pf32a : PackedFloat32Array
	pf32a.resize(100000)

	var t0 = Time.get_ticks_usec()
	for n in 100:
		vm_pf32a_callable.call(pf32a)
	var t1 = Time.get_ticks_usec()

	var sandbox_overhead = (t1 - t0) / 100.0;
	print("Mir-based Pf32A operation: ", sandbox_overhead, " micros")
