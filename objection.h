/*
 * objection.h
 *
 * Version: 3.0.0-alpha
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
#define _COLOR_RED     "\x1b[31m"
#define _COLOR_GREEN   "\x1b[32m"
#define _COLOR_YELLOW  "\x1b[33m"
#define _COLOR_RESET   "\x1b[0m"

#define _STYLE_BOLD         "\033[1m"
#define _STYLE_NO_BOLD      "\033[22m"

typedef void(*TrialFn)(void);

// Main function auto called from COURT macro
static int _main(char *court_name);

// .trials data section pointers
extern TrialFn __start_trials[];
extern TrialFn __stop_trials[];

// .trial_names data section pointers
extern char *__start_trial_names[];
extern char *__stop_trial_names[];

// Count objections made and failed
static uint16_t _total_objected = 0;
static uint16_t _total_failed   = 0;

// Name of the court (test file)
static char *_court_name;
// Name of the trial currently executing
static char **_curr_trial_name;
// True if last executed trial has failed
static int _has_failed;
// Names of falied Trials for summary
#define _FAILED_TRIALS_NAME_MAX_LEN 32
#define _FAILED_TRIALS_MAX_LEN 8
static char _failed_trials
    [_FAILED_TRIALS_MAX_LEN+1]         // +1 because last element would be "and more"
    [_FAILED_TRIALS_NAME_MAX_LEN+4]    // +4 because of '\0' and "..." in case of name is too large
    = {0};
static uint8_t _failed_trials_len = 0;

// --- Exposed macros for user api ---

/// Pass in a statement that should evaluate to true.
#define OBJECTION(statement) do {                               \
    _total_objected++;                                         \
    if(!(statement)) {                                          \
        _handle_objection_failure                              \
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

/// Give name to the current court (test file). Defaults to __FILE__
#define COURT(name) \
    int main(void) { \
        if(strcmp("", #name) == 0) { \
            return _main(__FILE__); \
        } else { \
            return _main(#name); \
        } \
    }

// --- Internal use functions ---

static inline void _print_fail_prefix() {
    printf(_STYLE_BOLD "[" _COLOR_RED "FAIL" _COLOR_RESET "] " _STYLE_NO_BOLD);
}

static inline void _print_pass_prefix() {
    printf(_STYLE_BOLD "[" _COLOR_GREEN "PASS" _COLOR_RESET "] " _STYLE_NO_BOLD);
}

static inline void _print_info_prefix() {
    printf(_STYLE_BOLD "[" _COLOR_YELLOW "INFO" _COLOR_RESET "] " _STYLE_NO_BOLD);
}

// Called if the evaluation of OBJECTION resulted in false
static void _handle_objection_failure(const char *statement_str, const char *file_name, const int line) {
    _total_failed++;
    _print_fail_prefix();
    printf("    OBJECTION(%s)\n", statement_str);
    _print_info_prefix();
    printf("      In trial " _STYLE_BOLD "%s"
        _STYLE_NO_BOLD " -> (%s, %d)\n",
        *_curr_trial_name, file_name, line);

    if(_has_failed == 0) {
        _has_failed = 1;
        // Append Trial name for info display
        if(_failed_trials_len < _FAILED_TRIALS_MAX_LEN) {
            strncpy(_failed_trials[_failed_trials_len], *_curr_trial_name, _FAILED_TRIALS_NAME_MAX_LEN);
            // In case name is too large
            if(strlen(*_curr_trial_name) > _FAILED_TRIALS_NAME_MAX_LEN) {
                strcat(_failed_trials[_failed_trials_len], "...");
            }
            _failed_trials_len++;
        // In case of reached limit
        } else if(_failed_trials_len == _FAILED_TRIALS_MAX_LEN) {
            strcpy(_failed_trials[_failed_trials_len], "and more");
            _failed_trials_len++;
        }
    }
}

// Main function that calls all TRIALs
static int _main(char *court_name) {
    _court_name = court_name;
    printf("[====] Unit testing with objection.h\n");
    printf("[----] Court is: %s\n", _court_name);
    printf("[----]\n");

    // Evaluate trials
    _curr_trial_name = __start_trial_names;
    _has_failed = 0;
    for(TrialFn *trial = __start_trials; trial < __stop_trials; trial++) {
        (*trial)();

        if(!_has_failed) {
            _print_pass_prefix();
            printf("Trial " _STYLE_BOLD "%s" _STYLE_NO_BOLD " passed\n", *_curr_trial_name);
        }
        printf("[----]\n");
        _curr_trial_name++;
        _has_failed = 0;
    }
    // Print failed trials summary
    if(_failed_trials_len > 0) {
        _print_info_prefix();
        printf("Failed trials: " _STYLE_BOLD);
        for(uint8_t i = 0; i < _failed_trials_len; i++) {
            printf("%s", _failed_trials[i]);
            if(i < _failed_trials_len-1) {
                printf(_STYLE_NO_BOLD  ", " _STYLE_BOLD);
            } else {
                printf(_STYLE_NO_BOLD ".\n");
            }
        }
        printf("[----]\n");
    }

    printf("[====] " _STYLE_BOLD "Objections: %u | Failed: %u\n" _STYLE_NO_BOLD, _total_objected, _total_failed);
    return 0;
}
