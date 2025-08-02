/// A decent set of utilitys to utilize zig strings in many ways.
pub const utils = struct {
    /// This and res is needed cuz else it js removes the variable causing crashes
    var buff: [10][220]u8 = undefined;
    var res: [10][]const u8 = undefined;
    /// Returns a array of zig strings seperated at the sepchar.
    pub fn sep(ztring: []const u8, sepchar: u8) [][]const u8 {
        var i: usize = 0;
        var j: u8 = 0;
        var sepnum: u8 = 0;
        while (i < ztring.len) : (i += 1) {
            if (ztring[i] == sepchar) {
                res[sepnum] = buff[sepnum][0..j];
                sepnum += 1;
                j = 0;
                continue;
            }
            buff[sepnum][j] = ztring[i];
            j += 1;
        }
        if (j > 0) {
            res[sepnum] = buff[sepnum][0..j];
            sepnum += 1;
        }
        return res[0..sepnum];
    }
    /// Compares the two zstrings.
    pub fn cmp(ztring: []const u8, toztring: []const u8) bool {
        var i: usize = 0;
        if (ztring.len != toztring.len) return false;
        while (i < ztring.len) : (i += 1) {
            if (ztring[i] != toztring[i]) {
                return false;
            }
        }
        return true;
    }
    /// Only way to use this function sadly
    var compbuff: [2200]u8 = undefined;
    /// Combinds the given array of zstrings to one ztring with combchar as the char between each zstring in the arry of them.
    pub fn comb(zstrings: [][]const u8, combchar: u8) []const u8 {
        if (zstrings.len == 0) return compbuff[0..0];
        var i: usize = 0;
        var l: usize = 0;
        while (i < zstrings.len) : (i += 1) {
            var j: usize = 0;
            while (j < zstrings[i].len) : (j += 1) {
                compbuff[l] = zstrings[i][j];
                l += 1;
            }
            if (i < zstrings.len - 1) {
                compbuff[l] = combchar;
                l += 1;
            }
        }
        return compbuff[0..l];
    }
};
