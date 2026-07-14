#include <stdio.h>
#include <string.h>
#include <time.h>

#define AUDIT_LOG "audit_log.txt"

void audit_log(const char *username, const char *action, const char *file) {
    FILE *f = fopen(AUDIT_LOG, "a");
    if (!f) return;
    time_t now = time(NULL);
    char *ts = ctime(&now);
    ts[strcspn(ts, "\n")] = 0;
    fprintf(f, "[%s] user=%s action=%s file=%s\n", ts, username, action, file);
    fclose(f);
}

void create_file_op(const char *username, const char *filename, const char *content) {
    FILE *f = fopen(filename, "w");
    if (!f) { printf("Error creating file.\n"); return; }
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    audit_log(username, "CREATE", filename);
    printf("File '%s' created.\n", filename);
}

void write_file_op(const char *username, const char *filename, const char *content) {
    FILE *f = fopen(filename, "a");
    if (!f) { printf("Error writing file.\n"); return; }
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    audit_log(username, "WRITE", filename);
    printf("Appended to '%s'.\n", filename);
}

void read_file_op(const char *username, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Error reading file.\n"); return; }
    char buf[1024];
    printf("---- contents of %s ----\n", filename);
    while (fgets(buf, sizeof(buf), f)) printf("%s", buf);
    printf("-------------------------\n");
    fclose(f);
    audit_log(username, "READ", filename);
}

void delete_file_op(const char *username, const char *filename) {
    if (remove(filename) == 0) {
        audit_log(username, "DELETE", filename);
        printf("File '%s' deleted.\n", filename);
    } else {
        printf("Error deleting file.\n");
    }
}

int main(void) {
    const char *username = "admin";

    printf("=== File Operations ===\n");
    create_file_op(username, "notes.txt", "Task 3 demo file.\n");
    write_file_op(username, "notes.txt", "This line was appended.\n");
    read_file_op(username, "notes.txt");
    delete_file_op(username, "notes.txt");

    printf("\nAudit log written to %s\n", AUDIT_LOG);
    return 0;
}
