#ifndef _DDA_KINEMATICS_H
#define _DDA_KINEMATICS_H

#include "config_wrapper.h"

#include <stdint.h>

#include "dda.h"


void carthesian_to_carthesian(TARGET *startpoint, TARGET *target,
                              axes_uint32_t delta_um, axes_int32_t steps);

void carthesian_to_corexy(TARGET *startpoint, TARGET *target,
                          axes_uint32_t delta_um, axes_int32_t steps);

//void carthesian_to_scara(TARGET *startpoint, TARGET *target,
//                         axes_uint32_t delta_um, axes_int32_t steps);

static void code_axes_to_stepper_axes(TARGET *, TARGET *, axes_uint32_t,
                                      axes_int32_t)
                                      __attribute__ ((always_inline));
inline void code_axes_to_stepper_axes(TARGET *startpoint, TARGET *target,
                                      axes_uint32_t delta_um,
                                      axes_int32_t steps) {
  #if defined KINEMATICS_STRAIGHT
    carthesian_to_carthesian(startpoint, target, delta_um, steps);
  #elif defined KINEMATICS_COREXY
    carthesian_to_corexy(startpoint, target, delta_um, steps);
//  #elif defined KINEMATICS_SCARA
//    return carthesian_to_scara(startpoint, target, delta_um, steps);
  #else
    #error KINEMATICS not defined or unknown, edit your config.h.
  #endif
}

#endif /* _DDA_KINEMATICS_H */
