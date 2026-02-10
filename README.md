# objection.h

*Tiny header-only unit testing utility with awful naming*

---

This is a **personal project** created for learning.

The **awful naming** (sorry) comes from the *Ace Attorney* games, where lawyers shout **"Objection!"** in court. You can now yell at your screen complaining about why the code you just wrote should have worked.

---

## Usage

* **Trials** (tests) are independent funcions. Use the *TRIAL* macro to define a new one.
* Trials are **automatically called** by the main function.
* Do not define a main funciton inside the file containing the trials, as it is already defined in *objection.h*.
* **Objections** (assertions) are made inside trials to check statements that should evaluate to true. When an objection fails, it is reported to stdout.
* *COURT* macro is needed for naming the **court** (test file). If no name is passed in, the name is defaulted to \_\_FILE\_\_.
* Once you have your *court* file ready, compile and it will be ready to execute.

## Example

`example.c`

```c
#include "objection.h"
#include <string.h>

COURT(example);

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
```

Compile the file and execute:

```console
$ gcc -o example example.c
$ ./example
```

This code produces the following output:

```
[====] Unit testing with objection.h
[----] Court is: example
[----]
[FAIL]     OBJECTION(a == b)
[INFO]       In trial example_one -> (example.c, 7)
[----]
[PASS] Trial example_two passed
[----]
[INFO] Failed trials: example_one.
[----]
[====] Objections: 2 | Failed: 1
```

---

**Work in progress**

The api of this utility is not guaranteed to remain the same in future updates.

Suggestions... I mean -- Objections are accepted.

Use at your own risk.
