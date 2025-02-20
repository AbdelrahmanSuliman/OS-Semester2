#include <stdio.h>
#include <stdlib.h>

int processes;
int resources;

int totalResources[10];
int allocationMatrix[10][10];
int requestMatrix[10][10];
int availableResources[10];
int finish[10];

void readFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fscanf(file, "%d %d", &processes, &resources);

    for (int i = 0; i < resources; i++) {
        fscanf(file, "%d", &totalResources[i]);
    }

    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            fscanf(file, "%d", &allocationMatrix[i][j]);
        }
    }

    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            fscanf(file, "%d", &requestMatrix[i][j]);
        }
    }

    fclose(file);
}

void calculateAvailableResources() {
    for (int j = 0; j < resources; j++) {
        int allocated = 0;
        for (int i = 0; i < processes; i++) {
            allocated += allocationMatrix[i][j];
        }
        availableResources[j] = totalResources[j] - allocated;
    }
}

void detectDeadlock() {
    for (int i = 0; i < processes; i++) {
        finish[i] = 0;
    }

    int done = 0;
    while (done < processes) {
        int found = 0;

        for (int i = 0; i < processes; i++) {
            if (!finish[i]) {
                int canRun = 1;
                for (int j = 0; j < resources; j++) {
                    if (requestMatrix[i][j] > availableResources[j]) {
                        canRun = 0;
                        break;
                    }
                }

                if (canRun) {
                    for (int j = 0; j < resources; j++) {
                        availableResources[j] += allocationMatrix[i][j];
                    }
                    finish[i] = 1;
                    found = 1;
                    done++;
                }
            }
        }

        if (!found) break;  
    }

    int deadlock = 0;
    printf("Deadlocked processes: ");
    for (int i = 0; i < processes; i++) {
        if (!finish[i]) {
            printf("P%d ", i);
            deadlock = 1;
        }
    }
    if (!deadlock) {
        printf("None");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide a filename\n");
        return 1;
    }

    readFile(argv[1]);
    calculateAvailableResources();
    detectDeadlock();

    return 0;
}

