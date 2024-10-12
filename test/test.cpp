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

#define ENABLE_MIR 1

#if ENABLE_MIR
extern "C" {
#include <mir-gen.h>
#include <c2mir/c2mir.h>
}
#include <cstdarg>

struct Data {
	const char *code;
	const char *code_end;
};

static int get_cfunc(void *opaque) {
	Data *data_ptr = (Data *)opaque;

	if (data_ptr->code >= data_ptr->code_end)
		return EOF;
	return *data_ptr->code++;
}
static MIR_context_t ctx;
static const int optlevel = 2;

static void *import_resolver(const char *name) {
	printf("import_resolver: %s\n", name);
	return nullptr;
}

static MIR_item_t mir_find_function(MIR_module_t module, const char *func_name) {
	MIR_item_t func, func_item = NULL;
	for (func = DLIST_HEAD(MIR_item_t, module->items); func != NULL; func = DLIST_NEXT(MIR_item_t, func)) {
		if (func->item_type == MIR_func_item && strcmp(func->u.func->name, func_name) == 0) {
			func_item = func;
			break;
		}
	}
	return func_item;
}

static void *mir_get_func(MIR_context_t ctx, MIR_module_t module, const char *func_name) {
	MIR_item_t func_item = mir_find_function(module, func_name);
	if (func_item == NULL) {
		fprintf(stderr, "Error: Mir function %s not found\n", func_name);
		exit(1);
	}
	return MIR_gen(ctx, func_item);
}

static __attribute__((noreturn)) void error_func(MIR_error_type_t error_type, const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	fflush(stdout);
	exit(1);
}

extern "C" Variant test_embedded_mir() {
	ctx = MIR_init();
	MIR_set_error_func(ctx, error_func);

	std::array<const char*, 0> include_dirs = {
	};
	std::array<c2mir_macro_command, 1> macro_commands = {
		{ 1, "TEST", "1" }
	};

	c2mir_init(ctx);
	MIR_gen_init(ctx);
	MIR_gen_set_optimize_level(ctx, optlevel);

	printf("Compiling C code...\n");
	fflush(stdout);

	const std::string code = R"(
int test(void *arg) {
	return 1234;
}
)";

	Data data;
	data.code = code.c_str();
	data.code_end = code.c_str() + code.size();

	c2mir_options ops;
	memset(&ops, 0, sizeof(ops));
	ops.message_file = stdout;
	ops.output_file_name = nullptr;
	ops.include_dirs_num = include_dirs.size();
	ops.include_dirs = include_dirs.data();
	ops.macro_commands_num = macro_commands.size();
	ops.macro_commands = macro_commands.data();
	int result = c2mir_compile(ctx, &ops, &get_cfunc, (void*)&data, "test.c", nullptr);
	if (!result) {
		printf("Failed to compile C code\n");
		fflush(stdout);
		return 1;
	}

	printf("*** Compilation successful\n");
	fflush(stdout);

	auto *module = DLIST_TAIL(MIR_module_t, *MIR_get_module_list(ctx));
	if (!module) {
		printf("No module found\n");
		fflush(stdout);
		return 1;
	}
	MIR_load_module(ctx, module);
	MIR_link(ctx, MIR_set_gen_interface, import_resolver);

	int (*fun_addr)(void *) = NULL;
	fun_addr = (int(*)(void *))mir_get_func(ctx, module, "test");

	// Finish the code-generation
	MIR_gen_finish(ctx);
	c2mir_finish(ctx);

	// Call the function
	printf("Calling function test at %p\n", fun_addr);
	result = fun_addr(nullptr);
	printf("Function result: %d\n", result);
	fflush(stdout);

	MIR_finish (ctx);

	return Nil;
}
#endif
