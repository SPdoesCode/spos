const input = @import("../input.zig");
const output = @import("../output.zig");

/// the virtio driver
pub const virtio = struct {
    /// Variables to help with pci activitys
    const queue_size = 8;
    const pci_config_address_port: u16 = 0xCF8;
    const pci_config_data_port: u16 = 0xCFC;
    pub const MAX_VIRTIO_DEVICES = 8;

    var io_base: u16 = 0;
    pub var io_bases: [MAX_VIRTIO_DEVICES]u16 = undefined;
    pub var device_count: usize = 0;

    /// error handling
    const errors = enum {
        NotQeue,
        FullQeue,
        NoError,
    };

    /// Structs to help aswell
    const Descriptor = packed struct {
        addr: u64,
        len: u32,
        flags: u16,
        next: u16,
    };
    const Avail = packed struct {
        flags: u16,
        idx: u16,
        ring: [queue_size]u16,
        unused: u16,
    };
    const UsedElem = packed struct {
        id: u32,
        len: u32,
    };
    const Used = packed struct {
        flags: u16,
        idx: u16,
        ring: [queue_size]UsedElem,
    };
    const VirtQueue = extern struct {
        desc: [queue_size]Descriptor,
        avail: Avail,
        used: Used,
    };

    /// the queue and buffer
    var queue: VirtQueue = undefined;
    var buffer: [512]u8 = undefined;

    /// Read from the PCI
    pub fn pci_config_read(bus: u8, slot: u8, func: u8, offset: u8) u32 {
        const address = (1 << 31) |
            (@as(u32, bus) << 16) |
            (@as(u32, slot) << 11) |
            (@as(u32, func) << 8) |
            (@as(u32, offset & 0xFC));

        output.outl(pci_config_address_port, address);
        return input.inl(pci_config_data_port);
    }

    /// detects the virtio blocs
    pub fn detectblock() void {
        device_count = 0;

        const bus: u8 = 0;
        for (0..32) |slot| {
            const func: u8 = 0;

            const id: u32 = pci_config_read(bus, @truncate(slot), func, 0);
            const vendor: u16 = @intCast(id & 0xFFFF);
            const device: u16 = @intCast((id >> 16) & 0xFFFF);

            if (vendor == 0x1AF4 and (device == 0x1001 or device == 0x1042)) {
                // get the bar0
                const bar0_raw: u32 = pci_config_read(bus, @truncate(slot), func, 0x10);

                // bar0 lower stuff is flags so mask em out for adds
                const bar0_base: u16 = @intCast(bar0_raw & 0xFFF0);

                if (device_count < MAX_VIRTIO_DEVICES) {
                    io_bases[device_count] = bar0_base;
                    device_count += 1;
                }
            }
        }
    }
};
