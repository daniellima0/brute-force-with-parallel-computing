#include <stdio.h>

size_t my_strlen(str) const char *str;
{
    register const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}

int main() {
    char string[4] = "abc";

    printf("%d\n", my_strlen(string));

    return 0;
}