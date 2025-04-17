#include <set>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cassert>

using namespace std;

struct Edge {
    int weight{};
    int vertex_a{};
    int vertex_b{};

    Edge(int w, int a, int b) : weight(w), vertex_a(std::min(a, b)), vertex_b(std::max(a, b)) {}

    Edge() = default;
};

bool operator<(const Edge &l, const Edge &r) {
    if (l.weight == r.weight) {
        if (l.vertex_a == r.vertex_a) {
            return l.vertex_b < r.vertex_b;
        } else {
            return l.vertex_a < r.vertex_a;
        }
    } else {
        return l.weight < r.weight;
    }
}

struct UnionSet {
    std::unordered_map<int, int> parent;
    std::unordered_map<int, int> rank;

    void insert(int x) {
        if (not find(x)) {
            parent[x] = x;
            rank[x] = 0;
        }
    }

    optional<int> find(int x) {
        auto it = parent.find(x);
        if (it == parent.end()) {
            return {};
        }
        if (it->second == x) {
            return x;
        }
        it->second = find(it->second).value();
        return it->second;
    }

    void unite(int x, int y) {
        auto parent_x = find(x);
        auto parent_y = find(y);
        if (not parent_x or not parent_y) {
            return;
        }
        if (parent_x == parent_y) {
            return;
        }
        if(rank[parent_x.value()] < rank[parent_y.value()]){
            std::swap(parent_x, parent_y);
        }
        parent[parent_y.value()] = parent_x.value();
        if(rank[parent_x.value()] == rank[parent_y.value()]){
            rank[parent_x.value()]++;
        }
    }
};

vector<vector<vector<int>>> kruskalsAlgorithm(const vector<vector<vector<int>>> &edges) {
    std::set<Edge> graph;
    for (auto a = 0; a < edges.size(); a++) {
        for (const auto &b_and_weight: edges[a]) {
            Edge e(b_and_weight[1], a, b_and_weight[0]);
            graph.insert(e);
        }
    }
    std::set<Edge> spanning_forest;
    UnionSet connected_edges;
    for (const auto &edge: graph) {
        auto parent_a = connected_edges.find(edge.vertex_a);
        auto parent_b = connected_edges.find(edge.vertex_b);
        if (not parent_a or not parent_b or connected_edges.find(edge.vertex_a) != connected_edges.find(edge.vertex_b)) {
            connected_edges.insert(edge.vertex_a);
            connected_edges.insert(edge.vertex_b);
            connected_edges.unite(edge.vertex_a, edge.vertex_b);
            spanning_forest.insert(edge);
        }
    }
    vector<vector<vector<int>>> result;
    for (const auto &edge: spanning_forest) {
        result.resize(max(result.size(), static_cast<size_t>(edge.vertex_a + 1)));
        result[edge.vertex_a].push_back(std::vector<int>{edge.vertex_b, edge.weight});
        result.resize(max(result.size(), static_cast<size_t>(edge.vertex_b + 1)));
        result[edge.vertex_b].push_back(std::vector<int>{edge.vertex_a, edge.weight});
    }
    return result;
}

int main() {
    {
        {
            vector<vector<vector<int>>> graph{
                    {
                            {1, 3},
                            {2, 5}
                    },
                    {
                            {0, 3},
                            {2, 10},
                            {3, 12}
                    },
                    {
                            {0, 5},
                            {1, 10}
                    },
                    {
                            {1, 12}
                    }
            };
            vector<vector<vector<int>>> expected = {
                    {
                            {1, 3},
                            {2, 5}
                    },
                    {
                            {0, 3},
                            {3, 12}
                    },
                    {
                            {0, 5},
                    },
                    {
                            {1, 12}
                    }
            };
            vector<vector<vector<int>>> actual = kruskalsAlgorithm(graph);
            assert(expected == actual);
        }
    }
}