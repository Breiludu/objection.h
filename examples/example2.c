#include "../objection.h"

COURT(example2);

float mid_point(float a, float b) {
    return (a + b)/2.0f;
}

TRIAL(mid_point_trial1) {
    OBJECTION(mid_point(2, 6) == 4);
    OBJECTION(mid_point(0, 0) == 0);
    OBJECTION(mid_point(2, 2) == 2);
}

TRIAL(mid_point_trial2) {
    float a = 10;
    float b = 20;
    float m = mid_point(a, b);

    OBJECTION(mid_point(10*a, 10*b) == 10*m);
}
