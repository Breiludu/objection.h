# objection.h

*Tiny header-only unit testing utility with awful naming*

---

This is a **personal project** created for learning.

The **awful naming** (sorry) comes from the *Ace Attorney* games, where lawyers shout **"Objection!"** in court. You can now yell at your screen complaining about why the code you just wrote should have worked.

---

## Usage

* All **trials** (tests) are written as independent funcions.
* **Objections** (assertions) are made inside trials to check statements that should evaluate to true. When an objection fails, it is reported to stderr.
* Trials can be gruped inside of another function from where they are called to **sentence** wheter a module tested worked or not.

## Example

```c
#include "objection.h"

void try_example1(void) {
    OBJECTION(1 == 0);      // Will fail and be reported to stderr
}

void try_example2(void) {
    OBJECTION(1 == 1);      // Will pass
}

void sentence_example(void) {
    TRIAL(try_example1);    // Calls try_example1
    TRIAL(try_example2);    // Calls try_example2
}

int main(void) {
    SENTENCE(sentence_example); // Runs all trials in the sentence
    return 0;
}
```

This code prints the following:

```
>>> SENTENCE of sentence_example <<<

TRIAL try_example1:
        **FAILED: 1 == 0
        objections: 1
        failed: 1

TRIAL try_example2:
        objections: 1
        failed: 0

total objections: 2
total failed: 1

>>> END of sentence_example <<<
```

---

**Work in progress**

Suggestions... I mean -- Objections are accepted.

Use at your own risk.
