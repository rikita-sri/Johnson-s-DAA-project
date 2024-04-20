#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX

struct Graph {
    int vertices;
    int **graph;
};

struct Graph *createGraph(int vertices) {
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->vertices = vertices;
    graph->graph = (int **)malloc(vertices * sizeof(int *));
    for (int i = 0; i < vertices; ++i)
        graph->graph[i] = (int *)calloc(vertices, sizeof(int));
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    graph->graph[src][dest] = weight;
}

int *BellmanFord(struct Graph *graph, int src) {
    int *dist = (int *)malloc(graph->vertices * sizeof(int));
    for (int i = 0; i < graph->vertices; ++i)
        dist[i] = INF;
    dist[src] = 0;
    for (int i = 1; i <= graph->vertices - 1; ++i) {
        for (int u = 0; u < graph->vertices; ++u) {
            for (int v = 0; v < graph->vertices; ++v) {
                if (dist[u] != INF && dist[u] + graph->graph[u][v] < dist[v])
                    dist[v] = dist[u] + graph->graph[u][v];
            }
        }
    }
    for (int u = 0; u < graph->vertices; ++u) {
        for (int v = 0; v < graph->vertices; ++v) {
            if (dist[u] != INF && dist[u] + graph->graph[u][v] < dist[v])
                return NULL;
        }
    }
    return dist;
}

int *Dijkstra(struct Graph *graph, int src, int *h) {
    int *dist = (int *)malloc(graph->vertices * sizeof(int));
    int *visited = (int *)calloc(graph->vertices, sizeof(int));

    for (int i = 0; i < graph->vertices; ++i)
        dist[i] = INF;
    dist[src] = 0;

    while (1) {
        int minDist = INF, minIndex = -1;
        for (int i = 0; i < graph->vertices; ++i) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIndex = i;
            }
        }
        if (minIndex == -1)
            break;
        visited[minIndex] = 1;
        for (int i = 0; i < graph->vertices; ++i) {
            if (!visited[i] && graph->graph[minIndex][i] != 0) {
                int newDist = dist[minIndex] + graph->graph[minIndex][i] + h[minIndex] - h[i];
                if (newDist < dist[i])
                    dist[i] = newDist;
            }
        }
    }

    free(visited);
    return dist;
}

void reweightGraph(struct Graph *graph, int *h) {
    for (int i = 0; i < graph->vertices; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            if (graph->graph[i][j] != 0)
                graph->graph[i][j] += h[i] - h[j];
        }
    }
}

void Johnson(struct Graph *graph) {
    graph->graph = (int **)realloc(graph->graph, (graph->vertices + 1) * sizeof(int *));
    graph->graph[graph->vertices] = (int *)calloc(graph->vertices, sizeof(int));
    graph->vertices++;
    int *h = BellmanFord(graph, graph->vertices - 1);
    if (h == NULL) {
        printf("Graph contains negative weight cycle\n");
        return;
    }
    reweightGraph(graph, h);
    for (int i = 0; i < graph->vertices - 1; ++i) {
        int *dist = Dijkstra(graph, i, h);
        printf("Shortest distances from vertex %d:\n", i);
        for (int j = 0; j < graph->vertices - 1; ++j) {
            if (dist[j] == INF)
                printf("INF ");
            else
                printf("%d ", dist[j]);
        }
        printf("\n");
        free(dist);
    }
    free(h);
}

int main() {
    // Read input from external file
    FILE *fp = fopen("C_input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    int vertices;
    fscanf(fp, "%d", &vertices);
    struct Graph *graph = createGraph(vertices);

    int src, dest, weight;
    while (fscanf(fp, "%d %d %d", &src, &dest, &weight) == 3)
        addEdge(graph, src, dest, weight);

    fclose(fp);

    // Perform Johnson's algorithm
    Johnson(graph);

    // Free allocated memory
    for (int i = 0; i < graph->vertices; ++i)
        free(graph->graph[i]);
    free(graph->graph);
    free(graph);

    return 0;
}
