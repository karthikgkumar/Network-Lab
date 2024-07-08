#include <stdio.h>
#include <limits.h>  // For INT_MAX

#define SIZE 10
#define INF INT_MAX  // Define infinity as INT_MAX

int costmat[SIZE][SIZE];
struct {
    int distance[SIZE];
    int adj[SIZE];
} node[SIZE];
int n;

void readCostMatrix() {
    int i, j, cost;
    printf("Enter cost matrix (n=%d, use -1 for no direct connection):\n", n);
    for (i = 0; i < n; i++) {
        printf("Router %d: ", (i + 1));
        for (j = 0; j < n; j++) {
            scanf("%d", &cost);
            if (i == j) {
                costmat[i][j] = 0;
            } else if (cost == -1) {
                costmat[i][j] = INF;
            } else {
                costmat[i][j] = cost;
            }
            node[i].distance[j] = costmat[i][j];
            node[i].adj[j] = j;
        }
        printf("\n");
    }
}


void calcRoutingTable() {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                if (costmat[i][k] != INF && node[k].distance[j] != INF &&
                    node[i].distance[j] > costmat[i][k] + node[k].distance[j]) {
                    node[i].distance[j] = costmat[i][k] + node[k].distance[j];
                    node[i].adj[j] = k;
                }
            }
        }
    }
}


void displayShortestRoutes() {
    int i, j;
    printf("Shortest Path - Routing Table\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (node[i].distance[j] == INF) {
                printf("From Node %d to node %d = ∞ via -\n", i, j);
            } else {
                printf("From Node %d to node %d = %d via node %d\n", i, j, node[i].distance[j], node[i].adj[j]);
            }
        }
        printf("\n");
    }
}


int main() {
    printf("Enter no of routers: ");
    scanf("%d", &n);
    readCostMatrix();
    calcRoutingTable();
    displayShortestRoutes();
    return 0;
}
