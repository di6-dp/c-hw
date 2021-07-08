#include "randomized_queue.h"
#include "subset.h"
#include <string>

void subset(unsigned long k, std::istream& in, std::ostream& out)
{
    randomized_queue<std::string>queue;
    std::string line;
    unsigned long t = k;
    while (k > 0 && std::getline(in, line)) {

        queue.enqueue(line);
        --k;
    }
    auto it = queue.begin();
    for (unsigned long i = 0; i < t; i++) {

        if (it == queue.end())break;
        out << *it<<'\n';
        ++it;
    }
}