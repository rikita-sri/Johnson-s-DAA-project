from queue import PriorityQueue
import sys

INF = float('inf')

class Graph:
    def __init__(self, vertices):
        self.V = vertices
        self.graph = [[0 for _ in range(vertices)] for _ in range(vertices)]

    def add_edge(self, src, dest, weight):
        self.graph[src][dest] = weight

    def bellman_ford(self, src):
        dist = [INF] * self.V
        dist[src] = 0
    
        for i in range(1, self.V):
            for u in range(self.V):
                for v in range(self.V):
                    if dist[u] != INF and dist[u] + self.graph[u][v] < dist[v]:
                        dist[v] = dist[u] + self.graph[u][v]
    
        for u in range(self.V):
            for v in range(self.V):
                if dist[u] != INF and dist[u] + self.graph[u][v] < dist[v]:
                    return None  # Negative cycle detected
    
        return dist

    def dijkstra(self, src, h):
        dist = [INF] * self.V
        visited = [False] * self.V
        dist[src] = 0
        pq = PriorityQueue()
        pq.put((0, src))

        while not pq.empty():
            cost, u = pq.get()
            visited[u] = True

            for v in range(self.V):
                if not visited[v] and self.graph[u][v] != 0 and dist[u] != INF and dist[u]+self.graph[u][v]<dist[v]:
                    dist[v] = dist[u] + self.graph[u][v] + h[u] - h[v]
                    pq.put((dist[v], v))
        return dist

    def reweight_graph(self, h):
        for i in range(self.V):
            for j in range(self.V):
                if self.graph[i][j] != 0:
                    self.graph[i][j] += h[i] - h[j]

    def johnson(self, filename):
        with open(filename, 'r') as f:
            lines = f.readlines()
            V, E = map(int, lines[0].split())
            for line in lines[1:]:
                src, dest, weight = map(int, line.split())
                self.add_edge(src, dest, weight)

        # Add a new vertex with edges of weight 0 to all other vertices
        for i in range(self.V):
            self.graph[i].append(0)
        self.graph.append([0] * (self.V + 1))
        self.V += 1

        # Run Bellman-Ford algorithm to get reweighting values
        h = self.bellman_ford(self.V - 1)
        if h is None:
            print("Graph contains negative weight cycle")
            return

        # Reweight the graph
        self.reweight_graph(h)

        # Remove the last row and column added for the extra vertex
        self.V -= 1
        self.graph.pop()
        for i in range(self.V):
            self.graph[i].pop()

        # Run Dijkstra's algorithm for each vertex
        for src in range(self.V):
            dist = self.dijkstra(src, h)
            print(f"Shortest distances from vertex {src}:")
            for i in range(self.V):
                if dist[i] == INF:
                    print("INF", end=" ")
                else:
                    print(dist[i], end=" ")
            print()

# Example usage:
if len(sys.argv) != 2:
    print("Usage: python program.py input_file")
    sys.exit(1)

filename = sys.argv[1]
graph = Graph(4)
graph.johnson(filename)
