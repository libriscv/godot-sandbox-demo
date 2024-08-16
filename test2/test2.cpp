#include "api.hpp"

int main() {
	UtilityFunctions::print("Hello, ", 123, " world!\n");

	// do shit here...

	halt(); // Prevent stdout,stderr closing etc.
}

extern "C" void public_function(Variant arg) {
}
