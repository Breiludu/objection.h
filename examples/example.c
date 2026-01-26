#include "../objection.h"
#include <string.h>

TRIAL(example_one) {
    int a = 1;
    int b = 2;
    OBJECTION(a == b);
}

TRIAL(example_two) {
    char str1[] = "Objection!";
    char str2[] = "Objection!";
    OBJECTION(strcmp(str1, str2) == 0);
}
