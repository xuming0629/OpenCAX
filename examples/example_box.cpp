#include <OpenCAX/Geometry/ShapeFactory.h>
#include <OpenCAX/IO/OccIO.h>
#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <iostream>

int main()
{
    auto shape = OpenCAX::ShapeFactory::makeBox(1.0, 1.0, 1.0);

    OpenCAX::OccIO::writeSTEP(shape, "box.step");
    OpenCAX::OccIO::writeBREP(shape, "box.brep");

    OpenCAX::MeshingOptions options;
    options.mesh_size_min = 0.1;
    options.mesh_size_max = 0.1;
    options.optimize = true;
    options.verbose = true;

    auto mesh = OpenCAX::GmshMesher::generateVolumeMesh(shape, options);

    std::cout << "nodes: " << mesh.nodes().size() << std::endl;
    std::cout << "cells: " << mesh.cells().size() << std::endl;

    OpenCAX::VtkViewer::showMesh(mesh);

    return 0;
}