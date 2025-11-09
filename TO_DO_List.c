#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LEN 100
#define DATE_LEN 11   // "DD/MM/YYYY" + '\0'
#define TIME_LEN 6    // "HH:MM" + '\0'
#define MAX_TASKS 1000

typedef struct {
    char name[NAME_LEN];
    char date[DATE_LEN];   // DD/MM/YYYY
    char time[TIME_LEN];   // HH:MM
    int  status;           // 0 = Not Done, 1 = Done
} Todo;

const char *FILENAME = "tasks.txt";

/* ---------------- ADD ---------------- */
void add_task() {
    Todo tasks[MAX_TASKS];
    int n = 0;

    /* load existing Not Done tasks */
    {
        FILE *fp = fopen(FILENAME, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp) && n < MAX_TASKS) {
                Todo t;
                if (sscanf(line, "%99[^|]|%10[^|]|%5[^|]|%d", t.name, t.date, t.time, &t.status) == 4) {
                    if (t.status == 0) tasks[n++] = t;
                }
            }
            fclose(fp);
        }
    }

    /* input */
    Todo t;
    printf("Enter Task Name: ");
    if (fgets(t.name, NAME_LEN, stdin)) {
        size_t L = strlen(t.name);
        if (L && t.name[L-1] == '\n') t.name[L-1] = '\0';
    }

    printf("Enter Deadline Date (DD/MM/YYYY): ");
    if (fgets(t.date, DATE_LEN, stdin)) {
        size_t L = strlen(t.date);
        if (L && t.date[L-1] == '\n') t.date[L-1] = '\0';
    }

    /* clear any stray newline before time (safe even if nothing to clear) */
    { int ch; while ((ch = getchar()) != '\n' && ch != EOF) { } }

    printf("Enter Deadline Time (HH:MM): ");
    if (fgets(t.time, TIME_LEN, stdin)) {
        size_t L = strlen(t.time);
        if (L && t.time[L-1] == '\n') t.time[L-1] = '\0';
    }

    t.status = 0;
    if (n < MAX_TASKS) tasks[n++] = t;

    /* sort by date then time (inline bubble sort) */
    {
        for (int i = 0; i < n-1; ++i) {
            for (int j = 0; j < n-1-i; ++j) {
                int d1=0,m1=0,y1=0,h1=0,min1=0, d2=0,m2=0,y2=0,h2=0,min2=0;
                sscanf(tasks[j].date, "%d/%d/%d", &d1, &m1, &y1);
                sscanf(tasks[j].time, "%d:%d", &h1, &min1);
                sscanf(tasks[j+1].date, "%d/%d/%d", &d2, &m2, &y2);
                sscanf(tasks[j+1].time, "%d:%d", &h2, &min2);

                int swap = 0;
                if (y1 > y2) swap = 1;
                else if (y1 == y2 && m1 > m2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 > d2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 == d2 &&
                         (h1 > h2 || (h1 == h2 && min1 > min2))) swap = 1;

                if (swap) { Todo tmp = tasks[j]; tasks[j] = tasks[j+1]; tasks[j+1] = tmp; }
            }
        }
    }

    /* save (write only Not Done, so Done lines vanish) */
    {
        FILE *fp = fopen(FILENAME, "w");
        if (!fp) { puts("Error writing file!"); return; }
        for (int i = 0; i < n; ++i) {
            if (tasks[i].status == 0) {
                fprintf(fp, "%s|%s|%s|%d\n", tasks[i].name, tasks[i].date, tasks[i].time, tasks[i].status);
            }
        }
        fclose(fp);
    }

    puts("Task added.");
}

/* ---------------- DISPLAY ---------------- */
void display_all() {
    Todo tasks[MAX_TASKS];
    int n = 0;

    /* load Not Done */
    {
        FILE *fp = fopen(FILENAME, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp) && n < MAX_TASKS) {
                Todo t;
                if (sscanf(line, "%99[^|]|%10[^|]|%5[^|]|%d", t.name, t.date, t.time, &t.status) == 4) {
                    if (t.status == 0) tasks[n++] = t;
                }
            }
            fclose(fp);
        }
    }

    /* sort */
    {
        for (int i = 0; i < n-1; ++i) {
            for (int j = 0; j < n-1-i; ++j) {
                int d1=0,m1=0,y1=0,h1=0,min1=0, d2=0,m2=0,y2=0,h2=0,min2=0;
                sscanf(tasks[j].date, "%d/%d/%d", &d1, &m1, &y1);
                sscanf(tasks[j].time, "%d:%d", &h1, &min1);
                sscanf(tasks[j+1].date, "%d/%d/%d", &d2, &m2, &y2);
                sscanf(tasks[j+1].time, "%d:%d", &h2, &min2);

                int swap = 0;
                if (y1 > y2) swap = 1;
                else if (y1 == y2 && m1 > m2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 > d2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 == d2 &&
                         (h1 > h2 || (h1 == h2 && min1 > min2))) swap = 1;

                if (swap) { Todo tmp = tasks[j]; tasks[j] = tasks[j+1]; tasks[j+1] = tmp; }
            }
        }
    }

    if (n == 0) { puts("\n(No tasks)"); return; }

    printf("\n%-3s %-30s %-12s %-7s %-10s\n", "#", "Task Name", "Date", "Time", "Status");
    for (int i=0;i<70;i++) putchar('-'); putchar('\n');

    for (int i = 0; i < n; ++i) {
        printf("%-3d %-30.30s %-12s %-7s %-10s\n",
               i+1, tasks[i].name, tasks[i].date, tasks[i].time,
               tasks[i].status ? "Done" : "Not Done");
    }
}

/* ---------------- SEARCH ---------------- */
void search_by_name() {
    Todo tasks[MAX_TASKS];
    int n = 0;

    /* load Not Done */
    {
        FILE *fp = fopen(FILENAME, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp) && n < MAX_TASKS) {
                Todo t;
                if (sscanf(line, "%99[^|]|%10[^|]|%5[^|]|%d", t.name, t.date, t.time, &t.status) == 4) {
                    if (t.status == 0) tasks[n++] = t;
                }
            }
            fclose(fp);
        }
    }

    char q[NAME_LEN];
    printf("Enter name to search (part or full): ");
    if (fgets(q, sizeof(q), stdin)) {
        size_t L = strlen(q);
        if (L && q[L-1] == '\n') q[L-1] = '\0';
    }

    int found = 0;
    for (int i = 0; i < n; ++i) {
        /* simple case-insensitive contains using local lowercase copies */
        char a[128], b[128];
        snprintf(a, sizeof(a), "%s", tasks[i].name);
        snprintf(b, sizeof(b), "%s", q);
        for (char *p=a; *p; ++p) if (*p>='A'&&*p<='Z') *p = *p - 'A' + 'a';
        for (char *p=b; *p; ++p) if (*p>='A'&&*p<='Z') *p = *p - 'A' + 'a';

        if (strstr(a, b)) {
            if (!found) {
                printf("\nMatches:\n");
                printf("%-3s %-30s %-12s %-7s %-10s\n", "#", "Task Name", "Date", "Time", "Status");
                for (int k=0;k<70;k++) putchar('-'); putchar('\n');
            }
            found = 1;
            printf("%-3d %-30.30s %-12s %-7s %-10s\n",
                   i+1, tasks[i].name, tasks[i].date, tasks[i].time,
                   tasks[i].status ? "Done" : "Not Done");
        }
    }
    if (!found) puts("No matching tasks.");
}

/* ---------------- UPDATE (edit; Done=delete) ---------------- */
void update_task() {
    Todo tasks[MAX_TASKS];
    int n = 0;

    /* load Not Done */
    {
        FILE *fp = fopen(FILENAME, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp) && n < MAX_TASKS) {
                Todo t;
                if (sscanf(line, "%99[^|]|%10[^|]|%5[^|]|%d", t.name, t.date, t.time, &t.status) == 4) {
                    if (t.status == 0) tasks[n++] = t;
                }
            }
            fclose(fp);
        }
    }

    if (n == 0) { puts("No tasks to update."); return; }

    /* sort for stable numbering */
    {
        for (int i = 0; i < n-1; ++i) {
            for (int j = 0; j < n-1-i; ++j) {
                int d1=0,m1=0,y1=0,h1=0,min1=0, d2=0,m2=0,y2=0,h2=0,min2=0;
                sscanf(tasks[j].date, "%d/%d/%d", &d1, &m1, &y1);
                sscanf(tasks[j].time, "%d:%d", &h1, &min1);
                sscanf(tasks[j+1].date, "%d/%d/%d", &d2, &m2, &y2);
                sscanf(tasks[j+1].time, "%d:%d", &h2, &min2);
                int swap = 0;
                if (y1 > y2) swap = 1;
                else if (y1 == y2 && m1 > m2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 > d2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 == d2 &&
                         (h1 > h2 || (h1 == h2 && min1 > min2))) swap = 1;
                if (swap) { Todo tmp = tasks[j]; tasks[j] = tasks[j+1]; tasks[j+1] = tmp; }
            }
        }
    }

    /* show */
    printf("\n%-3s %-30s %-12s %-7s %-10s\n", "#", "Task Name", "Date", "Time", "Status");
    for (int i=0;i<70;i++) putchar('-'); putchar('\n');
    for (int i = 0; i < n; ++i) {
        printf("%-3d %-30.30s %-12s %-7s %-10s\n",
               i+1, tasks[i].name, tasks[i].date, tasks[i].time,
               tasks[i].status ? "Done" : "Not Done");
    }

    /* pick */
    printf("\nEnter task number to update: ");
    char line[32]; int idx = 0;
    if (!fgets(line, sizeof(line), stdin) || sscanf(line, "%d", &idx) != 1) { puts("Invalid input."); return; }
    if (idx < 1 || idx > n) { puts("Out of range."); return; }
    int i = idx - 1;

    printf("\n(Press Enter to keep the old value)\n");

    char buf[128];

    printf("Task Name [%s]: ", tasks[i].name);
    if (fgets(buf, sizeof(buf), stdin)) {
        size_t L = strlen(buf); if (L && buf[L-1] == '\n') buf[L-1] = '\0';
        if (buf[0]) snprintf(tasks[i].name, NAME_LEN, "%s", buf);
    }

    printf("Deadline Date (DD/MM/YYYY) [%s]: ", tasks[i].date);
    if (fgets(buf, sizeof(buf), stdin)) {
        size_t L = strlen(buf); if (L && buf[L-1] == '\n') buf[L-1] = '\0';
        if (buf[0]) snprintf(tasks[i].date, DATE_LEN, "%s", buf);
    }

    printf("Deadline Time (HH:MM) [%s]: ", tasks[i].time);
    if (fgets(buf, sizeof(buf), stdin)) {
        size_t L = strlen(buf); if (L && buf[L-1] == '\n') buf[L-1] = '\0';
        if (buf[0]) snprintf(tasks[i].time, TIME_LEN, "%s", buf);
    }

    printf("Status (0=Not Done, 1=Done) [%d]: ", tasks[i].status);
    if (fgets(buf, sizeof(buf), stdin)) {
        int st; if (sscanf(buf, "%d", &st) == 1) tasks[i].status = (st!=0)?1:0;
    }

    if (tasks[i].status == 1) {
        /* delete by shifting */
        for (int k = i+1; k < n; ++k) tasks[k-1] = tasks[k];
        n--;
        puts("Task marked Done and removed.");
    } else {
        puts("Task updated.");
    }

    /* re-sort and save only Not Done */
    {
        for (int i2 = 0; i2 < n-1; ++i2) {
            for (int j = 0; j < n-1-i2; ++j) {
                int d1=0,m1=0,y1=0,h1=0,min1=0, d2=0,m2=0,y2=0,h2=0,min2=0;
                sscanf(tasks[j].date, "%d/%d/%d", &d1, &m1, &y1);
                sscanf(tasks[j].time, "%d:%d", &h1, &min1);
                sscanf(tasks[j+1].date, "%d/%d/%d", &d2, &m2, &y2);
                sscanf(tasks[j+1].time, "%d:%d", &h2, &min2);
                int swap = 0;
                if (y1 > y2) swap = 1;
                else if (y1 == y2 && m1 > m2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 > d2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 == d2 &&
                         (h1 > h2 || (h1 == h2 && min1 > min2))) swap = 1;
                if (swap) { Todo tmp = tasks[j]; tasks[j] = tasks[j+1]; tasks[j+1] = tmp; }
            }
        }

        FILE *fp = fopen(FILENAME, "w");
        if (!fp) { puts("Error writing file!"); return; }
        for (int r = 0; r < n; ++r) {
            if (tasks[r].status == 0) {
                fprintf(fp, "%s|%s|%s|%d\n", tasks[r].name, tasks[r].date, tasks[r].time, tasks[r].status);
            }
        }
        fclose(fp);
    }
}

/* ---------------- DELETE ---------------- */
void delete_task() {
    Todo tasks[MAX_TASKS];
    int n = 0;

    /* load Not Done */
    {
        FILE *fp = fopen(FILENAME, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp) && n < MAX_TASKS) {
                Todo t;
                if (sscanf(line, "%99[^|]|%10[^|]|%5[^|]|%d", t.name, t.date, t.time, &t.status) == 4) {
                    if (t.status == 0) tasks[n++] = t;
                }
            }
            fclose(fp);
        }
    }

    if (n == 0) { puts("No tasks to delete."); return; }

    /* sort for stable numbering */
    {
        for (int i = 0; i < n-1; ++i) {
            for (int j = 0; j < n-1-i; ++j) {
                int d1=0,m1=0,y1=0,h1=0,min1=0, d2=0,m2=0,y2=0,h2=0,min2=0;
                sscanf(tasks[j].date, "%d/%d/%d", &d1, &m1, &y1);
                sscanf(tasks[j].time, "%d:%d", &h1, &min1);
                sscanf(tasks[j+1].date, "%d/%d/%d", &d2, &m2, &y2);
                sscanf(tasks[j+1].time, "%d:%d", &h2, &min2);
                int swap = 0;
                if (y1 > y2) swap = 1;
                else if (y1 == y2 && m1 > m2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 > d2) swap = 1;
                else if (y1 == y2 && m1 == m2 && d1 == d2 &&
                         (h1 > h2 || (h1 == h2 && min1 > min2))) swap = 1;
                if (swap) { Todo tmp = tasks[j]; tasks[j] = tasks[j+1]; tasks[j+1] = tmp; }
            }
        }
    }

    /* show and pick */
    printf("\n%-3s %-30s %-12s %-7s %-10s\n", "#", "Task Name", "Date", "Time", "Status");
    for (int i=0;i<70;i++) putchar('-'); putchar('\n');
    for (int i = 0; i < n; ++i) {
        printf("%-3d %-30.30s %-12s %-7s %-10s\n",
               i+1, tasks[i].name, tasks[i].date, tasks[i].time,
               tasks[i].status ? "Done" : "Not Done");
    }

    printf("\nEnter task number to delete: ");
    char line[32]; int idx = 0;
    if (!fgets(line, sizeof(line), stdin) || sscanf(line, "%d", &idx) != 1) { puts("Invalid input."); return; }
    if (idx < 1 || idx > n) { puts("Out of range."); return; }

    for (int k = idx; k < n; ++k) tasks[k-1] = tasks[k];
    n--;

    /* save (only Not Done) */
    {
        FILE *fp = fopen(FILENAME, "w");
        if (!fp) { puts("Error writing file!"); return; }
        for (int r = 0; r < n; ++r) {
            if (tasks[r].status == 0) {
                fprintf(fp, "%s|%s|%s|%d\n", tasks[r].name, tasks[r].date, tasks[r].time, tasks[r].status);
            }
        }
        fclose(fp);
    }

    puts("Task deleted.");
}

/* ---------------- MAIN ---------------- */
int main() {
    int choice = -1;
    char line[32];

    while (1) {
        printf("\n===== TODO LIST (text file) =====\n");
        printf("1. Add Task\n");
        printf("2. Display All Tasks (sorted)\n");
        printf("3. Search Task by Name\n");
        printf("4. Update Task (edit; Done = remove)\n");
        printf("5. Delete Task\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (!fgets(line, sizeof(line), stdin) || sscanf(line, "%d", &choice) != 1) {
            puts("Please enter a number.");
            continue;
        }

        if (choice == 0) { puts("Goodbye!"); break; }
        else if (choice == 1) add_task();
        else if (choice == 2) display_all();
        else if (choice == 3) search_by_name();
        else if (choice == 4) update_task();
        else if (choice == 5) delete_task();
        else puts("Invalid choice.");
    }

    return 0;
}
