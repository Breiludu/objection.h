/*
 * objection.h
 *
 * Version: 2.1.1
 *
 * Copyright 2026 (c) Breixo Luna Durán
 *
 * Licensed under MIT License.
 * See the LICENSE file for details.
 * */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Colors for printing in terminal
#define __COLOR_RED     "\x1b[31m"
#define __COLOR_GREEN   "\x1b[32m"
#define __COLOR_YELLOW  "\x1b[33m"
#define __COLOR_BLUE    "\x1b[34m"
#define __COLOR_MAGENTA "\x1b[35m"
#define __COLOR_CYAN    "\x1b[36m"
#define __COLOR_RESET   "\x1b[0m"

#define __STYLE_BOLD         "\033[1m"
#define __STYLE_NO_BOLD      "\033[22m"

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
// Names of falied Trials for summary
static char __failed_trials[8][32] = {0};
static uint8_t __failed_trials_len = 0;
#define __FAILED_TRIALS_NAME_MAX_LEN 28
#define __FAILED_TRIALS_MAX_LEN 7

// --- Exposed macros for user api ---

/// Pass in a statement that should evaluate to true.
#define OBJECTION(statement) do {                               \
    __total_objected++;                                         \
    if(!(statement)) {                                          \
        __handle_objection_failure                              \
            (#statement, __FILE__, __LINE__);                   \
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

// --- Internal use functions ---

static inline void __print_fail_prefix() {
    printf(__STYLE_BOLD "[" __COLOR_RED "FAIL" __COLOR_RESET "] " __STYLE_NO_BOLD);
}

static inline void __print_pass_prefix() {
    printf(__STYLE_BOLD "[" __COLOR_GREEN "PASS" __COLOR_RESET "] " __STYLE_NO_BOLD);
}

static inline void __print_info_prefix() {
    printf(__STYLE_BOLD "[" __COLOR_YELLOW "INFO" __COLOR_RESET "] " __STYLE_NO_BOLD);
}

static void __handle_objection_failure(const char *statement_str, const char *file_name, const int line) {
    __total_failed++;
    __print_fail_prefix();
    printf("    OBJECTION(%s)\n", statement_str);
    __print_info_prefix();
    printf("      In trial " __STYLE_BOLD "%s"
        __STYLE_NO_BOLD " -> (%s, %d)\n",
        *__curr_trial_name, file_name, line);

    if(__has_failed == 0) {
        __has_failed = 1;
        // Append Trial name for info display
        if(__failed_trials_len < __FAILED_TRIALS_MAX_LEN) {
            strncpy(__failed_trials[__failed_trials_len], *__curr_trial_name, __FAILED_TRIALS_NAME_MAX_LEN);
            // In case name is too large
            if(strlen(*__curr_trial_name) > __FAILED_TRIALS_NAME_MAX_LEN) {
                strcat(__failed_trials[__failed_trials_len], "...");
            }
            __failed_trials_len++;
        // In case of reached limit
        } else if(__failed_trials_len == __FAILED_TRIALS_MAX_LEN) {
            strcpy(__failed_trials[__failed_trials_len], "and more");
            __failed_trials_len++;
        }
    }
}

// Main function that calls all TRIALs
int main(void) {
    printf("[====] objection.h unit testing\n");
    printf("[----]\n");

    __curr_trial_name = __start_trial_names;
    __has_failed = 0;
    for(TrialFn *trial = __start_trials; trial < __stop_trials; trial++) {
        (*trial)();

        if(!__has_failed) {
            __print_pass_prefix();
            printf("Trial " __STYLE_BOLD "%s" __STYLE_NO_BOLD " passed\n", *__curr_trial_name);
        }
        printf("[----]\n");
        __curr_trial_name++;
        __has_failed = 0;
    }
    // Print failed trials summary
    if(__failed_trials_len > 0) {
        __print_info_prefix();
        printf("Failed trials: " __STYLE_BOLD);
        for(uint8_t i = 0; i < __failed_trials_len; i++) {
            printf("%s", __failed_trials[i]);
            if(i < __failed_trials_len-1) {
                printf(__STYLE_NO_BOLD  ", " __STYLE_BOLD);
            } else {
                printf(__STYLE_NO_BOLD ".\n");
            }
        }
        printf("[----]\n");
    }

    printf("[====] " __STYLE_BOLD "Objections: %u | Failed: %u\n" __STYLE_NO_BOLD, __total_objected, __total_failed);
    return 0;
}
