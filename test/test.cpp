#include "api.hpp"


extern "C" Variant empty_function() {
	return Variant();
}

extern "C" Variant calling_function(Variant callable) {
	return callable.call();
}

extern "C" Variant my_function(Variant varg) {
	print("Hello, ", 124.5, " world!\n");
	print("Arg: ", varg);
	return varg; //
}

extern "C" Variant function3(Variant x, Variant y, Variant text) {
	print("x = ", x, " y = ", y, " text = ", text);
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
	auto data = var.operator std::string_view();

	char buffer[256];
	snprintf(buffer, sizeof(buffer),
		"The buffer is not here! Or is it?\n");

	print(buffer);
	return var;
}

extern "C" Variant player_process(Variant v_time_passed) {
	return {};
}
