//12S25027_Jessica Mariani Situmorang

#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 1000
#define MAX_LOGS 1000
#define MAX_LINE 1024

typedef struct {
    int log_id;
    char type[20];
    int quantity;
} LogEntry;

typedef struct {
    char id[50];
    char name[100];
    int quantity;
    double price;

    LogEntry logs[MAX_LOGS];
    int log_count;
} Item;

Item items[MAX_ITEMS];
int item_count = 0;
int global_log_id = 1;



void trim_newline(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

void safe_copy(char *dest, const char *src, size_t size) {
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

int find_item(const char *id) {
    for (int i = 0; i < item_count; i++) {
        if (strcmp(items[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void add_log(Item *item, const char *type, int quantity) {
    if (item == NULL) return;
    if (item->log_count >= MAX_LOGS) return;

    item->logs[item->log_count].log_id = global_log_id++;
    safe_copy(item->logs[item->log_count].type, type, sizeof(item->logs[item->log_count].type));
    item->logs[item->log_count].quantity = quantity;
    item->log_count++;
}

void handle_receive(const char *id, const char *name, int quantity, double price) {
    if (id == NULL || name == NULL) return;
    if (item_count >= MAX_ITEMS) return;
    if (find_item(id) != -1) return;
    if (quantity <= 0) return;
    if (price < 0) return;

    safe_copy(items[item_count].id, id, sizeof(items[item_count].id));
    safe_copy(items[item_count].name, name, sizeof(items[item_count].name));
    items[item_count].quantity = quantity;
    items[item_count].price = price;
    items[item_count].log_count = 0;

    add_log(&items[item_count], "receive", quantity);
    item_count++;
}

void handle_ship(const char *id, int quantity) {
    int idx = find_item(id);
    if (idx == -1) return;
    if (quantity <= 0) return;
    if (items[idx].quantity < quantity) return;

    items[idx].quantity -= quantity;
    add_log(&items[idx], "ship", quantity);
}

void handle_restock(const char *id, int quantity) {
    int idx = find_item(id);
    if (idx == -1) return;
    if (quantity <= 0) return;

    items[idx].quantity += quantity;
    add_log(&items[idx], "restock", quantity);
}

void handle_report(void) {
    for (int i = 0; i < item_count; i++) {
        printf("%s|%s|%d|%.1f\n",
               items[i].id,
               items[i].name,
               items[i].quantity,
               items[i].price);
    }
}

void handle_audit(const char *id) {
    int idx = find_item(id);
    if (idx == -1) return;

    printf("%s|%s|%d|%.1f\n",
           items[idx].id,
           items[idx].name,
           items[idx].quantity,
           items[idx].price);

    for (int i = 0; i < items[idx].log_count; i++) {
        printf("%d|%s|%d\n",
               items[idx].logs[i].log_id,
               items[idx].logs[i].type,
               items[idx].logs[i].quantity);
    }
}

int main(void) {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        trim_newline(line);

        if (strcmp(line, "---") == 0) {
            break;
        }

        if (strncmp(line, "receive#", 8) == 0) {
            char cmd[20], id[50], name[100];
            int quantity;
            double price;

            if (sscanf(line, "%19[^#]#%49[^#]#%99[^#]#%d#%lf",
                       cmd, id, name, &quantity, &price) == 5) {
                handle_receive(id, name, quantity, price);
            }
        } else if (strncmp(line, "ship#", 5) == 0) {
            char cmd[20], id[50];
            int quantity;

            if (sscanf(line, "%19[^#]#%49[^#]#%d", cmd, id, &quantity) == 3) {
                handle_ship(id, quantity);
            }
        } else if (strncmp(line, "restock#", 8) == 0) {
            char cmd[20], id[50];
            int quantity;

            if (sscanf(line, "%19[^#]#%49[^#]#%d", cmd, id, &quantity) == 3) {
                handle_restock(id, quantity);
            }
        } else if (strcmp(line, "report") == 0) {
            handle_report();
        } else if (strncmp(line, "audit#", 6) == 0) {
            char cmd[20], id[50];

            if (sscanf(line, "%19[^#]#%49[^#]", cmd, id) == 2) {
                handle_audit(id);
            }
        }
    }

    return 0;
}
