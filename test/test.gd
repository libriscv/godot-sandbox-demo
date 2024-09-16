extends Node

@export var my_program : Sandbox_TestTest

func measure_callable_overhead():
	var vmcallfunc = my_program.vmcallable("calling_function", [plain_function]);

	var gds_t0 = Time.get_ticks_usec()
	for n in 1500:
		calling_function(plain_function);
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 500.0;
	print("GDScript call overhead: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 1500:
		vmcallfunc.call();
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) * 1000.0 / 500.0;
	print("Sandbox vmcall callable time: ", sandbox_overhead, " nanos")

static func plain_function():
	return;

func _ready() -> void:
	#my_program.assault("Variant", 10000)

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
	var vm_pf32a_callable = my_program.vmcallable("pba_operation")

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

	var pf32a : PackedFloat32Array
	pf32a.resize(100000)

	t0 = Time.get_ticks_usec()
	for n in 100:
		vm_pf32a_callable.call(pf32a)
	t1 = Time.get_ticks_usec()
	gds_t0 = Time.get_ticks_usec()
	for n in 100:
		pf32a_operation(pf32a)
	gds_t1 = Time.get_ticks_usec()

	gds_overhead = (gds_t1 - gds_t0) / 100.0;
	print("GDScript Pf32A operation: ", gds_overhead, " micros")

	sandbox_overhead = (t1 - t0) / 100.0;
	print("Sandbox Pf32A operation: ", sandbox_overhead, " micros")

	pass # Replace with function body.

func pf32a_operation(pf32a : PackedFloat32Array):
	#for n in pf32a.size():
	#	pf32a[n] = 0.0
	pf32a.fill(1.0)

func gds_function(_a1, _a2, _a3, _a4, _a5, _a6, _a7):
	return;

static func calling_function(cl):
	cl.call();

func _process(_delta: float) -> void:
	pass
