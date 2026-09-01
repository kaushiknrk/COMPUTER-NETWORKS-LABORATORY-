#include <stdio.h>
#define MAX 10
#define INF 999

void displayMatrix(int n, int graph[MAX][MAX]) {
    int i, j;
    printf("\n--- CURRENT BIDIRECTIONAL NETWORK MAP ---\n\t");
    for (i = 0; i < n; i++) printf("%c\t", 'A' + i);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c\t", 'A' + i);
        for (j = 0; j < n; j++) {
            if (graph[i][j] == INF) printf("INF\t");
            else printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
}

/* Helper function to display final converged matrix */
void displayFinalCostMatrix(int n, int distance[MAX][MAX]) {
    int i, j;
    printf("\n========================================\n");
    printf("         FINAL CONVERGED COST MATRIX\n");
    printf("========================================\n\t");
    for (i = 0; i < n; i++) printf("%c\t", 'A' + i);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c\t", 'A' + i);
        for (j = 0; j < n; j++) {
            if (distance[i][j] == INF) printf("INF\t");
            else printf("%d\t", distance[i][j]);
        }
        printf("\n");
    }
}

void distanceVector(int n, int cost[MAX][MAX]) {
    int distance[MAX][MAX], nextHop[MAX][MAX];
    int i, j, k, updated, iteration = 0;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            distance[i][j] = cost[i][j];
            if (cost[i][j] != INF && i != j) nextHop[i][j] = j;
            else nextHop[i][j] = -1;
        }
    }

    do {
        updated = 0;
        iteration++;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    if (distance[i][k] != INF && distance[k][j] != INF && nextHop[i][k] != -1) {
                        if (distance[i][k] + distance[k][j] < distance[i][j]) {
                            distance[i][j] = distance[i][k] + distance[k][j];
                            nextHop[i][j] = nextHop[i][k];
                            updated = 1;
                        }
                    }
                }
            }
        }
    } while (updated);

    printf("\n*** RIP DISTANCE VECTOR RESULTS ***\nTotal convergence rounds taken: %d\n", iteration);
    for (i = 0; i < n; i++) {
        printf("\n[ RIP TABLE FOR ROUTER %c ]\nEnd Router\tNext Step\tTotal Cost\n-----------------------------------\n", 'A' + i);
        for (j = 0; j < n; j++) {
            printf("%c\t\t", 'A' + j);
            if (i == j) printf("-\t\t0\n");
            else if (distance[i][j] == INF) printf("-\t\tINF\n");
            else printf("%c\t\t%d\n", 'A' + nextHop[i][j], distance[i][j]);
        }
    }
    displayFinalCostMatrix(n, distance);
}

int findMinimum(int distance[MAX], int visited[MAX], int n) {
    int min = INF, index = -1, i;
    for (i = 0; i < n; i++) {
        if (!visited[i] && distance[i] < min) {
            min = distance[i];
            index = i;
        }
    }
    return index;
}

void displayPath(int parent[MAX], int destination) {
    if (parent[destination] == -1) {
        printf("%c", 'A' + destination);
        return;
    }
    displayPath(parent, parent[destination]);
    printf(" > %c", 'A' + destination);
}

void linkState(int n, int graph[MAX][MAX]) {
    int distance[MAX], visited[MAX], parent[MAX];
    int finalMatrix[MAX][MAX];
    int source, i, j, count, current;
    char srcChar;

    printf("\nEnter starting OSPF router (A-%c): ", 'A' + n - 1);
    scanf(" %c", &srcChar);
    source = srcChar - 'A';

    if (source < 0 || source >= n) {
        printf("\nInvalid choice!\n");
        return;
    }

    /* Pre-populate matrix tracking for display layout */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) finalMatrix[i][j] = 0;
            else finalMatrix[i][j] = INF;
        }
    }

    for (i = 0; i < n; i++) {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    distance[source] = 0;

    for (count = 0; count < n - 1; count++) {
        current = findMinimum(distance, visited, n);
        if (current == -1) break;
        visited[current] = 1;
        for (i = 0; i < n; i++) {
            if (!visited[i] && graph[current][i] != INF && distance[current] != INF) {
                int newDistance = distance[current] + graph[current][i];
                if (newDistance < distance[i]) {
                    distance[i] = newDistance;
                    parent[i] = current;
                }
            }
        }
    }
    printf("\n*** OSPF LINK STATE RESULTS ***\nOSPF Root Source: %c\n\nEnd Router\tOSPF Cost\tFull SPF Route\n-----------------------------------\n", 'A' + source);
    for (i = 0; i < n; i++) {
        printf("%c\t\t", 'A' + i);
        if (distance[i] == INF) printf("INF\tNo Route Found\n");
        else {
            printf("%d\t\t", distance[i]);
            displayPath(parent, i);
            printf("\n");
        }
        finalMatrix[source][i] = distance[i];
    }

    /* Populate remaining matrix row metrics temporarily for full map view parity */
    for (i = 0; i < n; i++) {
        if (i == source) continue;
        int tempDist[MAX], tempVis[MAX];
        for (j = 0; j < n; j++) { tempDist[j] = INF; tempVis[j] = 0; }
        tempDist[i] = 0;
        for (count = 0; count < n - 1; count++) {
            int curr = findMinimum(tempDist, tempVis, n);
            if (curr == -1) break;
            tempVis[curr] = 1;
            for (j = 0; j < n; j++) {
                if (!tempVis[j] && graph[curr][j] != INF && tempDist[curr] != INF) {
                    int nextD = tempDist[curr] + graph[curr][j];
                    if (nextD < tempDist[j]) tempDist[j] = nextD;
                }
            }
        }
        for (j = 0; j < n; j++) finalMatrix[i][j] = tempDist[j];
    }
    displayFinalCostMatrix(n, finalMatrix);
}

void findSpecificShortestPath(int n, int graph[MAX][MAX]) {
    char srcChar, destChar;
    int source, dest;
    int distance[MAX], visited[MAX], parent[MAX];
    int i, count, current;

    printf("\nEnter starting router (A-%c): ", 'A' + n - 1);
    scanf(" %c", &srcChar);
    printf("Enter destination router (A-%c): ", 'A' + n - 1);
    scanf(" %c", &destChar);

    source = srcChar - 'A';
    dest = destChar - 'A';

    if (source < 0 || source >= n || dest < 0 || dest >= n) {
        printf("\nInvalid router options chosen!\n");
        return;
    }

    for (i = 0; i < n; i++) {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    distance[source] = 0;

    for (count = 0; count < n - 1; count++) {
        current = findMinimum(distance, visited, n);
        if (current == -1) break;
        visited[current] = 1;

        for (i = 0; i < n; i++) {
            if (!visited[i] && graph[current][i] != INF && distance[current] != INF) {
                int newDistance = distance[current] + graph[current][i];
                if (newDistance < distance[i]) {
                    distance[i] = newDistance;
                    parent[i] = current;
                }
            }
        }
    }

    printf("\n=== MINIMUM COST PATH ROUTE ===\n");
    if (distance[dest] == INF) {
        printf("No path exists between %c and %c.\n", srcChar, destChar);
    } else {
        printf("Minimum Total Path Cost: %d\n", distance[dest]);
        printf("Shortest Structural Route: ");
        displayPath(parent, dest);
        printf("\n");
    }
}

int main() {
    int n = 0, graph[MAX][MAX];
    int i, j, choice, newCost, isConfigured = 0;
    char modSrcChar, modDestChar;
    int modSrc, modDest;

    while (1) {
        printf("\n=== CHOOSE AN ALGORITHM ===\n");
        printf("1. RIP (Distance Vector Protocol)\n");
        printf("2. OSPF (Link State Protocol)\n");
        printf("3. Run Both Protocols\n");
        printf("4. Modify Shared Link Cost\n");
        printf("5. Find Minimum Cost Path for a Pair\n");
        printf("6. Exit\n\nYour selection: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\nWrong option choice!\n");
            continue;
        }
        if (choice == 6) {
            printf("\nGoodbye.\n");
            break;
        }
        if (choice < 1 || choice > 6) {
            printf("\nWrong option choice!\n");
            continue;
        }
        if (!isConfigured && choice != 6) {
            printf("\n[ NETWORK NOT CONFIGURED ]\nEnter total routers (max %d): ", MAX);
            scanf("%d", &n);
            if (n <= 0 || n > MAX) {
                printf("\nInvalid number!\n");
                n = 0;
                continue;
            }
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (i == j) graph[i][j] = 0;
                    else graph[i][j] = INF;
                }
            }
            printf("\n--- ENTER SHARED TWO-WAY LINK COSTS ---\nType %d if there is no direct connection link.\n\n", INF);
            for (i = 0; i < n; i++) {
                for (j = i + 1; j < n; j++) {
                    printf("Link Cost between %c and %c: ", 'A' + i, 'A' + j);
                    scanf("%d", &newCost);

                    if (newCost == 0) {
                        newCost = INF;
                    }

                    graph[i][j] = newCost;
                    graph[j][i] = newCost;
                }
            }
            isConfigured = 1;
            displayMatrix(n, graph);
        }
        switch (choice) {
            case 1:
                printf("\n>> Running RIP Protocol\n");
                distanceVector(n, graph);
                break;
            case 2:
                printf("\n>> Running OSPF Protocol\n");
                linkState(n, graph);
                break;
            case 3:
                printf("\n>> Running Both Protocols\n");
                distanceVector(n, graph);
                linkState(n, graph);
                break;
            case 4:
                displayMatrix(n, graph);
                printf("\n--- MODIFY SHARED LINK COST ---\nEnter first router (A-%c): ", 'A' + n - 1);
                scanf(" %c", &modSrcChar);
                printf("Enter second router (A-%c): ", 'A' + n - 1);
                scanf(" %c", &modDestChar);

                modSrc = modSrcChar - 'A' + 1;
                modDest = modDestChar - 'A' + 1;

                if (modSrc < 1 || modSrc > n || modDest < 1 || modDest > n) {
                    printf("\nInvalid router options selected!\n");
                    break;
                }
                if (modSrc == modDest) {
                    printf("\nInternal loop metrics cannot change from 0!\n");
                    break;
                }
                printf("Enter new symmetric cost (use %d for link down): ", INF);
                scanf("%d", &newCost);

                if (newCost == 0) {
                    newCost = INF;
                }

                graph[modSrc - 1][modDest - 1] = newCost;
                graph[modDest - 1][modSrc - 1] = newCost;
                printf("\nShared link values updated dynamically!\n");
                displayMatrix(n, graph);
                break;
            case 5:
                findSpecificShortestPath(n, graph);
                break;
            default:
                break;
        }
    }
    printf("\n======\n FINISHED RUNNING\n=====\n");
    return 0;
}
