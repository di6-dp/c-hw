#include "graph.h"

namespace genome
{
    Edge::Edge(Node from, Node to)
            : m_from(from), m_to(to)
    {
    }

    bool Edge::operator==(const Edge& other) const
    {
        return m_from == other.m_from && m_to == other.m_to;
    }

    Graph::Graph(size_t k, const std::vector<std::string>& reads)
    {
        for (const auto & read : reads)   //2. исправлено условие цикла
        {
            for (size_t j = 0; j < read.size() - k; j++)
            {
                //8. from и to так же string_view, но создание массива строк не кажется целесообразным, тк на каждой итерации необходимо только 2 подстроки read
                Node from = std::string_view(read).substr(j, k);
                Node to = std::string_view(read).substr(j + 1, k);
                Edge temp = Edge(from, to);
                if (!contains(from))
                {
                    add_node(from);
                    if (!(from == to))  //3. исправлена проверка на равенство вершин
                    {
                        edgeCounts[from] = 1;
                    }
                    else
                    {
                        edgeCounts[from] = 0;
                    }
                }
                else
                {
                    if (!(from == to)) //3. исправлена проверка на равенство вершин
                        edgeCounts[from]++;
                }
                if (!contains(to))
                {
                    add_node(to);
                    if (!(from == to))  //3. исправлена проверка на равенство вершин
                    {
                        edgeCounts[to] = -1;
                    }
                    else {
                        edgeCounts[to] = 0;
                    }
                }
                else
                {
                    if (!(from == to)) //3. исправлена проверка на равенство вершин
                        edgeCounts[to]--;
                }
                add_edge(std::move(from), std::move(temp));//7. добавлен std::move

            }
        }
    }

    bool Graph::contains(const Node& node) const
    {
        return edgeList.find(node) != edgeList.end(); // 1. убраны лишние if-else
    }

    //6. метод contains убран за ненадобностью


    //9. исправлен метод add_node, при добавлении новой вершины, добавляется пустой вектор ее ребер
    void Graph::add_node(const Node& node)
    {
        edgeList.try_emplace(node);
    }

//10. исправлен метод add_edge
    void Graph::add_edge(const Node & node, Edge edge)
    {
        if (auto it = edgeList.find(node); it != edgeList.end()) {
            it->second.push_back(std::move(edge));
        }
    }

    //11. тело цикла исправлено с использованием std::find_if
    Node Graph::find_first_node()
    {
        auto res = std::find_if(edgeCounts.begin(), edgeCounts.end(), [](const std::pair <Node, int> &x) {
            return x.second == 1;
        });
        return res != edgeCounts.end() ? res->first : 0;
    }



    //11. тело цикла исправлено с использованием std::find_if, тернарный оператор не используется для удобства
    Node Graph::find_next_node(const Node& from)
    {
        auto  res = std::find_if(edgeList[from].begin(), edgeList[from].end(), [] (const Edge & it) {
            return ! it.is_passed();});
        if   (res != edgeList[from].end())
        {
            res->mark();
            return res->to();

        }
        return std::string_view("");
    }




    std::list<Node> Graph::find_euler_path()
    {

        std::list<Node> res;
        auto start = find_first_node();

        st.push(start);
        while (!st.empty())
        {
            Node t  = find_next_node(st.top());
            if( t.compare("") == 0) {
                res.push_front(st.top());
                st.pop();
            }
            else {

                st.push(t);
            }

        }

        return res;
    }

    std::string Graph::result_genome(std::list<Node> nodes) {
        std::string res = "";
        int  k = nodes.size();
        for (int i = 0; i < k - 1; i++) {
            auto a = nodes.front();
            res.append(a.substr(0, 1));
            nodes.pop_front();

        }
        // Node b = nodes.front();
        // nodes.pop_front();
        Node c = nodes.front();
        //  res.append(b.substr(0, 1));
        res.append(c);
        return res;
    }


} // namespace genome