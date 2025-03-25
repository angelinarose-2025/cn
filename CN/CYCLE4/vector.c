#include <stdio.h>
#define INF 9999
#define MAX_NODES 10

struct Node {
    int distance[MAX_NODES];
    int next_hop[MAX_NODES];
} routing_table[MAX_NODES];

void distance_vector(int cost[MAX_NODES][MAX_NODES], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            routing_table[i].distance[j] = cost[i][j];
            routing_table[i].next_hop[j] = (cost[i][j] != INF && i != j) ? j : -1;
        }
    }

    int updated;
    do {
        updated = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    if (routing_table[i].distance[k] > cost[i][j] + routing_table[j].distance[k]) {
                        routing_table[i].distance[k] = cost[i][j] + routing_table[j].distance[k];
                        routing_table[i].next_hop[k] = j;
                        updated = 1;
                    }
                }
            }
        }
    } while (updated);
}

void print_routing_table(int n) {
    for (int i = 0; i < n; i++) {
        printf("\nRouting table for Router %d:\n", i);
        printf("Destination | Next Hop | Distance\n");
        for (int j = 0; j < n; j++) {
            printf("%11d | %8d | %8d\n", j, routing_table[i].next_hop[j], routing_table[i].distance[j]);
        }
    }
}

int main() {
    int n;
    printf("Enter the number of routers: ");
    scanf("%d", &n);

    int cost[MAX_NODES][MAX_NODES];
    printf("Enter the cost adjacency matrix (use %d for INF):\n", INF);
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            scanf("%d", &cost[i][j]);

    distance_vector(cost, n);
    print_routing_table(n);

    return 0;
}