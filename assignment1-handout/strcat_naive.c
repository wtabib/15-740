char *my_strcat(char *dest, const char *src) {
    char *tmp = dest;

    while (*dest != '\0') {
        dest++;
    }
    for (;;) {
        *dest = *src;
        if (*src == '\0') {
            break;
        }
        src++;
        dest++;
    }
    
    return tmp;
}