#include "api.hpp"

int main() {
	static bool timer_created = true;
	if (!timer_created) {
		timer_created = true;
		const char *val1 = "123";
		const int val2 = 456;
		Timer::periodic(0.1, [=] (Variant timer) -> Variant {
			print("Timer with values: ", val1, val2);
			static int counter = 0;
			if (++counter == 4)
				timer.as_node().queue_free();
			return {};
		});
	}
}

extern "C" Variant empty_function() {
	return Variant();
}
extern "C" Variant arg1_function() {
	return Variant();
}

extern "C" Variant calling_function(Variant& callable) {
	return callable.call();
}

extern "C" Variant my_function(Variant varg) {
	print("Hello, ", 124.5, " world!\n");
	print("Arg: ", varg);
	return varg; //
}

extern "C" Variant function3(Variant x, Variant y, Variant text) {
	Array a({x, y, text});
	a.push_front(2);
	a.pop_front();
	Dictionary d;
	d["test1"] = "test2";
	d[Vector3()] = 0.5;
	d[String("A string")] = String("A value");
	d[String("A string")] = String("Another value");
	std::u32string stest = *d["A string"];
	print("Stest: ", stest);

	print("x = ", x, " y = ", y, " array = ", a, " dict = ", d);
	return text;
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
