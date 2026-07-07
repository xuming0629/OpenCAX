# OpenCAX
# 基于 OpenCasecade 的 CAX 读写工具


cmake -S . -B build \
  -DOpenCASCADE_DIR=/home/xuming/software/occt-7.9.2/lib/cmake/opencascade \
  -DVTK_DIR=/home/xuming/software/vtk-9.4.2/lib/cmake/vtk-9.4 \
  -DGMSH_ROOT=/home/xuming/software/gmsh-4.15

cmake --build build -j8



