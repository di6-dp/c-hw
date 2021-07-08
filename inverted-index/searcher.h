#pragma once

#include <string>
#include <utility>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <set>
#include <map>

class Searcher
{
    //1. Заменены названия переменных, std::string заменен на Filename в необходимых местах
    using word_list =  std::map<std::string, std::map<std::string, std::vector<size_t>>>;
    word_list m_dictionary;
    std::unordered_map<std::string, word_list::iterator> m_file_list;
public:
    using Filename = std::string;

    void add_document(const Filename & filename, std::istream & strm);

    void remove_document(const Filename & filename);


    class DocIterator
    {
        std::vector<Filename> find_list;
        size_t index;

    public:
        using value_type = const Filename;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator_category = std::forward_iterator_tag;

        explicit DocIterator( std::vector<std::string> positions, size_t pos)
                :   find_list(std::move(positions)), index(pos)
        {}

        DocIterator &operator++();


        DocIterator operator++(int);


        reference operator*() const;


        bool operator==(const DocIterator& ) const ;

        bool operator!=(const DocIterator& ) const;

    };


    class BadQuery: public std::exception
    {
    private:
        std::string m_error;

    public:
        explicit BadQuery(std::string error)
                : m_error(std::move(error))
        {
        }

        [[nodiscard]] const char* what() const noexcept override { return m_error.c_str(); }
    };

    std::vector<size_t> get_index(const Filename& , const std::string& ) const;

    bool search_query(const Filename& filename,const std::vector<std::string>& query,  const std::vector<size_t>& v1,  size_t i)  const;

    std::pair<DocIterator, DocIterator> search(const std::string & query) const;
};