#include <api.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>

extern "C" void funlockfile(FILE *) {}
extern "C" void flockfile(FILE *) {}

extern "C" Variant generate_json_diff(String p_source, String p_reference) {
	using json = nlohmann::json;
	json source_json = json::parse(p_source.utf8().c_str());
	json reference_json = json::parse(p_reference.utf8().c_str());
	json patch = json::diff(source_json, reference_json);
	return patch.dump(4, ' ');
}
