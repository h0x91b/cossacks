"""Render a BMP through both its embedded palette and the game palette.
Outputs two 24-bit BMP files for visual comparison."""
import struct, sys, os

def read_pal(path):
    """Read game palette (768 bytes, RGB triplets)"""
    with open(path, 'rb') as f:
        data = f.read(768)
    pal = []
    for i in range(256):
        r, g, b = data[i*3], data[i*3+1], data[i*3+2]
        pal.append((r, g, b))
    return pal

def read_bmp8(path):
    """Read 8-bit BMP, return (width, height, pixels, bmp_palette)"""
    with open(path, 'rb') as f:
        # File header (14 bytes)
        bfType, bfSize, r1, r2, bfOffBits = struct.unpack('<HIHHI', f.read(14))
        assert bfType == 0x4D42, "Not a BMP"

        # DIB header (variable size, first 4 bytes = biSize)
        biSize = struct.unpack('<I', f.read(4))[0]
        # Read rest of the standard fields
        biWidth, biHeight = struct.unpack('<ii', f.read(8))
        biPlanes, biBitCount = struct.unpack('<HH', f.read(4))
        assert biBitCount == 8, f"Not 8-bit (got {biBitCount})"

        # Skip to palette
        palOffset = 14 + biSize
        f.seek(palOffset)
        pal_data = f.read(1024)
        bmp_pal = []
        for i in range(256):
            b, g, r, a = pal_data[i*4], pal_data[i*4+1], pal_data[i*4+2], pal_data[i*4+3]
            bmp_pal.append((r, g, b))

        # Read pixel data (bottom-up)
        wid = biWidth
        rwid = wid
        if rwid & 3:
            rwid = (rwid | 3) + 1

        pixels = bytearray(wid * biHeight)
        for i in range(biHeight):
            f.seek(bfOffBits + (biHeight - i - 1) * rwid)
            row = f.read(wid)
            pixels[i*wid:(i+1)*wid] = row

        return biWidth, biHeight, pixels, bmp_pal

def save_bmp24(path, width, height, rgb_data):
    """Save 24-bit BMP from RGB byte array"""
    rlx = width * 3
    if rlx & 3:
        rlx = (rlx | 3) + 1
    pad = rlx - width * 3

    file_size = 54 + rlx * height
    with open(path, 'wb') as f:
        # File header
        f.write(struct.pack('<HIHHI', 0x4D42, file_size, 0, 0, 54))
        # DIB header
        f.write(struct.pack('<IiiHHIIiiII', 40, width, height, 1, 24, 0, 0, 0, 0, 0, 0))
        # Pixel data (bottom-up)
        for y in range(height - 1, -1, -1):
            for x in range(width):
                idx = (y * width + x) * 3
                r, g, b = rgb_data[idx], rgb_data[idx+1], rgb_data[idx+2]
                f.write(bytes([b, g, r]))
            f.write(b'\x00' * pad)

def render_through_palette(width, height, pixels, palette):
    """Render indexed pixels through a palette, return RGB byte array"""
    rgb = bytearray(width * height * 3)
    for i in range(width * height):
        r, g, b = palette[pixels[i]]
        rgb[i*3] = r
        rgb[i*3+1] = g
        rgb[i*3+2] = b
    return rgb

def main():
    if len(sys.argv) < 3:
        print("Usage: python render_compare.py <bmp_file> <pal_file>")
        sys.exit(1)

    bmp_path = sys.argv[1]
    pal_path = sys.argv[2]

    game_pal = read_pal(pal_path)
    width, height, pixels, bmp_pal = read_bmp8(bmp_path)

    print(f"Image: {width}x{height}")
    print(f"BMP palette[1] = {bmp_pal[1]}")
    print(f"Game palette[1] = {game_pal[1]}")

    # Render through BMP palette
    rgb_bmp = render_through_palette(width, height, pixels, bmp_pal)
    # Render through game palette
    rgb_game = render_through_palette(width, height, pixels, game_pal)

    base = os.path.splitext(bmp_path)[0]
    out_bmp = base + "_VIA_BMP_PAL.BMP"
    out_game = base + "_VIA_GAME_PAL.BMP"

    save_bmp24(out_bmp, width, height, rgb_bmp)
    save_bmp24(out_game, width, height, rgb_game)

    print(f"Saved: {out_bmp}")
    print(f"Saved: {out_game}")
    print("Open both to see which one looks correct!")

if __name__ == "__main__":
    main()
