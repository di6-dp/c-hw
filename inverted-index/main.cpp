#include "searcher.h"

#include <fstream>
#include <iostream>
#include <iterator>

int main(int argc, char ** argv)
{
    Searcher s;

    std::ifstream simple_file_stream("test/etc/simple_file.txt");
    s.add_document("test/etc/simple_file.txt",simple_file_stream );
    for (int i = 1; i < argc; ++i) {
        std::ifstream f(argv[i]);
        s.add_document(argv[i], f);
    }


    const auto [begin, end] = s.search("engine");
    std::ostream_iterator<Searcher::Filename> out(std::cout, ", ");
    std::copy(begin, end, out);
    std::cout << std::endl;

    return 0;
}