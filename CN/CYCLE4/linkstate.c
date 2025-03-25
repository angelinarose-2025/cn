#include <stdio.h>
#include <limits.h>
#define MAX_NODES 10
#define INF 9999

int find_min_distance(int dist[], int visited[], int n) {
    int min = INF, min_index;
    for (int v = 0; v < n; v++)
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int n) {
    int dist[MAX_NODES], visited[MAX_NODES], parent[MAX_NODES];
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = find_min_distance(dist, visited, n);
        visited[u] = 1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
    }

    printf("\nRouter %d Shortest Path Table:\n", src);
    printf("Dest | Cost | Path\n");
    for (int i = 0; i < n; i++) {
        printf("%4d | %4d | ", i, dist[i]);
        int j = i;
        while (j != -1) {
            printf("%d ", j);
            j = parent[j];
        }
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter number of routers: ");
    scanf("%d", &n);

    int graph[MAX_NODES][MAX_NODES];
    printf("Enter the adjacency matrix (use %d for INF):\n", INF);
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            scanf("%d", &graph[i][j]);

    for (int i = 0; i < n; i++)
        dijkstra(graph, i, n);

    return 0;
}