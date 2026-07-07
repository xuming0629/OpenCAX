#include <OpenCAX/Post/ConvergencePlotter.h>

#include <fstream>

namespace OpenCAX
{

bool ConvergencePlotter::writePythonScript(
    const std::string& csv_file,
    const std::string& py_file,
    const std::string& png_file
)
{
    std::ofstream fout(py_file);

    if (!fout.is_open())
    {
        return false;
    }

    fout << R"(import pandas as pd
import matplotlib.pyplot as plt

csv_file = r")" << csv_file << R"("
png_file = r")" << png_file << R"("

df = pd.read_csv(csv_file)

plt.figure(figsize=(8, 6))

plt.loglog(df["h"], df["l2"], "o-", label="discrete l2")
plt.loglog(df["h"], df["L2"], "s-", label="L2")
plt.loglog(df["h"], df["H1Semi"], "^-", label="H1 semi")
plt.loglog(df["h"], df["H1"], "d-", label="H1")

plt.gca().invert_xaxis()
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.xlabel("h")
plt.ylabel("error")
plt.title("OpenCAX Poisson2D Convergence")
plt.legend()
plt.tight_layout()
plt.savefig(png_file, dpi=300)
plt.show()
)";

    return true;
}

} // namespace OpenCAX