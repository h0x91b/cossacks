/*
 * gsc_extract - GSC archive extractor for Cossacks: Back to War
 *
 * Usage:
 *   gsc_extract.exe <archive.gsc> -l                  List files
 *   gsc_extract.exe <archive.gsc> -x [-o <outdir>]    Extract all
 *   gsc_extract.exe <archive.gsc> -x <name> [-o dir]  Extract one file
 *
 * Build (MSVC):
 *   cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS gsc_extract.c /Fe:gsc_extract.exe
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <direct.h>
#include <windows.h>

#pragma pack(push, 1)

typedef struct {
    unsigned char  descriptor[6];
    unsigned short version;
    unsigned short key;
    unsigned int   entries;
} GSCHeader;   /* 14 bytes */

typedef struct {
    unsigned int   hash;
    unsigned char  filename[64];
    unsigned int   offset;    /* bitwise-inverted */
    unsigned int   size;
    unsigned int   reserved;
    unsigned char  flags;     /* 0=plain, 1=encrypted */
} GSCFatEntry;  /* 81 bytes */

#pragma pack(pop)

/* ---------- decryption ---------- */

static void gsc_decrypt(unsigned char *buf, unsigned int size, unsigned char key)
{
    unsigned int i;
    for (i = 0; i < size; i++) {
        buf[i] = (unsigned char)((~buf[i]) ^ key);
    }
}

/* ---------- hash (big-endian DWORD sum over 64-byte uppercase name) ---------- */

static unsigned int gsc_calc_hash(const char *name)
{
    unsigned char padded[64];
    unsigned int hash = 0;
    int i;

    memset(padded, 0, 64);
    for (i = 0; i < 64 && name[i]; i++)
        padded[i] = (unsigned char)toupper((unsigned char)name[i]);

    for (i = 0; i < 16; i++) {
        unsigned int d = ((unsigned int)padded[i*4]   << 24)
                       | ((unsigned int)padded[i*4+1] << 16)
                       | ((unsigned int)padded[i*4+2] << 8)
                       | ((unsigned int)padded[i*4+3]);
        hash += d;
    }
    return hash;
}

/* ---------- helpers ---------- */

static void str_upper(char *s)
{
    while (*s) { *s = (char)toupper((unsigned char)*s); s++; }
}

/* Recursively create directories for a path like "a\b\c\file.bmp" */
static void ensure_directories(const char *filepath)
{
    char tmp[MAX_PATH];
    char *p;

    strncpy(tmp, filepath, MAX_PATH - 1);
    tmp[MAX_PATH - 1] = '\0';

    /* Convert forward slashes */
    for (p = tmp; *p; p++) {
        if (*p == '/') *p = '\\';
    }

    /* Walk path components and create dirs */
    for (p = tmp; *p; p++) {
        if (*p == '\\') {
            *p = '\0';
            if (tmp[0] != '\0') {
                _mkdir(tmp);
            }
            *p = '\\';
        }
    }
}

/* ---------- archive loading ---------- */

typedef struct {
    unsigned char *data;       /* entire file in memory */
    unsigned int   file_size;
    GSCHeader     *header;
    GSCFatEntry   *fat;
    unsigned char *blob;       /* start of data section */
    unsigned char  crypt_key;  /* derived decryption key */
} GSCArchive;

static int gsc_open(GSCArchive *arc, const char *path)
{
    FILE *f;
    long fsize;

    f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Error: cannot open '%s'\n", path);
        return 0;
    }

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize < (long)sizeof(GSCHeader)) {
        fprintf(stderr, "Error: file too small to be a GSC archive\n");
        fclose(f);
        return 0;
    }

    arc->file_size = (unsigned int)fsize;
    arc->data = (unsigned char *)malloc(arc->file_size);
    if (!arc->data) {
        fprintf(stderr, "Error: out of memory (%u bytes)\n", arc->file_size);
        fclose(f);
        return 0;
    }

    if (fread(arc->data, 1, arc->file_size, f) != arc->file_size) {
        fprintf(stderr, "Error: failed to read archive\n");
        free(arc->data);
        fclose(f);
        return 0;
    }
    fclose(f);

    arc->header = (GSCHeader *)arc->data;
    arc->fat = (GSCFatEntry *)(arc->data + sizeof(GSCHeader));
    arc->blob = arc->data + sizeof(GSCHeader)
              + (arc->header->entries * sizeof(GSCFatEntry));

    /* Derive decryption key: NOT of high byte of the archive key field */
    arc->crypt_key = (unsigned char)(~(arc->header->key >> 8));

    return 1;
}

static void gsc_close(GSCArchive *arc)
{
    if (arc->data) {
        free(arc->data);
        arc->data = NULL;
    }
}

/* ---------- commands ---------- */

static void cmd_info(GSCArchive *arc, const char *path)
{
    printf("Archive:     %s\n", path);
    printf("Descriptor:  %02X %02X %02X %02X %02X %02X\n",
        arc->header->descriptor[0], arc->header->descriptor[1],
        arc->header->descriptor[2], arc->header->descriptor[3],
        arc->header->descriptor[4], arc->header->descriptor[5]);
    printf("Version:     %u\n", arc->header->version);
    printf("Key:         0x%04X\n", arc->header->key);
    printf("Entries:     %u\n", arc->header->entries);
    printf("Header size: %u bytes\n",
        (unsigned)(sizeof(GSCHeader) + arc->header->entries * sizeof(GSCFatEntry)));
    printf("\n");
}

static void cmd_list(GSCArchive *arc)
{
    unsigned int i;
    unsigned long long total = 0;

    printf("  %-8s %-8s %-5s %-10s %s\n", "Index", "Hash", "Enc", "Size", "Filename");
    printf("  %-8s %-8s %-5s %-10s %s\n", "-----", "--------", "---", "----------", "--------");

    for (i = 0; i < arc->header->entries; i++) {
        GSCFatEntry *e = &arc->fat[i];
        printf("  %-8u %08X %-5s %-10u %s\n",
            i, e->hash, e->flags ? "yes" : "no", e->size, e->filename);
        total += e->size;
    }

    printf("\n  %u files, %llu bytes total\n", arc->header->entries, total);
}

static int extract_entry(GSCArchive *arc, unsigned int idx, const char *outdir)
{
    GSCFatEntry *e = &arc->fat[idx];
    unsigned int real_offset = ~e->offset;
    unsigned char *src;
    unsigned char *buf;
    char outpath[MAX_PATH];
    FILE *fout;

    /* Bounds check */
    if (arc->blob + real_offset + e->size > arc->data + arc->file_size) {
        fprintf(stderr, "  WARNING: entry %u '%s' extends beyond archive, skipping\n",
            idx, e->filename);
        return 0;
    }

    src = arc->blob + real_offset;

    /* Build output path */
    if (outdir && outdir[0]) {
        _snprintf(outpath, MAX_PATH, "%s\\%s", outdir, (char *)e->filename);
    } else {
        _snprintf(outpath, MAX_PATH, "%s", (char *)e->filename);
    }
    outpath[MAX_PATH - 1] = '\0';

    /* Ensure parent directories exist */
    ensure_directories(outpath);

    /* Allocate buffer and copy data */
    buf = (unsigned char *)malloc(e->size);
    if (!buf) {
        fprintf(stderr, "  ERROR: out of memory for '%s' (%u bytes)\n",
            e->filename, e->size);
        return 0;
    }
    memcpy(buf, src, e->size);

    /* Decrypt if needed */
    if (e->flags) {
        gsc_decrypt(buf, e->size, arc->crypt_key);
    }

    /* Write file */
    fout = fopen(outpath, "wb");
    if (!fout) {
        fprintf(stderr, "  ERROR: cannot create '%s'\n", outpath);
        free(buf);
        return 0;
    }

    fwrite(buf, 1, e->size, fout);
    fclose(fout);
    free(buf);

    return 1;
}

static int cmd_extract_all(GSCArchive *arc, const char *outdir)
{
    unsigned int i;
    unsigned int ok = 0, fail = 0;

    for (i = 0; i < arc->header->entries; i++) {
        printf("  [%u/%u] %s", i + 1, arc->header->entries, arc->fat[i].filename);
        if (extract_entry(arc, i, outdir)) {
            printf(" OK\n");
            ok++;
        } else {
            printf(" FAILED\n");
            fail++;
        }
    }

    printf("\nExtracted %u files", ok);
    if (fail) printf(", %u failed", fail);
    printf(".\n");

    return fail == 0;
}

static int cmd_extract_one(GSCArchive *arc, const char *name, const char *outdir)
{
    unsigned int i;
    char upper_name[64];

    memset(upper_name, 0, 64);
    strncpy(upper_name, name, 63);
    str_upper(upper_name);

    /* Also try with backslash normalization */
    for (i = 0; upper_name[i]; i++) {
        if (upper_name[i] == '/') upper_name[i] = '\\';
    }

    for (i = 0; i < arc->header->entries; i++) {
        char entry_name[64];
        memset(entry_name, 0, 64);
        strncpy(entry_name, (char *)arc->fat[i].filename, 63);
        str_upper(entry_name);

        if (strcmp(entry_name, upper_name) == 0) {
            printf("  Extracting: %s (%u bytes%s)\n",
                arc->fat[i].filename, arc->fat[i].size,
                arc->fat[i].flags ? ", encrypted" : "");
            if (extract_entry(arc, i, outdir)) {
                printf("  Done.\n");
                return 1;
            }
            return 0;
        }
    }

    fprintf(stderr, "Error: file '%s' not found in archive\n", name);
    return 0;
}

static int cmd_verify(GSCArchive *arc)
{
    unsigned int i;
    unsigned int ok = 0, bad = 0;

    printf("Verifying hashes...\n");

    for (i = 0; i < arc->header->entries; i++) {
        GSCFatEntry *e = &arc->fat[i];
        unsigned int computed = gsc_calc_hash((char *)e->filename);
        if (computed != e->hash) {
            printf("  MISMATCH [%u] '%s': stored=%08X computed=%08X\n",
                i, e->filename, e->hash, computed);
            bad++;
        } else {
            ok++;
        }
    }

    printf("%u OK, %u mismatched\n", ok, bad);
    return bad == 0;
}

/* ---------- usage ---------- */

static void usage(const char *progname)
{
    printf("GSC Archive Extractor for Cossacks: Back to War\n\n");
    printf("Usage:\n");
    printf("  %s <archive.gsc> -l                  List all files\n", progname);
    printf("  %s <archive.gsc> -x [-o <outdir>]    Extract all files\n", progname);
    printf("  %s <archive.gsc> -x <name> [-o dir]  Extract one file\n", progname);
    printf("  %s <archive.gsc> -i                   Show archive info\n", progname);
    printf("  %s <archive.gsc> -v                   Verify hashes\n", progname);
    printf("\nOptions:\n");
    printf("  -l          List files in the archive\n");
    printf("  -x          Extract files\n");
    printf("  -i          Show archive header info\n");
    printf("  -v          Verify FAT entry hashes\n");
    printf("  -o <dir>    Output directory (default: current directory)\n");
}

/* ---------- main ---------- */

int main(int argc, char *argv[])
{
    GSCArchive arc;
    const char *archive_path = NULL;
    const char *outdir = NULL;
    const char *extract_name = NULL;
    int mode = 0;  /* 0=none, 'l'=list, 'x'=extract, 'i'=info, 'v'=verify */
    int i;
    int result = 0;

    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    archive_path = argv[1];

    /* Parse arguments */
    for (i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            mode = 'l';
        } else if (strcmp(argv[i], "-x") == 0) {
            mode = 'x';
            /* Check if next arg is a filename (not another flag) */
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                extract_name = argv[++i];
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                outdir = argv[++i];
            } else {
                fprintf(stderr, "Error: -o requires a directory argument\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            mode = 'i';
        } else if (strcmp(argv[i], "-v") == 0) {
            mode = 'v';
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (mode == 0) {
        fprintf(stderr, "Error: no command specified (-l, -x, -i, or -v)\n");
        usage(argv[0]);
        return 1;
    }

    /* Open archive */
    memset(&arc, 0, sizeof(arc));
    if (!gsc_open(&arc, archive_path)) {
        return 1;
    }

    /* Create output directory if needed */
    if (outdir && outdir[0]) {
        _mkdir(outdir);
    }

    /* Execute command */
    switch (mode) {
    case 'i':
        cmd_info(&arc, archive_path);
        cmd_list(&arc);
        break;

    case 'l':
        cmd_list(&arc);
        break;

    case 'x':
        cmd_info(&arc, archive_path);
        if (extract_name) {
            result = cmd_extract_one(&arc, extract_name, outdir) ? 0 : 1;
        } else {
            result = cmd_extract_all(&arc, outdir) ? 0 : 1;
        }
        break;

    case 'v':
        cmd_info(&arc, archive_path);
        result = cmd_verify(&arc) ? 0 : 1;
        break;
    }

    gsc_close(&arc);
    return result;
}
