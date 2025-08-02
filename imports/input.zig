/// Simple PS2 driver for the 0x60 port.
pub const PS2 = struct {
    /// Default keymap.
    pub const keymap: [128]u8 = blk: {
        var map: [128]u8 = undefined;

        // Control characters
        map[0x01] = 0x1B; // ESC
        map[0x0E] = '\x08'; // Backspace
        map[0x0F] = '\t'; // Tab
        map[0x1C] = '\n'; // Enter
        map[0x39] = ' '; // Spacebar

        // Number row
        map[0x02] = '1';
        map[0x03] = '2';
        map[0x04] = '3';
        map[0x05] = '4';
        map[0x06] = '5';
        map[0x07] = '6';
        map[0x08] = '7';
        map[0x09] = '8';
        map[0x0A] = '9';
        map[0x0B] = '0';
        map[0x0C] = '-';
        map[0x0D] = '=';

        // Top row
        map[0x10] = 'q';
        map[0x11] = 'w';
        map[0x12] = 'e';
        map[0x13] = 'r';
        map[0x14] = 't';
        map[0x15] = 'y';
        map[0x16] = 'u';
        map[0x17] = 'i';
        map[0x18] = 'o';
        map[0x19] = 'p';
        map[0x1A] = '[';
        map[0x1B] = ']';

        // Home row
        map[0x1E] = 'a';
        map[0x1F] = 's';
        map[0x20] = 'd';
        map[0x21] = 'f';
        map[0x22] = 'g';
        map[0x23] = 'h';
        map[0x24] = 'j';
        map[0x25] = 'k';
        map[0x26] = 'l';
        map[0x27] = ';';
        map[0x28] = '\'';
        map[0x29] = '`';

        // Bottom row
        map[0x2C] = 'z';
        map[0x2D] = 'x';
        map[0x2E] = 'c';
        map[0x2F] = 'v';
        map[0x30] = 'b';
        map[0x31] = 'n';
        map[0x32] = 'm';
        map[0x33] = ',';
        map[0x34] = '.';
        map[0x35] = '/';

        // Misc.
        map[0x2A] = 0; // Left Shift
        map[0x36] = 0; // Right Shift
        map[0x1D] = 0; // Ctrl
        map[0x38] = 0; // Alt
        map[0x3A] = 0; // CapsLock

        break :blk map;
    };

    pub const NOVALUE_INPUT: u8 = 255;

    /// Gets a byte of input from a port.
    pub fn inb(port: u16) u8 {
        return asm volatile ("inb %[port], %[ret]"
            : [ret] "={al}" (-> u8),
            : [port] "{dx}" (port),
        );
    }

    /// Outputs a byte to a port.
    pub fn outb(port: u16, value: u8) void {
        asm volatile ("outb %[value], %[port]"
            :
            : [value] "{al}" (value),
              [port] "{dx}" (port),
            : "memory"
        );
    }

    /// Decodes a key code, good to use with inb.
    pub fn decodecode(code: u8) u8 {
        var ch: u8 = NOVALUE_INPUT;
        if ((code & 0x80) == 0 and code < keymap.len) {
            ch = keymap[code & 0x7F];
        }
        return ch;
    }

    // Returns a key input from the 0x60 port.
    pub fn getkey() u8 {
        while ((inb(0x64) & 0x01) == 0) {}
        return decodecode(inb(0x60));
    }

    /// Returns a zig string (max size of 128) of non-echoed input ended by a newline.
    pub fn getstring() []const u8 {
        var buff: [128]u8 = undefined;
        var i: usize = 0;

        while (true) {
            const key: u8 = getkey();
            if (key == NOVALUE_INPUT) continue;

            if (key == '\n') {
                break;
            }

            if (key == '\x08') {
                i -= 1;
                continue;
            }

            if (i < buff.len) {
                buff[i] = key;
                i += 1;
            }
        }

        return buff[0..i];
    }
};
