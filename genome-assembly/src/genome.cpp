#include "genome.h"
#include "graph.h"

namespace genome
{

    std::string assembly(size_t  k, const std::vector<std::string> reads)
    {
        if (k == 0 || reads.empty()) {
            return "";
        }
        else {
            Graph g(k, reads);//4. вместо создания указателя, граф добавляется в стек памяти
            std::string res;
            res = g.result_genome(g.find_euler_path());
            return res;
        }
    }

}