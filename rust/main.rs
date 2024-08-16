mod sysalloc;
mod api;
use api::*;

pub fn main() {
}

#[no_mangle]
pub fn public_function() -> Variant {
	gprint(&Variant::new_string("Hello from Rust!"));

	return Variant::new_float(3.14);
}
