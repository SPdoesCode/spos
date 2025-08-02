const ps2 = @import("input.zig").PS2;

/// The text mode driver for writing to vga while in textmode.
pub const textmode = struct {
    const vga_buffer: [*]volatile u16 = @ptrFromInt(0xB8000);

    pub const MAX_X: usize = 80;
    pub const MAX_Y: usize = 25;

    pub var currentline: u8 = 0;
    pub var normcolor: u8 = 0x0F;

    /// Write a char to a location on the vga buffer.
    pub fn writechar(char: u8, y: u8, x: u8, color: u8) void {
        if (x >= MAX_X or y >= MAX_Y) return;
        const location: usize = y * MAX_X + x;

        vga_buffer[location] = (@as(u16, color) << 8) | @as(u16, char);
    }

    /// Clear the screen, good for before writing stuff.
    pub fn clearscreen(color: ?u8) void {
        const bcolor: u8 = color orelse normcolor;
        var x: u8 = 0;
        var y: u8 = 0;

        while (y < MAX_Y) : (y += 1) {
            x = 0;
            while (x < MAX_X) : (x += 1) {
                writechar(' ', y, x, bcolor);
            }
        }
    }

    /// Write a line to a tty like buffer.
    pub fn writeline(chars: []const u8, line: ?u8, indentby: ?u8, color: ?u8) void {
        var wline: u8 = line orelse currentline;
        const ccolor: u8 = color orelse normcolor;
        const ispace: u8 = indentby orelse 0;
        var i: usize = 0;
        var j: u8 = 0; // for incrimenting the char pos in the line

        while (i < chars.len) : (i += 1) {
            if (j >= MAX_X) { // change the line and then the x pos
                wline += 1;
                if (line == null) {
                    currentline += 1;
                } // if we are using currentline incriment it
                j = 0;
            }
            if (wline >= MAX_Y) {
                clearscreen(ccolor);
                if (line == null) {
                    currentline = 0;
                }
                wline = 0;
            }
            writechar(chars[i], wline, ispace + j, ccolor);
            j += 1;
        }

        if (line == null) {
            currentline += 1;
        }
    }

    // Returns a zig string (max size of 128) of echoed input termed by a newline.
    pub fn getstring(line: ?u8, indent: ?u8, color: ?u8) []const u8 {
        var y: u8 = line orelse currentline;
        var x: u8 = indent orelse 0;
        const c: u8 = color orelse normcolor;
        var buff: [128]u8 = undefined;
        var i: usize = 0;

        writechar('|', y, x, c); // inits the cursor
        while (true) {
            const key: u8 = ps2.getkey();
            if (key == ps2.NOVALUE_INPUT) continue;

            const debugmsg: []const u8 = &[_]u8{ 'K', 'E', 'Y', ' ', key, ' ', 'P', 'R', 'E', 'S', 'S', 'E', 'D', '!' };
            writeline(debugmsg, 24, 79 - 14, null);

            if (key == '\n') {
                writechar(' ', y, x, c); // clean up the extra |
                currentline += 1;
                y += 1;
                x = 0;
                break;
            }

            if (key == '\x08') {
                writechar(' ', y, x, c); // clean up the extra |
                if (x == 0) {
                    currentline -= 1;
                    y -= 1;
                    if (i > 0) {
                        i -= 1;
                    }
                    x = MAX_X - 1;
                } else {
                    x -= 1;
                    if (i > 0) {
                        i -= 1;
                    }
                }

                buff[i] = ' ';
                writechar('|', y, x, c);
                writechar(' ', y, x + 1, c); // clean up the extra |
                continue;
            }

            if (i < buff.len) {
                buff[i] = key;
                if (x >= MAX_X) {
                    currentline += 1;

                    x = 0;
                    y += 1;
                }
                if (y >= MAX_Y) {
                    clearscreen(color);
                    currentline = 0;
                    y = 0;
                }
                writechar(buff[i], y, x, c);
                if (x < MAX_X) writechar('|', y, x + 1, c);
                i += 1;
                x += 1;
            }
        }

        return buff[0..i];
    }

    /// A basic gui libary
    pub const GUI = struct {
        /// A Minimal titlebar thats quite basic.
        pub fn titlebar(chars: []const u8, ocolor: ?u8) void {
            const color: u8 = ocolor orelse normcolor;
            clearscreen(color);
            writeline("__|", 0, null, color);
            writeline(chars, 0, 3, color);
            var i: usize = 4 + chars.len;
            writeline("|", 0, @intCast(i - 1), color);
            while (i < MAX_X) : (i += 1) {
                writeline("_", 0, @intCast(i), color);
            }
        }
    };
};
