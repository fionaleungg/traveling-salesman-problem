#include "path.h"

#include "graph.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

// remember that the capacity that we want is the number of vertices + 1 (for returning back to start)

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) calloc(1, sizeof(Path));
    p->vertices = stack_create(capacity);
    p->total_weight = 0;
    return p;
}

void path_free(Path **pp) {
    if (pp != NULL && *pp != NULL) {
        stack_free(&((*pp)->vertices));
        free(*pp);
    }
    if (pp != NULL) {
        *pp = NULL;
    }
}

uint32_t path_vertices(const Path *p) {
    return (stack_size(p->vertices));
}

uint32_t path_distance(const Path *p) {
    return (p->total_weight);
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    // make sure that a push is possible first! (path is NOT full)
    //if (!(stack_full(p->vertices))){
    //        stack_push(p->vertices, val);
    //}

    // make sure that a peek is possible first! (path is NOT empty)
    if (!(stack_empty(p->vertices))) {
        // if the path is NOT empty then proceed to increase the distance (total_weight)
        // figure out where the previous city was (starting city)
        uint32_t start = 0;
        stack_peek(p->vertices, &start);
        // add the distance between the starting city and next city (val) to the total_weight
        p->total_weight += graph_get_weight(g, start, val);
    }
    // make sure that a push is possible first! (path is NOT full)
    if (!(stack_full(p->vertices))) {
        stack_push(p->vertices, val);
    }
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t removed = 0;
    uint32_t new_top_item = 0;
    // if there are at least 2 vertices in the path, then remove the last one and peek at the next vertex
    if (stack_size(p->vertices) >= (uint32_t) 2) {
        stack_pop(p->vertices, &removed);
        stack_peek(p->vertices, &new_top_item);
        p->total_weight -= graph_get_weight(g, new_top_item, removed);
        // if there's only one vertex in the path, then just remove the last vertex
    } else if (stack_size(p->vertices) == (uint32_t) 1) {
        stack_pop(p->vertices, &removed);
        p->total_weight = 0;
    }
    // return the removed vertex's index
    return (removed);
}

void path_copy(Path *dst, const Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->total_weight = src->total_weight;
}

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    uint32_t top_index = 0;
    uint32_t size = stack_size(p->vertices);
    Stack *backwards = stack_create(size);
    //printf("stack_size: %u\n", stack_size(p->vertices));
    for (uint32_t i = 0; i < size; i++) {
        //printf("i: %u\n", i);
        stack_pop(p->vertices, &top_index);
        stack_push(backwards, top_index);
        //fprintf(outfile, "%s\n", (graph_get_names(g))[(int) top_index]);
    }
    for (uint32_t i = 0; i < size; i++) {
        stack_pop(backwards, &top_index);
        fprintf(outfile, "%s\n", (graph_get_names(g))[(int) top_index]);
    }
    stack_free(&backwards);
}
