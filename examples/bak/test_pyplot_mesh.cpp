#include <Python.h>
#include <vector>
#include <iostream>

void plot_mesh_with_python(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const std::vector<int>& edges
)
{
    // ==============================
    // 1. 初始化 Python 解释器
    // ==============================
    Py_Initialize();

    if (!Py_IsInitialized()) {
        std::cerr << "Python init failed!" << std::endl;
        return;
    }

    // ==============================
    // 2. 导入 matplotlib
    // ==============================
    PyRun_SimpleString("import matplotlib.pyplot as plt");

    // ==============================
    // 3. 传入数据到 Python
    // ==============================
    PyObject* main = PyImport_AddModule("__main__");
    PyObject* global = PyModule_GetDict(main);

    PyObject* py_x = PyList_New(x.size());
    PyObject* py_y = PyList_New(y.size());

    for (size_t i = 0; i < x.size(); ++i) {
        PyList_SetItem(py_x, i, PyFloat_FromDouble(x[i]));
        PyList_SetItem(py_y, i, PyFloat_FromDouble(y[i]));
    }

    PyDict_SetItemString(global, "x", py_x);
    PyDict_SetItemString(global, "y", py_y);

    // ==============================
    // 4. 执行 matplotlib 绘图
    // ==============================
    const char* script = R"(
import matplotlib.pyplot as plt

plt.figure()

plt.scatter(x, y, c='red')

# 画简单连线（edges: [i0,i1,i2,i3,...]）
for i in range(0, len(edges) if 'edges' in globals() else 0, 2):
    i0 = edges[i]
    i1 = edges[i+1]
    plt.plot([x[i0], x[i1]], [y[i0], y[i1]], 'b-')

plt.title("OpenCAX Mesh")
plt.axis("equal")
plt.show()
)";

    PyRun_SimpleString(script);

    // ==============================
    // 5. 清理
    // ==============================
    Py_Finalize();
}


#include <vector>



int main()
{
    // 简单 2x2 mesh
    std::vector<double> x = {
        0, 1, 2,
        0, 1, 2,
        0, 1, 2
    };

    std::vector<double> y = {
        0, 0, 0,
        1, 1, 1,
        2, 2, 2
    };

    std::vector<int> edges = {
        0,1, 1,2,
        3,4, 4,5
    };

    plot_mesh_with_python(x, y, edges);

    return 0;
}