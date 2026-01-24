/* objection.h - v1.0 - (c) Breixo Luna Durán */

#include <stdio.h>
#include <stdint.h>

// Sentence > Trial > Objection
static uint16_t _total_objected = 0;
static uint16_t _total_failed   = 0;

static uint16_t _trial_objected = 0;
static uint16_t _trial_failed   = 0;

/// Pass in a statement that should evaluate to true.
#define OBJECTION(statement) do { \
    _total_objected++; \
    _trial_objected++; \
    if(!(statement)) { \
        _total_failed++; \
        _trial_failed++; \
        printf("\t**FAILED: %s\n", #statement); \
    } \
} while(0)

/// Pass in a function with OBJECTIONs inside
#define TRIAL(func) do { \
    _trial_objected = 0; \
    _trial_failed   = 0; \
    printf("TRIAL %s:\n", #func); \
    func(); \
    printf("\tobjections: %u\n", _trial_objected); \
    printf("\tfailed: %u\n\n", _trial_failed); \
} while(0)

/// Pass in a function with TRIALs inside
#define SENTENCE(func) do { \
    _total_objected = 0; \
    _total_failed   = 0; \
    printf(">>> SENTENCE of %s <<<\n\n", #func); \
    func(); \
    printf("total objections: %u\n", _total_objected); \
    printf("total failed: %u\n\n", _total_failed); \
    printf(">>> END of %s <<<\n\n", #func); \
} while(0)

/* What? Expected something better? My apologies. No, I mean it. */
