#include <stdio.h>
#include <string.h>
#include <time.h>

#define AUDIT_LOG "audit_log.txt"
#define XOR_KEY   0x5A

void audit_log(const char *username, const char *action, const char *file) {
    FILE *f = fopen(AUDIT_LOG, "a");
    if (!f) return;
    time_t now = time(NULL);
    char *ts = ctime(&now);
    ts[strcspn(ts, "\n")] = 0;
    fprintf(f, "[%s] user=%s action=%s file=%s\n", ts, username, action, file);
    fclose(f);
}

/* XOR is its own inverse, so one function serves as both encrypt and decrypt */
void xor_encrypt_file(const char *username, const char *infile, const char *outfile) {
    FILE *in = fopen(infile, "rb");
    FILE *out = fopen(outfile, "wb");
    if (!in || !out) { printf("Encryption error.\n"); return; }
    int c;
    while ((c = fgetc(in)) != EOF)
        fputc(c ^ XOR_KEY, out);
    fclose(in);
    fclose(out);
    audit_log(username, "ENCRYPT", outfile);
    printf("'%s' encrypted -> '%s'.\n", infile, outfile);
}

void xor_decrypt_file(const char *username, const char *infile, const char *outfile) {
    xor_encrypt_file(username, infile, outfile);
    audit_log(username, "DECRYPT", outfile);
}

int main(void) {
    const char *username = "admin";

    /* create a small sample file to encrypt, so this program is self-contained */
    FILE *f = fopen("secret.txt", "w");
    fprintf(f, "Confidential exam results: 87%%\n");
    fclose(f);

    printf("=== Encryption ===\n");
    xor_encrypt_file(username, "secret.txt", "secret.enc");
    xor_decrypt_file(username, "secret.enc", "secret.dec");

    printf("---- decrypted contents ----\n");
    FILE *check = fopen("secret.dec", "r");
    char buf[256];
    while (fgets(buf, sizeof(buf), check)) printf("%s", buf);
    fclose(check);

    return 0;
}
