#include "utils.h"

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pressEnterToContinue(void) {
    printf("Press Enter to continue...\n");
    clearInputBuffer();
}

void clearScreen(void) {
#ifdef _WIN32
    (void)system("cls");
#else
    const char *term = getenv("TERM");
    if (term && term[0] != '\0') {
        (void)system("clear");
    }
#endif
}

int getIntInput(const char *prompt) {
    int val;
    printf("%s", prompt);
    if (scanf("%d", &val) != 1) {
        clearInputBuffer();
        return -1;
    }
    clearInputBuffer();
    return val;
}

float getFloatInput(const char *prompt) {
    float val;
    printf("%s", prompt);
    if (scanf("%f", &val) != 1) {
        clearInputBuffer();
        return -1.0f;
    }
    clearInputBuffer();
    return val;
}

void getStringInput(const char *prompt, char *buffer, int maxLen) {
    if (!buffer || maxLen <= 0) return;
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clearInputBuffer();
        }
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\r') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

bool isValidEmail(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return false;
    if (strchr(at + 1, '@')) return false; // exactly one @
    const char *dot = strchr(at + 1, '.');
    return dot != NULL;
}

bool isValidPhone(const char *phone) {
    int digits = 0;
    int i = (phone[0] == '+') ? 1 : 0;
    for (; phone[i] != '\0'; i++) {
        if (!isdigit((unsigned char)phone[i])) return false;
        digits++;
    }
    return digits >= 10 && digits <= 15;
}

void formatDate(time_t t, char *buffer, int bufLen) {
    if (t == 0) {
        strncpy(buffer, "N/A", bufLen);
        if (bufLen > 0) buffer[bufLen - 1] = '\0';
        return;
    }
    struct tm *tm_info = localtime(&t);
    if (!tm_info) {
        strncpy(buffer, "N/A", bufLen);
        buffer[bufLen - 1] = '\0';
        return;
    }
    strftime(buffer, bufLen, "%d-%b-%Y", tm_info);
}

time_t addDays(time_t t, int days) {
    return t + (time_t)(days * 86400);
}

int daysBetween(time_t t1, time_t t2) {
    int days = (int)(difftime(t2, t1) / 86400.0);
    return days < 0 ? 0 : days;
}

void printHeader(const char *title) {
    printSeparator();
    int len = (int)strlen(title);
    int pad = (48 - len) / 2;
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) putchar(' ');
    printf("%s\n", title);
    printSeparator();
}

void printSeparator(void) {
    for (int i = 0; i < 48; i++) {
        putchar('=');
    }
    putchar('\n');
}

void toLowerStr(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}
