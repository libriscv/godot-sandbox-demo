extends Node;

@export var program_elf : Sandbox_TestTest

func measure_callable_overhead():
	var vmcallfunc = program_elf.vmcallable("calling_function", [plain_function]);

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
	#program_elf.assault("Variant", 10000)

	program_elf.function3(1, 2, "3");
	program_elf.empty_function();
	#var f3 = program_elf.vmcallable("function3");
	#f3.call(1, 2, "3");

	# Measure the time it takes to cold-call one function
	print("\nMeasuring call overhead...");
	var emptyfunc = program_elf.vmcallable("empty_function");
	var arg1func = program_elf.vmcallable("arg1_function", []);

	var gds_t0 = Time.get_ticks_usec()
	for n in 500:
		plain_function();
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 500.0;
	print("GDScript call overhead: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 500:
		emptyfunc.call();
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) * 1000.0 / 500.0;
	print("Sandbox vmcall overhead: ", sandbox_overhead, " nanos")

	t0 = Time.get_ticks_usec()
	for n in 500:
		arg1func.call("11", "22", "33", "44", "55", "66", "77");
	t1 = Time.get_ticks_usec()
	gds_t0 = Time.get_ticks_usec()
	for n in 500:
		arg1_function("11", "22", "33", "44", "55", "66", "77");
	gds_t1 = Time.get_ticks_usec()

	gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 500.0;
	print("GDScript call 6xargs overhead: ", gds_overhead, " nanos")

	sandbox_overhead = (t1 - t0) * 1000.0 / 500.0;
	print("Sandbox vmcall 6xargs overhead: ", sandbox_overhead, " nanos")

	pass # Replace with function body.

static func arg1_function(_a1, _a2, _a3, _a4, _a5, _a6, _a7):
	return;

static func calling_function(cl):
	cl.call();

func _process(_delta: float) -> void:
	pass
