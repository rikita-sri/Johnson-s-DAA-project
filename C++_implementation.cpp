#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <climits>
#define INF INT_MAX
using namespace std;

class Graph {
    int V;
    vector<vector<int>> graph;

public:
    Graph(int V) : V(V), graph(V, vector<int>(V, 0)) {}

    void addEdge(int src, int dest, int weight) {
        graph[src][dest] = weight;
    }

    vector<int> BellmanFord(int src) {
        vector<int> dist(V, INF);
        dist[src] = 0;
        for (int i = 1; i < V; i++) {
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    if (dist[u] != INF && dist[u] + graph[u][v] < dist[v])
                        dist[v] = dist[u] + graph[u][v];
                }
            }
        }

        for (int u = 0; u < V; u++) {
            for (int v = 0; v < V; v++) {
                if (dist[u] != INF && dist[u] + graph[u][v] < dist[v])
                    return vector<int>();
            }
        }
        return dist;
    }

    vector<int> Dijkstra(int src, const vector<int>& h) {
        vector<int> dist(V, INF);
        vector<bool> visited(V, false);
        dist[src] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src});
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            visited[u] = true;
            for (int v = 0; v < V; v++) {
                if (!visited[v] && graph[u][v] != 0 && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v] + h[u] - h[v];
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    void reweightGraph(const vector<int>& h) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (graph[i][j] != 0)
                    graph[i][j] += h[i] - h[j];
            }
        }
    }

    void Johnson() {
        graph.push_back(vector<int>(V, 0));
        V++;
        for(int i=0;i<V;i++)
            graph[i][V-1]=0;
        vector<int> h = BellmanFord(V - 1);
        if (h.empty()) {
            cout << "Graph contains negative weight cycle\n";
            return;
        }
        reweightGraph(h);
        for (int i = 0; i < V - 1; i++) {
            vector<int> dist = Dijkstra(i, h);
            cout << "Shortest distances from vertex " << i << ":\n";
            for (int j = 0; j < V - 1; j++) {
                if (dist[j] == INF)
                    cout << "INF ";
                else
                    cout << dist[j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    ifstream infile("C++_input.txt");
    if (!infile) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    int V, E;
    infile >> V >> E;

    Graph graph(V);

    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        infile >> src >> dest >> weight;
        graph.addEdge(src, dest, weight);
    }

    infile.close();

    graph.Johnson();

    return 0;
}
