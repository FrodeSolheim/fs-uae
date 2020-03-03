#define FSEMU_INTERNAL
#include "fsemu-util.h"

#include "fsemu-glib.h"
#include "fsemu-time.h"

const char *fsemu_read_env_option(const char *name)
{
    gchar *key = g_strdup_printf("FSEMU_%s", name);
    char *c = key;
    while (*c) {
        *c = g_ascii_toupper(*c);
        c++;
    }
    const char *value = getenv(key);
    g_free(key);
    if (value != NULL) {
        return value;
    }
    /*
    key = g_strdup_printf("FSGS_%s", name);
    value = getenv(key);
    g_free(key);
    if (value != NULL) {
        return value;
    }
    */
    return "";
}

const char *fsemu_getenv(const char *name)
{
    const char *value = getenv(name);
    if (value != NULL) {
        return value;
    }
    return "";
}

// ----------------------------------------------------------------------------

// Same spring calculation and initial values as used by react-spring.

void fsemu_util_spring_init(fsemu_util_spring_t *spring)
{
    spring->current = 0.0;
    // FIXME: Consider including a spring->icurrent (double value rounded
    // to nearest integer), but only done once per current update. This would
    // allow clients to not having to convert to int every time current is
    // checked.
    spring->friction = 26.0;
    spring->last_us = 0;
    spring->mass = 1.0;
    spring->target = 0.0;
    spring->stopped = false;
    spring->tension = 170.0;
    spring->velocity = 0.0;
}

// void fsemu_util_spring_init_with_params(fsemu_util_spring_t *spring, double
// ...);

void fsemu_util_spring_update(fsemu_util_spring_t *spring)
{
    fsemu_util_spring_update_with_time(spring, fsemu_time_us());
}

void fsemu_util_spring_update_with_time(fsemu_util_spring_t *spring,
                                        int64_t time_us)
{
    if (spring->stopped) {
        return;
    }
    if (time_us - spring->last_us > 30000) {
        spring->last_us = time_us;
    }
    int numsteps = (time_us - spring->last_us) / 1000;
    for (int i = 0; i < numsteps; i++) {
        double force = -spring->tension * (spring->current - spring->target);
        double damping = -spring->friction * spring->velocity;
        double acceleration = (force + damping) / spring->mass;
        spring->velocity = spring->velocity + acceleration / 1000;
        spring->current = spring->current + spring->velocity / 1000;
    }

    spring->last_us = spring->last_us + numsteps * 1000;
    // printf("%d %d\n", (int) spring->current, (int) spring->target);

    // FIXME: Temporary
    double precision = 1.0;

    bool clamp = true;
    bool overshooting = clamp && spring->tension != 0
                            ? spring->from < spring->target
                                  ? spring->current > spring->target
                                  : spring->current < spring->target
                            : false;
    bool velocity_target = fabs(spring->velocity) <= precision;
    bool displacement_target =
        spring->tension != 0
            ? fabs(spring->target - spring->current) <= precision
            : true;
#if 0
    if (abs(spring->from - spring->target) < 100) {
        printf("%d %d velocity %0.1f displacement %0.1f overshooting %d\n",
               (int) spring->current,
               (int) spring->target,
               fabs(spring->velocity),
               fabs(spring->target - spring->current),
               overshooting);
    }
#endif
    if (overshooting || (velocity_target && displacement_target)) {
        fsemu_util_spring_finish(spring);
    }
}

void fsemu_util_spring_finish(fsemu_util_spring_t *spring)
{
    spring->stopped = true;
    spring->current = spring->target;
    spring->velocity = 0.0;
    spring->last_us = 0;
}

void fsemu_util_spring_set_target(fsemu_util_spring_t *spring, double target)
{
    if (spring->target == target) {
        return;
    }
    spring->from = spring->current;
    spring->target = target;
    spring->stopped = false;
}

void fsemu_util_spring_set_tension(fsemu_util_spring_t *spring, double tension)
{
    spring->tension = tension;
}

// ----------------------------------------------------------------------------
