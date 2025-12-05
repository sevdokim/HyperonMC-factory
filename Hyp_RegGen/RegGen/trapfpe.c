//-----------------------------------------------------------------------------
// Enable floatint point exceptions:
//        division by zero
//        result not representable due to underflow
//        result not representable due to overflow
//        invalid operation
// Yuri Kharlov. 18.07.2017
//-----------------------------------------------------------------------------

#define _GNU_SOURCE
#include <fenv.h>

void trapfpe_ () {
//  feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW);
  feenableexcept(FE_DIVBYZERO  | FE_INVALID | FE_UNDERFLOW);
}
