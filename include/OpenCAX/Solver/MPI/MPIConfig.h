#pragma once
namespace OpenCAX {

struct MPIConfig
{
    int rank=0;
    int size=1;
    bool initialized=false;
};

}
