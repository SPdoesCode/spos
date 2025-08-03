const vga = @import("imports/vga.zig").textmode;
const kshell = @import("imports/kshell.zig").vga.textmode;
const virtio = @import("imports/drivers/ide.zig").virtio;
// zig will freak without this.
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
    vga.writeline("Detecting blocks...", null, null, null);
    virtio.detectblock();
    vga.writeline("Booted into spos, starting shell...", null, null, null);
    kshell.init();
    while (true) {
        asm volatile ("hlt");
    }
}
