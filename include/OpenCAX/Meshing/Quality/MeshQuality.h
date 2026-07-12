#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <vector>

namespace OpenCAX
{

struct MeshQualitySummary
{
    bool valid = false;
    double min_measure = 0.0;
    double max_measure = 0.0;
    double avg_measure = 0.0;
    int bad_cell_count = 0;
};

class OpenCAX_API MeshQuality
{
public:
    static double cell_measure(const Mesh& mesh, int cell_id);
    static double triangle_area(const Mesh& mesh, int cell_id);
    static double tetra_volume(const Mesh& mesh, int cell_id);
    static double hexa_volume(const Mesh& mesh, int cell_id);
    static MeshQualitySummary summarize(const Mesh& mesh, double eps = 1.0e-14);
};

} // namespace OpenCAX
