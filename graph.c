#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));

    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

void graph_free(Graph **gp) {
    if (gp != NULL && *gp != NULL) {
        if ((*gp)->visited) {
            free((*gp)->visited);
            (*gp)->visited = NULL;
        }

        if ((*gp)->names) {
            // free each name
            for (int name = 0; name < (int) (*gp)->vertices; name++) {
                free((*gp)->names[name]);
                (*gp)->names[name] = NULL;
            }
            // then free the names array
            free((*gp)->names);
            (*gp)->names = NULL;
        }

        if ((*gp)->weights) {
            // free each name
            for (int weight_row = 0; weight_row < (int) (*gp)->vertices; weight_row++) {
                free((*gp)->weights[weight_row]);
                (*gp)->weights[weight_row] = NULL;
            }
            // then free the names array
            free((*gp)->weights);
            (*gp)->weights = NULL;
        }
        free(*gp);
    }

    if (gp != NULL) {
        *gp = NULL;
    }
}

uint32_t graph_vertices(const Graph *g) {
    return (g->vertices);
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    // creates a duplicate of the name string and stores it in the graph object
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return (g->names[v]);
}

char **graph_get_names(const Graph *g) {
    return (g->names);
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    g->weights[start][end] = weight;
    // if the graph is directed, then only add edge in the specified direction. otherwise, add edge in the opposite
    // direction too
    if ((g->directed) == false) {
        g->weights[end][start] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return (g->weights[start][end]);
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    return (g->visited[v]);
}

void graph_print(const Graph *g) {
    printf("going to city #    ");
    for (int top = 0; top < (int) (g->vertices); top++) {
        printf("%5d", top);
    }
    printf("\n\n");
    for (int row = 0; row < (int) (g->vertices); row++) {
        printf("starting at city #%d", row);
        for (int col = 0; col < (int) (g->vertices); col++) {
            printf("%5d", g->weights[row][col]);
        }
        printf("\n");
    }
}
