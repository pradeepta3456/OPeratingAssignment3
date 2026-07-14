#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERS_FILE "users.txt"

/* ---------------------- Authentication ---------------------------------
 * Credentials are stored in a plain text file "users.txt" in the format
 *   username:password
 * one entry per line. */
int authenticate(const char *username, const char *password) {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        char *sep = strchr(line, ':');
        if (!sep) continue;
        *sep = '\0';
        char *file_user = line;
        char *file_pass = sep + 1;
        if (strcmp(file_user, username) == 0 && strcmp(file_pass, password) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* ---------------------- Permission model --------------------------------
 * A minimal owner/group/others rwx model, e.g. "rw-r--r--". */
typedef struct {
    char perms[10];
} FilePermission;

int has_permission(FilePermission fp, int is_owner, int is_group, char action) {
    int offset = is_owner ? 0 : (is_group ? 3 : 6);
    if (action == 'r') return fp.perms[offset] == 'r';
    if (action == 'w') return fp.perms[offset + 1] == 'w';
    if (action == 'x') return fp.perms[offset + 2] == 'x';
    return 0;
}

int main(void) {
    /* seed a demo credentials file if one does not already exist */
    FILE *check = fopen(USERS_FILE, "r");
    if (!check) {
        FILE *f = fopen(USERS_FILE, "w");
        fprintf(f, "admin:admin123\n");
        fprintf(f, "student:pass123\n");
        fclose(f);
    } else {
        fclose(check);
    }

    char username[64] = "admin";
    char password[64] = "admin123";

    printf("=== Authentication ===\n");
    if (!authenticate(username, password)) {
        printf("Authentication failed.\n");
        return 1;
    }
    printf("User '%s' authenticated successfully.\n\n", username);

    FilePermission fp = { "rw-r--r--" }; /* owner: rw-, group: r--, others: r-- */
    printf("=== Permission Check ===\n");
    printf("Owner write allowed?  %s\n", has_permission(fp, 1, 0, 'w') ? "YES" : "NO");
    printf("Others write allowed? %s\n", has_permission(fp, 0, 0, 'w') ? "YES" : "NO");

    return 0;
}
