extends Node

@export var my_program : Sandbox_TestTest

func measure_callable_overhead():
	var vmcallfunc = my_program.vmcallable("calling_function", [plain_function]);

	var gds_t0 = Time.get_ticks_usec()
	for n in 1500:
		calling_function(plain_function);
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 1500.0;
	print("GDScript call overhead: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 1500:
		vmcallfunc.call();
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) * 1000.0 / 1500.0;
	print("Sandbox vmcall callable time: ", sandbox_overhead, " nanos")

func measure_obj_callp_overhead():
	var obj : Node = Node.new()
	var vmcallfunc = my_program.vmcallable("measure_obj_callp");

	var gds_t0 = Time.get_ticks_usec()
	for n in 1500:
		obj_callp_function(obj);
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 1500.0;
	print("GDScript obj_callp overhead: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 1500:
		vmcallfunc.call(obj);
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) * 1000.0 / 1500.0;
	print("Sandbox obj_callp overhead: ", sandbox_overhead, " nanos")

func measure_skydome_creation():
	var cppfunc = my_program.vmcallable("createSkyDome", [10.0, 10.25]);

	var gds_t0 = Time.get_ticks_usec()
	for n in 15:
		createSkyDome(10.0, 10.25)
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) / 15.0;
	print("GDScript createSkyDome time: ", gds_overhead, " micros")

	var t0 = Time.get_ticks_usec()
	for n in 15:
		cppfunc.call();
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) / 15.0;
	print("Sandbox createSkyDome time: ", sandbox_overhead, " micros")

func benchmark_method_calls():
	var cppfunc = my_program.vmcallable("method_call_bench");

	var gds_t0 = Time.get_ticks_usec()
	for n in 1500:
		method_call_bench()
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) / 1500.0 * 1000.0;
	print("GDScript method call time: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 1500:
		cppfunc.call();
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) / 1500.0 * 1000.0;
	print("Sandbox method call time: ", sandbox_overhead, " nanos")

static func plain_function():
	return;

func instantiation_benchmark():
	var instances : Array
	for i in 100:
		var s = Sandbox.new()
		s.set_program(Sandbox_TestTest)
		instances.push_back(s)
	print("Global instance count: ", my_program.get("monitor_global_instance_count"))
	print("Accumulated startup time: ", my_program.get("monitor_accumulated_startup_time"))
	print("Average startup time: ", my_program.get("monitor_accumulated_startup_time") / my_program.get("monitor_global_instance_count") * 1e6, " us")
	for i in 100:
		instances[i].queue_free()

func _ready() -> void:
	#my_program.assault("Variant", 10000)

	if (my_program.is_binary_translated()):
		print("Test.cpp was binary translated")
	else:
		print("Test.cpp IS NOT binary translated")
		var bintr = my_program.emit_binary_translation(true, true)
		var f = FileAccess.open("res://bintr_test.cpp", FileAccess.WRITE)
		f.store_string(bintr)
		f.close()

	var aa : Array
	aa.push_back("Array 123")
	aa.push_back("Array 456")
	my_program.function3(1, 2, aa)
	var dd : Dictionary
	dd["123"] = "Dict 123"
	dd["456"] = "Dict 456"

	#my_program.empty_function()
	var f3 = my_program.vmcallable("my_function");
	f3.call(dd);

	var adder_func = my_program.vmcallable("adding_function", [1, 2, 3])
	print("Sum: ", adder_func.call(4, 5, 6))

	# Measure the time it takes to cold-call one function
	print("\nMeasuring call overhead...")
	var emptyfunc = my_program.vmcallable("empty_function")
	var vm_callable = my_program.vmcallable("arg1_function", [])

	var gds_t0 = Time.get_ticks_usec()
	for n in 500:
		plain_function()
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 500.0
	print("GDScript call overhead: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 500:
		emptyfunc.call()
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) * 1000.0 / 500.0;
	print("Sandbox vmcall overhead: ", sandbox_overhead, " nanos")

	t0 = Time.get_ticks_usec()
	for n in 2000:
		vm_callable.call(11, 22, 33, 44, 55, 66, 77)
	t1 = Time.get_ticks_usec()
	gds_t0 = Time.get_ticks_usec()
	for n in 2000:
		gds_function(11, 22, 33, 44, 55, 66, 77)
	gds_t1 = Time.get_ticks_usec()

	gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 2000.0;
	print("GDScript call 7xargs overhead: ", gds_overhead, " nanos")

	sandbox_overhead = (t1 - t0) * 1000.0 / 2000.0;
	print("Sandbox vmcall 7xargs overhead: ", sandbox_overhead, " nanos")

	measure_skydome_creation()

	measure_obj_callp_overhead()

	benchmark_method_calls()

	measure_pfa_operation()

	my_program.test_embedded_mir()

	pass # Replace with function body.

func measure_pfa_operation():
	var vm_pf32a_callable = my_program.vmcallable("pba_operation")
	var pf32a : PackedFloat32Array
	pf32a.resize(100000)

	var t0 = Time.get_ticks_usec()
	for n in 100:
		vm_pf32a_callable.call(pf32a)
	var t1 = Time.get_ticks_usec()
	var gds_t0 = Time.get_ticks_usec()
	for n in 100:
		pf32a_operation(pf32a)
	var gds_t1 = Time.get_ticks_usec()

	var gds_overhead = (gds_t1 - gds_t0) / 100.0;
	print("GDScript Pf32A operation: ", gds_overhead, " micros")

	var sandbox_overhead = (t1 - t0) / 100.0;
	print("Sandbox Pf32A operation: ", sandbox_overhead, " micros")

func pf32a_operation(pf32a : PackedFloat32Array):
	for n in pf32a.size():
		pf32a[n] = 1.0
	assert(pf32a[0] == 1.0)
	#pf32a.fill(1.0)

func gds_function(_a1, _a2, _a3, _a4, _a5, _a6, _a7):
	return;

static func calling_function(cl):
	cl.call();

func obj_callp_function(obj):
	#return obj.call("get_name")
	return obj.get_name()

func method_call_bench():
	var v : Vector3 = Vector3(1.5, 2.5, 3.5)
	return v.floor()

func _process(_delta: float) -> void:
	pass

func skyDomeLookup(vertices : Array, dv : Vector3):
	for idx in vertices.size():
		if vertices[vertices.size()-1 - idx].distance_squared_to(dv) < 0.0001:
			return idx
	var index : int = vertices.size()
	vertices.push_back(dv)
	return index

func createSkyDome(r_inner : float, r_outer : float):
	const numX = 16
	const numY = 4
	const total_points = (2 * numY + 1) * numX * 3 # triangles * 3

	# total number of vertices allocated to buffer:
	var vertices : Array
	var indices : Array
	var lookup : Array # Vector3 -> index

	# vertices for triangle strip:
	var va : Vector3
	var vb : Vector3
	var vc : Vector3
	var vd : Vector3

	var stepa  : float = PI * 2.0 / numX
	var startb : float = asin(r_inner / r_outer)
	var stepb  : float = (PI / 2.0 - startb) / 4.0

	for y in numY:
		var b : float = startb + stepb * y

		for x in numX:
			var a : float = stepa * x

			va = Vector3( sin(a)         * cos(b) * r_outer,         sin(b) * r_outer,         -cos(a)         * cos(b) * r_outer )
			vb = Vector3( sin(a + stepa) * cos(b) * r_outer,         sin(b) * r_outer,         -cos(a + stepa) * cos(b) * r_outer )
			vc = Vector3( sin(a + stepa) * cos(b + stepb) * r_outer, sin(b + stepb) * r_outer, -cos(a + stepa) * cos(b + stepb) * r_outer )
			vd = Vector3( sin(a)         * cos(b + stepb) * r_outer, sin(b + stepb) * r_outer, -cos(a)         * cos(b + stepb) * r_outer )

			# Find va, vb, vc, vd in the lookup hashmap and add their index to the indices vector
			# Triangle 1: va, vb, vc
			indices.push_back(skyDomeLookup(vertices, va));
			var va_idx = indices.back()
			indices.push_back(skyDomeLookup(vertices, vb));
			indices.push_back(skyDomeLookup(vertices, vc));
			var vc_idx = indices.back()
			# Triangle 2: vc, vd, va
			indices.push_back(vc_idx);
			indices.push_back(skyDomeLookup(vertices, vd));
			indices.push_back(va_idx);

	var b : float = startb + stepb * 3

	for x in numX:
		var a : float = stepa * x

		va = Vector3 ( sin(a)         * cos(b) * r_outer, sin(b) * r_outer, -cos(a)         * cos(b) * r_outer )
		vb = Vector3 ( sin(a + stepa) * cos(b) * r_outer, sin(b) * r_outer, -cos(a + stepa) * cos(b) * r_outer )
		vc = Vector3 ( 0.0, r_outer, 0.0 )

		indices.push_back(skyDomeLookup(vertices, va));
		indices.push_back(skyDomeLookup(vertices, vb));
		indices.push_back(skyDomeLookup(vertices, vc));

	assert(indices.size() == total_points)
	#print(vertices.size())
