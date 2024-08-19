extends Node;

@export var program_elf : Sandbox_TestTest;

func _ready() -> void:
	#program_elf.assault("Variant", 10000)

	program_elf.function3(1, 2, "3");
	#var f3 = program_elf.vmcallable("function3");
	#f3.call(1, 2, "3");

	# Measure the time it takes to cold-call one function
	print("\nMeasuring call overhead...");
	var emptyfunc = program_elf.vmcallable("empty_function");
	var vmcallfunc = program_elf.vmcallable("calling_function");

	var gds_t0 = Time.get_ticks_usec()
	for n in 200:
		plain_function();
	var gds_t1 = Time.get_ticks_usec()
	var gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 200.0;
	print("GDScript call overhead: ", gds_overhead, " nanos")

	var t0 = Time.get_ticks_usec()
	for n in 200:
		emptyfunc.call();
	var t1 = Time.get_ticks_usec()
	var sandbox_overhead = (t1 - t0) * 1000.0 / 200.0;
	print("Sandbox vmcall time: ", sandbox_overhead, " nanos")
	
	gds_t0 = Time.get_ticks_usec()
	for n in 200:
		calling_function(plain_function);
	gds_t1 = Time.get_ticks_usec()
	gds_overhead = (gds_t1 - gds_t0) * 1000.0 / 200.0;
	print("GDScript call overhead: ", gds_overhead, " nanos")

	t0 = Time.get_ticks_usec()
	for n in 200:
		vmcallfunc.call(plain_function);
	t1 = Time.get_ticks_usec()
	sandbox_overhead = (t1 - t0) * 1000.0 / 200.0;
	print("Sandbox vmcall callable time: ", sandbox_overhead, " nanos")

	pass # Replace with function body.

static func plain_function():
	return;

static func calling_function(cl):
	cl.call();

func _process(_delta: float) -> void:
	pass
