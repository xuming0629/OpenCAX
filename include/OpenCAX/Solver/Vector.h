#pragma once
#include <vector>
#include <algorithm>

namespace OpenCAX {

class Vector
{
public:

    void resize(int n)
    {
        data.resize(n);
    }

    int size() const
    {
        return static_cast<int>(data.size());
    }

    void setZero()
    {
        std::fill(data.begin(),data.end(),0.0);
    }

    double& operator[](int i)
    {
        return data[i];
    }

    const double& operator[](int i) const
    {
        return data[i];
    }

    std::vector<double> data;
};

}
