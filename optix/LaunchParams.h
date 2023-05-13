//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/13
//

#ifndef OPENGL_FROM_SCRATCH_LAUNCHPARAMS_H
#define OPENGL_FROM_SCRATCH_LAUNCHPARAMS_H


#include <sutil/sutil.h>
#include <sutil/vec_math.h>

namespace optix7tutorial {
struct LaunchParams
{
  uint32_t *colorBuffer;
  int2      fbSize;
};

}

#endif // OPENGL_FROM_SCRATCH_LAUNCHPARAMS_H
