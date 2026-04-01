
BOOTLOADER_SIZE = 0x8000
BOOTLOADER_FILE = "Build/bootloader.bin"


with open(BOOTLOADER_FILE, "rb") as f:
    raw_file = f.read()

print(f"Booloader size: {len(raw_file)} bytes")

bytes_to_pad = BOOTLOADER_SIZE - len(raw_file)
print(f"Bytes that needs to be padded: {bytes_to_pad} bytes")

padding = bytes([0xff for _ in range(bytes_to_pad)])

with open(BOOTLOADER_FILE, "wb") as f:
    f.write(raw_file + padding)
