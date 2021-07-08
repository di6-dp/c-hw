#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <iterator>
#include <iostream>
#include <utility>
#include "searcher.h"

std::string trim(std::string str)//4.во избежание лишнего копирования  строка передается по ссылке
{
    std::string p;
    std::size_t i;
    std::size_t j;
    if (str.empty()) return  str;
    for (i = 0; std::ispunct(str[i]) ; i++);
    for (j = str.length() - 1; std::ispunct(str[j]); j--);
    std::string c = str.substr(i, j - i + 1);
    return c;
}

void Searcher::add_document(const Filename& filename, std::istream& strm)
{
    std::size_t index = 0;
    std::string str, word;
    strm.ignore(std::numeric_limits<std::size_t>::max(), '\n');
    std::string line;
    while(strm >> line){
        std::stringstream ss(line);
        while (ss >> word)
        {
            auto t = trim(word);
            if (!word.empty())
            {
                m_dictionary[t][filename].push_back(index++);
            }
        }
    }

    auto it = m_file_list.find(filename);
    if (it == m_file_list.end())
    {
        m_file_list[filename] = m_dictionary.end();
    }
}

void Searcher::remove_document(const Filename & filename)
{
    if (m_file_list.find(filename) != m_file_list.end()){
        auto it = m_dictionary.begin();
        std::string word;
        while (it != m_file_list[filename] && it!= m_dictionary.end()){
            if (it->second.find(filename)!= it->second.end()){
                it->second.erase(filename);
            }
            if (it->second.empty()){
                word = it->first;
                it++;
                m_dictionary.erase(word);
            } else {
                it++;
            }
        }
        m_file_list.erase(filename);
    }
}

Searcher::DocIterator::reference Searcher::DocIterator::operator*() const
{
    return find_list[index];
}

Searcher::DocIterator& Searcher::DocIterator::operator++()
{
    index++;
    return *this;
}

Searcher::DocIterator Searcher::DocIterator::operator++(int)
{
    auto tmp = *this;
    operator++();
    return tmp;
}

bool Searcher::DocIterator::operator==( const DocIterator& other) const {
    return (find_list == other.find_list) && (index == other.index);
}

bool Searcher::DocIterator::operator!=(const DocIterator& other) const {
    return !(*this == other);
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (!token.empty() )
            tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::size_t>  Searcher::get_index(const Filename & filename, const std::string& str) const
{
    if ( (m_dictionary.find(str) != m_dictionary.end()) && (m_dictionary.at(str).find(filename) != m_dictionary.at(str).end())){
        return m_dictionary.at(str).at(filename);
    }
    return std::vector<std::size_t>();

}

bool Searcher::search_query(const Filename& filename,const std::vector<std::string>& query, const std::vector<std::size_t>& v1, std::size_t i) const//8.Исправлена сигнатура метода 
{
    if (v1.empty()) return false;

    if (i == 0 && i == query.size() - 1)
    {
        return !v1.empty();
    }

    if (i == 0 && i != query.size() -1 )
    {
        return search_query(filename, query, v1,1);
    }

    std::string str = query[i];
    std::vector<std::size_t> v2 = get_index(filename, str);
    std::vector<std::size_t> v3;
    for (auto x : v1)
    {
        for (auto y : v2)
        {
            if (y == x + 1)
            {
                v3.push_back(y);
            }
        }
    }

    if (i < query.size() -1 )
    {
        return search_query(filename, query,  v3, i + 1);
    }

    if (i == query.size() -1 )
    {
        return (!v3.empty());
    }
    return false;
}

void add_str(std::vector<std::vector<std::string>>& sequences, std::string::const_iterator lit, std::string::const_iterator rit){
    auto temp = split(std::string(lit, rit),' ' );
    for (auto& word: temp){
        word = trim(word);
    }
    temp.erase(std::remove(temp.begin(), temp.end(), ""), temp.end());
    if(!temp.empty()) sequences.push_back(temp);
}


std::pair<Searcher::DocIterator, Searcher::DocIterator> Searcher::search(const std::string & query) const {//5.Слово приводится к требованиям запроса при разборе запроса, удалено двойное приведение

    if (query.empty()){
        throw BadQuery("Incorrect input: request is empty");//6.Исправлено сообщение об ошибке
    }

    std::vector<std::vector<std::string>> sequences;
    std::vector<std::string> temp;
    bool is_sequence = false;
    bool is_word = false;
    auto lit = query.begin();
    auto it = query.begin();
    while (it < query.end()){
        if (!is_sequence){
            if (! is_word){
                switch (*it){
                    case ' ':
                        it++;
                        break;
                    case '\"':
                        is_sequence = true;
                        lit = it;
                        it++;
                        break;
                    default:
                        is_word = true;
                        lit = it;
                        it++;
                        break;
                }
            } else {
                switch (*it){
                    case ' ':
                        is_word = false;
                        add_str(sequences, lit, it);
                        it++;
                        break;
                    case '\"':
                        is_word = false;
                        is_sequence = true;
                        add_str(sequences, lit, it);
                        lit = it;
                        it++;
                        break;
                    default:
                        it++;
                        break;
                }
            }
        } else {
            switch (*it){
                case '\"':
                    is_sequence = false;
                    add_str(sequences, lit, it);
                    it++;
                    lit = it;
                    break;
                default:
                    it++;
                    break;
            }
        }
    }


    if (is_word)
    {
        add_str(sequences, lit, it);
    };
    if (is_sequence)
    {
        throw BadQuery("Incorrect input: Wrong number of '\"' ");//6. Исправлено сообщение об ошибке
    }
    if (sequences.empty() ) throw BadQuery ("Incorrect input: strings query does not meet the requirements of request"); //6. Исправлено сообщение об ошибке
    std::unordered_map<Filename, bool> res;
    std::vector <Filename> pos;
    for (const auto & file_it : m_file_list) {
        res[file_it.first] = true;
    }

    for (const auto &sequence : sequences) {
        auto posting_lists = m_dictionary.find(sequence[0]);
        if (posting_lists == m_dictionary.end()){
            return std::pair<Searcher::DocIterator, Searcher::DocIterator>(DocIterator(pos, 0), DocIterator(pos, 0));
        }
        for (const auto & fit : m_file_list) {
            //7.По ошибке была оставлена старая неиспользующаяся переменная, она удалена
            if (res[fit.first]) {
                res[fit.first] &= search_query(fit.first, sequence, get_index(fit.first, sequence[0]), 0);

            }
        }
    }
    for (const auto& [path, is_found] : res)//3. x заменено на [path, is_found] чтобы избежать копирования
    {
        if (is_found)
        {
            pos.push_back(path);
        }
    }

    std::size_t  c = pos.size();
    auto x = DocIterator( pos, 0);
    auto y = DocIterator( pos, c);
    auto a =  std::pair<Searcher::DocIterator, Searcher::DocIterator>(x, y);

    return a;
}