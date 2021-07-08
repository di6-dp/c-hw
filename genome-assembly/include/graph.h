#pragma once

#include <list>
#include <string_view>
#include <stack>
#include <string>
#include<unordered_map>
#include<map>
#include <vector>
#include <algorithm>

namespace genome
{

    //8. Node заменен обратно на string_view
    using Node = std::string_view ;

    class Edge
    {
    public:
        Edge(Node from, Node to);

        const auto& from() const { return m_from; }
        const auto& to() const { return m_to; }
        const auto& is_passed() const { return m_passed; }
        void mark() { m_passed = true; }

        bool operator==(const Edge& other) const;



    private:
        Node m_from;
        Node m_to;
        bool m_passed = false;

    };

    class Graph
    {
    public:
        Graph() = default;
        Graph(size_t, const std::vector<std::string>&);

        void add_edge(const Node&, Edge);
        void add_node(const Node&);


        bool contains(const Node&) const;


        std::list<Node> find_euler_path();
        Node find_first_node();
        Node find_next_node(const Node&);

        std::string result_genome(std::list<Node>);

    private:
        //8. убраны неиспользуемые поля
        std::map <Node, std::vector<Edge>> edgeList;
        std::stack<Node> st;
        std::map <Node, int> edgeCounts;
    };



} // namespace genome