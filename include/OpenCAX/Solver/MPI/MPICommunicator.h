#pragma once

namespace OpenCAX {

class MPICommunicator
{
public:

    int rank() const {return rank_;}
    int size() const {return size_;}

    bool is_parallel() const
    {
        return size_>1;
    }

private:

    int rank_=0;
    int size_=1;
};

}
