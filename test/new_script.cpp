#include "api.hpp"

extern "C" Variant public_function(Variant arg) {
	print("Arguments: ", arg);
	return "Hello from the other side";
}
