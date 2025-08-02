const vga = @import("imports/vga.zig").textmode;
const ps2 = @import("imports/input.zig").PS2;

pub fn panic(msg: []const u8, trace: ?*anyopaque) callconv(.C) noreturn {
    _ = trace;
    vga.println("ERROR:", null, null, null);
    vga.println(msg, null, null, null);
    while (true) {
        asm volatile ("hlt");
    }
}

pub export fn kmain() callconv(.C) void {
    vga.clearscreen(null);
    vga.writeline("this is a multiline test", null, null, null);
    vga.writeline("this is a multiline test", null, null, null);
    vga.writeline("this is a multiline test", null, null, null);
    vga.writeline("this is a multiline test", null, null, null);
    vga.writeline("this is a multiline test", null, null, null);
    vga.writeline("write stuff:", null, null, null);
    while (true) {
        const output: []const u8 = vga.getstring(null, null, null);
        _ = output;
    }
}
