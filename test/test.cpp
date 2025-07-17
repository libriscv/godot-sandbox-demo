#include <api.hpp>

EMBED_BINARY(binary_data, "../../scenes/mod/lua.elf.gz")
__attribute__((noreturn))
static inline void vreturn(const Variant &x) {
	register const Variant *a0 asm("a0") = &x;

	asm(".insn i SYSTEM, 0, x0, x0, 0x7ff" : : "r"(a0), "m"(*a0));
	__builtin_unreachable();
}

static Variant test_sandbox_pass(Sandbox s) {
	Sandbox s2 = s;
	//s2 = s2.FromBuffer(PackedArray<uint8_t>((const uint8_t *)binary_data, binary_data_size));
	return s2;
}

static Variant empty_function() {
	return {};
	//vreturn({});
}
static Variant arg1_function(int, int, int, int, int, int, int) {
	return {};
}

static Variant method_call_bench() {
	//return Vector3 { 1.5, 2.5, 3.5 }("floor");
	Variant(Vector3 { 1.5, 2.5, 3.5 }).voidcall("floor");
	return Nil;
	//return Vector3 { 1.5, 2.5, 3.5 }.floor();
}

static Variant calling_function(Callable callable) {
	return callable.call();
}

static Variant my_function(Dictionary text) {
	//print("Hello, ", 124.5, " world!\n");
	//print("Arg: ", text);
	return text; //
}

static Variant function3(long x, long y, Array text) {
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
	return x + y;
}

static Variant adding_function(int a1, int a2, int a3, int a4, int a5, int a6) {
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

static Variant pba_operation(PackedArray<float> farr) {
	auto array = std::make_unique_for_overwrite<float[]>(100000);
	//for (int i = 0; i < 100000; i++)
	//	array[i] = 1.0f;
	int i = 0;
	for (; i < 100000; i += 8) {
		array[i] = 1.0f;
		array[i + 1] = 1.0f;
		array[i + 2] = 1.0f;
		array[i + 3] = 1.0f;
		array[i + 4] = 1.0f;
		array[i + 5] = 1.0f;
		array[i + 6] = 1.0f;
		array[i + 7] = 1.0f;
	}
	for (; i < 100000; i += 4) {
		array[i] = 1.0f;
		array[i + 1] = 1.0f;
		array[i + 2] = 1.0f;
		array[i + 3] = 1.0f;
	}

	//const float   f = 1.0f;
	//const int32_t val = *(int32_t *)&f;
	//memset_i32((int *)array.get(), val, 100000);
	farr.store(array.get(), 100000);
	return Nil;
}

extern "C" __attribute__((used, retain)) Variant get_embedded_luajit() {
	return PackedByteArray((const uint8_t *)binary_data, binary_data_size);
}

int main() {
	ADD_API_FUNCTION(my_function, "Dictionary", "Dictionary text");
	ADD_API_FUNCTION(empty_function, "void", "");
	ADD_API_FUNCTION(arg1_function, "void", "int, int, int, int, int, int, int");
	ADD_API_FUNCTION(function3, "int", "long x, long y, Array text");
	add_sandbox_api_function("function33", (void *)function3, "int", "long x, long y, Array text");
	ADD_API_FUNCTION(test_sandbox_pass, "Sandbox", "Sandbox s");
	ADD_API_FUNCTION(method_call_bench, "Variant", "");
	ADD_API_FUNCTION(calling_function, "Variant", "Callable callable");
	ADD_API_FUNCTION(adding_function, "int", "int a1, int a2, int a3, int a4, int a5, int a6");
	ADD_API_FUNCTION(pba_operation, "void", "PackedFloat32Array farr", "Sets each float in a PackedArray to 1.0f");
	ADD_API_FUNCTION(get_embedded_luajit, "PackedByteArray", "");

	extern String measure_obj_callp(Node obj);
	extern Variant createSkyDome(double r_inner64, double r_outer64);
	ADD_API_FUNCTION(measure_obj_callp, "String", "Node obj");
	ADD_API_FUNCTION(createSkyDome, "void", "double r_inner64, double r_outer64");

	static bool timer_created = true; //
	if (!timer_created) {
		timer_created = true;
		const char *val1 = "123";
		const int val2 = 456;
		CallbackTimer::native_periodic(0.1, [=] (Timer timer) -> Variant {
			print("Timer with values: ", val1, val2);
			static int counter = 0;
			if (++counter >= 4)
				timer.queue_free();
			return {};
		});
	}

	print(Vector3 { 1.5, 2.5, 3.5 }("floor")); //

	JSON j = ClassDB::instantiate<JSON>("JSON");
	j.parse(
	R"({
		"pi": 3.141,
		"happy": true,
		"name": "Niels Nielsen",
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
	print(j.get_data());
	//print(r2.get_pattern());
	Sandbox s = ClassDB::instantiate<Sandbox>("Sandbox");
	s.queue_free();
	print("Hot reload");
	print("Hot relo2d22");

	halt();
}
