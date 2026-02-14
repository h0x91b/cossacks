#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#pragma pack(push, 1)
typedef struct {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
	DWORD   biSize;
	LONG    biWidth;
	LONG    biHeight;
	WORD    biPlanes;
	WORD    biBitCount;
	DWORD   biCompression;
	DWORD   biSizeImage;
	LONG    biXPelsPerMeter;
	LONG    biYPelsPerMeter;
	DWORD   biClrUsed;
	DWORD   biClrImportant;
} BMPformat;
#pragma pack(pop)

typedef unsigned char byte;

int main(int argc, char* argv[])
{
	if (argc < 3) {
		printf("Usage: bmp_remap_test.exe <bmp_file> <pal_file>\n");
		printf("Example: bmp_remap_test.exe BACKGROUND_MAIN_MENU.BMP agew_1.pal\n");
		return 1;
	}

	const char* bmpPath = argv[1];
	const char* palPath = argv[2];

	/* ---- Read game palette ---- */
	byte gamePal[768];
	memset(gamePal, 0, sizeof gamePal);
	{
		HANDLE h = CreateFileA(palPath, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (h == INVALID_HANDLE_VALUE) {
			printf("ERROR: Cannot open palette file: %s\n", palPath);
			printf("  GetLastError = %lu\n", GetLastError());
			return 1;
		}
		DWORD bytesRead = 0;
		ReadFile(h, gamePal, 768, &bytesRead, NULL);
		CloseHandle(h);
		printf("Game palette: read %lu bytes from %s\n", bytesRead, palPath);
		if (bytesRead != 768) {
			printf("ERROR: Expected 768 bytes, got %lu\n", bytesRead);
			return 1;
		}
		printf("  First 8 entries (R,G,B):\n");
		for (int i = 0; i < 8; i++) {
			printf("    [%3d] = (%3d, %3d, %3d)\n", i,
				gamePal[i*3+0], gamePal[i*3+1], gamePal[i*3+2]);
		}
		printf("  ...\n");
		for (int i = 248; i < 256; i++) {
			printf("    [%3d] = (%3d, %3d, %3d)\n", i,
				gamePal[i*3+0], gamePal[i*3+1], gamePal[i*3+2]);
		}
	}

	/* ---- Read BMP header ---- */
	BMPformat BM;
	FILE* f = fopen(bmpPath, "rb");
	if (!f) {
		printf("ERROR: Cannot open BMP file: %s\n", bmpPath);
		return 1;
	}
	fread(&BM, sizeof(BMPformat), 1, f);

	printf("\nBMP header for: %s\n", bmpPath);
	printf("  bfType       = 0x%04X ('%c%c')\n", BM.bfType, BM.bfType & 0xFF, (BM.bfType >> 8) & 0xFF);
	printf("  bfSize       = %lu\n", BM.bfSize);
	printf("  bfOffBits    = %lu\n", BM.bfOffBits);
	printf("  biSize       = %lu\n", BM.biSize);
	printf("  biWidth      = %ld\n", BM.biWidth);
	printf("  biHeight     = %ld\n", BM.biHeight);
	printf("  biBitCount   = %u\n", BM.biBitCount);
	printf("  biClrUsed    = %lu\n", BM.biClrUsed);

	if (BM.bfType != 0x4D42) { /* 'BM' */
		printf("ERROR: Not a BMP file\n");
		fclose(f);
		return 1;
	}
	if (BM.biBitCount != 8) {
		printf("ERROR: Not an 8-bit BMP (biBitCount=%u)\n", BM.biBitCount);
		fclose(f);
		return 1;
	}

	DWORD palOffset = 14 + BM.biSize;
	DWORD palSize = BM.bfOffBits - palOffset;
	printf("\n  palOffset    = %lu  (14 + biSize)\n", palOffset);
	printf("  palSize      = %lu  (bfOffBits - palOffset)\n", palSize);
	printf("  bfOffBits > palOffset? %s\n", BM.bfOffBits > palOffset ? "YES" : "NO");

	/* ---- Read BMP palette ---- */
	byte bmpPal[1024];
	memset(bmpPal, 0, sizeof bmpPal);
	int hasPalette = 0;
	if (BM.bfOffBits > palOffset && palSize >= 1024) {
		fseek(f, palOffset, SEEK_SET);
		size_t r = fread(bmpPal, 1, 1024, f);
		printf("  BMP palette: read %zu bytes\n", r);
		hasPalette = 1;

		printf("\n  BMP palette first 8 entries (B,G,R,A -> R,G,B):\n");
		for (int i = 0; i < 8; i++) {
			printf("    [%3d] = B=%3d G=%3d R=%3d A=%3d  -> RGB(%3d,%3d,%3d)\n", i,
				bmpPal[i*4+0], bmpPal[i*4+1], bmpPal[i*4+2], bmpPal[i*4+3],
				bmpPal[i*4+2], bmpPal[i*4+1], bmpPal[i*4+0]);
		}
		printf("  ...\n");
		for (int i = 248; i < 256; i++) {
			printf("    [%3d] = B=%3d G=%3d R=%3d A=%3d  -> RGB(%3d,%3d,%3d)\n", i,
				bmpPal[i*4+0], bmpPal[i*4+1], bmpPal[i*4+2], bmpPal[i*4+3],
				bmpPal[i*4+2], bmpPal[i*4+1], bmpPal[i*4+0]);
		}
	} else {
		printf("  No BMP palette block (or too small)\n");
	}

	/* ---- Check if BMP palette is all zeros ---- */
	if (hasPalette) {
		int allZero = 1;
		for (int i = 0; i < 1024; i++) {
			if (bmpPal[i] != 0) { allZero = 0; break; }
		}
		printf("\n  BMP palette all zeros? %s\n", allZero ? "YES" : "NO");
	}

	/* ---- Compute distance ---- */
	int totalDist = 0;
	if (hasPalette) {
		int maxEntryDist = 0;
		int maxEntryIdx = 0;
		int nonZeroBmp = 0;
		int nonZeroGame = 0;

		for (int c = 0; c < 256; c++) {
			int bR = bmpPal[c * 4 + 2];
			int bG = bmpPal[c * 4 + 1];
			int bB = bmpPal[c * 4 + 0];
			int gR = gamePal[c * 3 + 0];
			int gG = gamePal[c * 3 + 1];
			int gB = gamePal[c * 3 + 2];

			if (bR || bG || bB) nonZeroBmp++;
			if (gR || gG || gB) nonZeroGame++;

			int dr = bR - gR; if (dr < 0) dr = -dr;
			int dg = bG - gG; if (dg < 0) dg = -dg;
			int db = bB - gB; if (db < 0) db = -db;
			int entryDist = dr + dg + db;
			totalDist += entryDist;
			if (entryDist > maxEntryDist) {
				maxEntryDist = entryDist;
				maxEntryIdx = c;
			}
		}

		printf("\n  === DISTANCE ANALYSIS ===\n");
		printf("  totalDist        = %d\n", totalDist);
		printf("  threshold        = 1000\n");
		printf("  REMAP TRIGGERED? = %s\n", totalDist > 1000 ? "YES" : "NO");
		printf("  maxEntryDist     = %d (at index %d)\n", maxEntryDist, maxEntryIdx);
		printf("  nonZeroBmpEntries  = %d / 256\n", nonZeroBmp);
		printf("  nonZeroGameEntries = %d / 256\n", nonZeroGame);

		/* ---- Build remap table ---- */
		if (totalDist > 1000) {
			byte remap[256];
			printf("\n  === REMAP TABLE (first 16 + last 8) ===\n");
			for (int c = 0; c < 256; c++) {
				int bR = bmpPal[c * 4 + 2];
				int bG = bmpPal[c * 4 + 1];
				int bB = bmpPal[c * 4 + 0];
				int bestIdx = 0;
				int bestDist = 0x7FFFFFFF;
				for (int g = 0; g < 256; g++) {
					int gR = gamePal[g * 3 + 0];
					int gG = gamePal[g * 3 + 1];
					int gB = gamePal[g * 3 + 2];
					int dr = bR - gR; if (dr < 0) dr = -dr;
					int dg = bG - gG; if (dg < 0) dg = -dg;
					int db = bB - gB; if (db < 0) db = -db;
					int dist = dr + dg + db;
					if (dist < bestDist) {
						bestDist = dist;
						bestIdx = g;
					}
				}
				remap[c] = (byte)bestIdx;
				if (c < 16 || c >= 248) {
					printf("    remap[%3d] = %3d  (BMP RGB(%3d,%3d,%3d) -> Game[%3d] RGB(%3d,%3d,%3d), dist=%d)\n",
						c, bestIdx,
						bR, bG, bB,
						bestIdx, gamePal[bestIdx*3], gamePal[bestIdx*3+1], gamePal[bestIdx*3+2],
						bestDist);
				}
				if (c == 16) printf("    ...\n");
			}

			/* ---- Read pixel data and show histogram ---- */
			int wid = BM.biWidth;
			int rwid = wid;
			if (wid & 3) rwid = (wid | 3) + 1;

			byte* pixels = (byte*)malloc(wid * BM.biHeight);
			if (pixels) {
				for (int i = 0; i < BM.biHeight; i++) {
					fseek(f, BM.bfOffBits + (long)(BM.biHeight - i - 1) * rwid, SEEK_SET);
					fread(&pixels[i * wid], 1, wid, f);
				}

				/* Histogram of top 10 most used indices before remap */
				int hist[256] = {0};
				int totalPix = wid * BM.biHeight;
				for (int p = 0; p < totalPix; p++)
					hist[pixels[p]]++;

				printf("\n  === PIXEL HISTOGRAM (top 10 before remap) ===\n");
				for (int rank = 0; rank < 10; rank++) {
					int bestC = 0, bestN = 0;
					for (int c = 0; c < 256; c++) {
						if (hist[c] > bestN) { bestN = hist[c]; bestC = c; }
					}
					if (bestN == 0) break;
					printf("    index %3d: %7d pixels (%5.1f%%)  -> remap to %3d\n",
						bestC, bestN, 100.0 * bestN / totalPix, remap[bestC]);
					hist[bestC] = 0;
				}

				free(pixels);
			}

		} else {
			printf("\n  Remap NOT triggered (totalDist <= 1000). Original indices preserved.\n");
		}
	}

	/* ---- COMBINED DETECTION ---- */
	printf("\n  === COMBINED DETECTION LOGIC ===\n");
	printf("  biSize = %lu\n", BM.biSize);
	if (BM.biSize > 40) {
		printf("  biSize > 40 (V4/V5 header) -> ORIGINAL GAME BMP -> SKIP REMAP\n");
	} else if (!hasPalette) {
		printf("  No embedded palette -> SKIP REMAP\n");
	} else if (totalDist <= 1000) {
		printf("  biSize == 40, but totalDist=%d <= 1000 -> palette matches game -> SKIP REMAP\n", totalDist);
	} else {
		printf("  biSize == 40, totalDist=%d > 1000 -> EDITOR-SAVED BMP -> APPLY REMAP\n", totalDist);
	}

	/* ---- Read a few raw pixel bytes at offset to verify data ---- */
	{
		printf("\n  === RAW PIXEL BYTES (first 16 of first row) ===\n");
		fseek(f, BM.bfOffBits, SEEK_SET);
		byte raw[16];
		fread(raw, 1, 16, f);
		printf("    ");
		for (int i = 0; i < 16; i++) printf("%02X ", raw[i]);
		printf("\n");
	}

	fclose(f);

	printf("\nDone.\n");
	return 0;
}
