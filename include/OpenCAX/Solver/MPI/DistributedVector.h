#pragma once
#include <OpenCAX/Solver/Vector.h>

namespace OpenCAX {

class DistributedVector
{
public:

    Vector local;

    int global_size=0;

    int offset=0;
};

}
