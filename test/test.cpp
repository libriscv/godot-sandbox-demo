#include "api.hpp"

int main() {
	static bool timer_created = true;
	if (!timer_created) {
		timer_created = true;
		const char *val1 = "123";
		const int val2 = 456;
		Timer::native_periodic(0.1, [=] (Object timer) -> Variant {
			print("Timer with values: ", val1, val2);
			static int counter = 0;
			if (++counter >= 4)
				timer.as_node().queue_free();
			return {};
		});
	}

	print(Vector3 { 1.5, 2.5, 3.5 }("floor"));

	Object j = ClassDB::instantiate("JSON");
	j("parse",
	R"({
		"pi": 3.141,
		"happy": true,
		"name": "Niels",
		"nothing": null,
		"answer": {
			"everything": 42
		},
		"list": [1, 0, 2],
		"object": {
			"currency": "USD",
			"value": 42.99
		}
	})");
	print(j.get("data"));
}

extern "C" Variant empty_function() {
	return {};
}
extern "C" Variant arg1_function(int, int, int, int, int, int, int) {
	return {};
}

extern "C" Variant method_call_bench() {
	//return Vector3 { 1.5, 2.5, 3.5 }("floor");
	Variant(Vector3 { 1.5, 2.5, 3.5 }).void_method("floor");
	return Nil;
	//return Vector3 { 1.5, 2.5, 3.5 }.floor();
}

extern "C" Variant calling_function(Variant& callable) {
	return callable.call();
}

extern "C" Variant my_function(Dictionary text) {
	//print("Hello, ", 124.5, " world!\n");
	//print("Arg: ", text);
	return text; //
}

extern "C" Variant function3(long x, long y, String text) {
	String s = "A value";
	Array a({x, y});
	a.push_front(2);
	a.pop_front();
	Dictionary d = Dictionary::Create();
	d["test1"] = "test2";
	d[Vector3()] = 0.5;
	d[String("A string")] = s;
	d[String("A string")] = String("Another value");
	std::u32string stest = *d["A string"];
	print("Stest: ", stest);

	print("x = ", x, " y = ", y, " array = ", a, " dict = ", d, " string = ", s);
	return {};
}

static Variant copy;

extern "C" Variant trampoline_function(Variant callback, Variant text) {
	copy = callback;
	return callback.call(1, 2, text);
}

extern "C" Variant failing_function() {
	copy.call(1, 2, 3);
	return Variant();
}

extern "C" Variant test_buffer(Variant var) {
	auto data = var.operator std::string();

	char buffer[256];
	snprintf(buffer, sizeof(buffer),
		"The buffer is not here! Or is it?\n");

	print(buffer);
	return var;
}

extern "C" Variant player_process(Variant v_time_passed) {
	return {};
}

extern "C" Variant adding_function(int a1, int a2, int a3, int a4, int a5, int a6) {
	return a1 + a2 + a3 + a4 + a5 + a6;
}

#include <cstring>
#include <memory>
static inline void memset_i32(int *ptr, int value, size_t num) {
	if (num == 0) return;
	*ptr = value;
	size_t start = 1, step = 1;
	for ( ; start + step <= num; start += step, step *= 2)
		memcpy(ptr + start, ptr, sizeof(int) * step);

	if (start < num)
		memcpy(ptr + start, ptr, sizeof(int) * (num - start));
}

extern "C" Variant pba_operation(PackedArray<float> farr) {
	auto array = std::make_unique_for_overwrite<float[]>(100000);
	for (int i = 0; i < 100000; i++)
		array[i] = 1.0f;
	//const float   f = 1.0f;
	//const int32_t val = *(int32_t *)&f;
	//memset_i32((int *)array.get(), val, 100000);
	farr.store(array.get(), 100000);
	return Nil;
}

EMBED_BINARY(binary_data, "../scenes/mod/lua.elf.gz")

extern "C" Variant get_embedded_luajit() {
	return PackedArray<uint8_t>((const uint8_t *)binary_data, binary_data_size);
}

extern "C" {
#include <c2mir/c2mir.h>
}
#include <cstdarg>

static std::string code = R"(
int test() {
	return 1234;
}
)";

static int get_cfunc(void *data) {
	printf("C data: %p\n", data);
	fflush(stdout);
	return code.size();
}
static MIR_context_t ctx;

static void error_func(MIR_error_type_t error_type, const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	fflush(stdout);
	halt();
}

extern "C" Variant test_embedded_mir() {

	ctx = MIR_init();
	MIR_set_error_func(ctx, error_func);

	std::array<const char*, 0> include_dirs = {
	};
	std::array<c2mir_macro_command, 1> macro_commands = {
		{ 1, "TEST", "1" }
	};

	printf("Initialized MIR context: %p\n", ctx);
	fflush(stdout);

	c2mir_init(ctx);

	printf("Compiling C code...\n");
	fflush(stdout);

	// Create a memory stream
	std::vector<char> buffer;
	buffer.resize(1024*1024*2); // 2MB

	FILE *output = fmemopen(buffer.data(), buffer.size(), "w");
	if (!output) {
		printf("Failed to create memory stream\n");
		fflush(stdout);
		return Nil;
	}

	void *getc_data = (void *)code.c_str();

	c2mir_options ops;
	memset(&ops, 0, sizeof(ops));
	ops.message_file = stdout;
	ops.output_file_name = "test.elf";
	ops.include_dirs_num = include_dirs.size();
	ops.include_dirs = include_dirs.data();
	ops.macro_commands_num = macro_commands.size();
	ops.macro_commands = macro_commands.data();
	c2mir_compile(ctx, &ops, &get_cfunc, getc_data, "test.c", output);
	c2mir_finish(ctx);

	return Nil;
}
