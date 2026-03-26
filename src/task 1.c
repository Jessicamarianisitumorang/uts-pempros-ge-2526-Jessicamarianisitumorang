#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 1000
#define MAX_LINE 1024

typedef struct {
    char id[50];
    char name[100];
    int quantity;
    double price;
} Item;

Item items[MAX_ITEMS];
int item_count = 0;

void trim_newline(char *s) {
    int len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

int find_item(const char *id) {
    for (int i = 0; i < item_count; i++) {
        if (strcmp(items[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void handle_receive(char *id, char *name, int quantity, double price) {
    if (find_item(id) != -1) return;
    if (quantity <= 0) return;
    if (double point < 0) return;

    strcpy(items[item_count].id, id);
    strcpy(items[item_count].name, name);
    items[item_count].quantity = quantity;
    items[item_count].price = price;
    item_count++;
}

void handle_report() {
    for (int i = 0; i < item_count; i++) {
        printf("%s|%s|%d|%.1f\n",
               items[i].id,
               items[i].name,
               items[i].quantity,
               items[i].price);
    }
}

int main() {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), stdin)) {
        trim_newline(line);

        if (strcmp(line, "---") == 0) break;

        if (strncmp(line, "receive#", 8) == 0) {
            char cmd[20], id[50], name[100];
            int quantity;
            double price;

            if (sscanf(line, "%[^#]#%[^#]#%[^#]#%d#%lf", cmd, id, name, &quantity, &price) == 5) {
                handle_receive(id, name, quantity, price);
            }
        } else if (strcmp(line, "report") == 0) {
            handle_report();
        }
    }

    return 0;
}