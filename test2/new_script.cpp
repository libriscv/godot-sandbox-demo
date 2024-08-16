#include "api.hpp"

int main() {
	UtilityFunctions::print("Hello, ", 123, " world from the second program!\n");

	// do shit here...

	halt(); // Prevent stdout,stderr closing etc.
}

extern "C" void another_function(Variant arg) {
	
}
