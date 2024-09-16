const api = @import("api/api.zig");
const std = @import("std");

pub fn main() !void {
	const stdout = std.io.getStdOut().writer();
	try stdout.print("Hello {d} World from Zig!\n", .{123});
}

export fn add(result: *api.Variant, x: i32, y: i32) void {
	result.init_int(x + y);
}
