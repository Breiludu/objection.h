/* objection.h - main branch - (c) Breixo Luna Durán */

#include <stdio.h>
#include <stdint.h>

typedef void(*TrialFn)(void);

// .trials data section pointers
extern TrialFn __start_trials[];
extern TrialFn __stop_trials[];

// .trial_names data section pointers
extern char *__start_trial_names[];
extern char *__stop_trial_names[];

// Count objections made and failed
static uint16_t __total_objected = 0;
static uint16_t __total_failed   = 0;

// Name of the trial currently executing
static char **__curr_trial_name;
// True if last executed trial has failed
static int __has_failed;

/// Pass in a statement that should evaluate to true.
#define OBJECTION(statement) do {                               \
    __total_objected++;                                         \
    if(!(statement)) {                                          \
        __total_failed++;                                       \
        __has_failed = 1;                                       \
        printf("[FAIL]     OBJECTION(%s)\n", #statement);       \
        printf("[INFO]       In trial %s -> (%s, %d)\n",        \
            *__curr_trial_name, __FILE__, __LINE__);            \
    }                                                           \
} while(0)

/// Declares and defines a new TRIAL function
#define TRIAL(name)                                            \
    static void __trial_##name(void);                          \
                                                               \
    static char *__trial_name_##name                           \
    __attribute__((used, section("trial_names"))) = #name;     \
                                                               \
    static TrialFn __trial_ptr_##name                          \
    __attribute__((used, section("trials"))) = __trial_##name; \
                                                               \
    static void __trial_##name(void)

// Main function that calls all TRIALs
int main(void) {
    printf("[====] objection.h unit testing\n");
    printf("[----]\n");

    __curr_trial_name = __start_trial_names;
    __has_failed = 0;
    for(TrialFn *trial = __start_trials; trial < __stop_trials; trial++) {
        (*trial)();

        if(!__has_failed) {
            printf("[PASS] Trial %s passed\n", *__curr_trial_name);
        }
        printf("[----]\n");
        __curr_trial_name++;
        __has_failed = 0;
    }
    printf("[====] Objections: %u | Failed: %u\n", __total_objected, __total_failed);
    return 0;
}
