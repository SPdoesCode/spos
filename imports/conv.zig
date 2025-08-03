pub const u16_to = struct {
    pub var buf: [6]u8 = undefined;
    pub fn hex(value: u16) []const u8 {
        const hex_chars = "0123456789ABCDEF";
        buf[0] = '0';
        buf[1] = 'x';
        buf[2] = hex_chars[(value >> 12) & 0xF];
        buf[3] = hex_chars[(value >> 8) & 0xF];
        buf[4] = hex_chars[(value >> 4) & 0xF];
        buf[5] = hex_chars[value & 0xF];
        return buf[0..6];
    }
};
