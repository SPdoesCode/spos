const string = @import("strings.zig").utils;
const virtio = @import("drivers/ide.zig").virtio;
const conv = @import("conv.zig");

/// The vga driver prompt.
pub const vga = struct {
    /// The textmode part of vga.
    pub const textmode = struct {
        /// Imported because it is textmode vga.
        const vgaout = @import("vga.zig").textmode;

        /// The Global shell startup prompt.
        pub var initpromt: []const u8 = "Wellcome to spos";
        /// The Global shell cli prompt.
        pub var shellprompt: []const u8 = ">";
        /// The shell color.
        pub var shellcolor: u8 = 0x0F;

        /// The vga evaluator for vga commands.
        pub fn evaluate(command: []const u8) void {
            const fullcmd: [][]const u8 = string.sep(command, ' ');
            const cmd: []const u8 = fullcmd[0];
            const args: [][]const u8 = fullcmd[1..];
            if (string.cmp(cmd, "echo")) {
                vgaout.writeline(string.comb(args[0..], ' '), null, null, shellcolor);
            } else if (string.cmp(cmd, "clear")) {
                vgaout.clearscreen(shellcolor);
                vgaout.currentline = 0;
            } else if (string.cmp(cmd, "getblocks")) {
                virtio.detectblock();
                vgaout.writeline("Found:", null, null, shellcolor);
                for (virtio.io_bases[0..virtio.device_count]) |base| {
                    vgaout.writeline(conv.u16_to.hex(base), null, null, shellcolor);
                }
                vgaout.writeline("Done!", null, null, shellcolor);
            } else {
                vgaout.writeline("COMMAND: ", null, null, shellcolor);
                vgaout.writeline(cmd, null, null, shellcolor);
                vgaout.writeline("NOT FOUND!!", null, null, shellcolor);
            }
        }

        /// The vga textmode shell.
        pub fn init() void {
            vgaout.writeline(initpromt, null, null, shellcolor);
            const indentamt: u8 = @truncate(shellprompt.len); // to conv usize to u8
            while (true) {
                vgaout.writeline(shellprompt, null, null, shellcolor);
                const command: []const u8 = vgaout.getstring(vgaout.currentline - 1, indentamt, shellcolor);
                evaluate(command);
            }
        }
    };
};
