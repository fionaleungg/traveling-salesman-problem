#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "i:o:dh"
#define HELP                                                                                       \
    "Usage: tsp [options]\n\n"                                                                     \
    "-i infile    Specify the input file path containing the cities and edges\n"                   \
    "             of a graph. If not specified, the default input should be\n"                     \
    "             set as stdin.\n\n"                                                               \
    "-o outfile   Specify the output file path to print to. If not specified,\n"                   \
    "             the default output should be set as stdout.\n\n"                                 \
    "-d           Specifies the graph to be directed.\n\n"                                         \
    "-h           Prints out a help message describing the purpose of the\n\n"                     \
    "             graph and the command-line options it accepts, exiting the\n"                    \
    "             program afterwards.\n"

void dfs(uint32_t node, Graph *g, Path *p, Path *best);

int main(int argc, char **argv) {
    int opt = 0;
    bool dash_i = false;
    bool dash_o = false;

    char *in = NULL;
    char *out = NULL;

    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool directed = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            printf("%s", HELP);
            exit(1);
            break;
        case 'i':
            dash_i = true;
            in = optarg;
            break;
        case 'o':
            dash_o = true;
            out = optarg;
            break;
        case 'd': directed = true; break;
        default: exit(1); break;
        }
    }

    if (dash_i) {
        infile = fopen(in, "r");
        if (infile == NULL) {
            fprintf(stderr, "tsp:  unknown or poorly formatted option -i\n");
            exit(1);
        }
    }
    if (dash_o) {
        outfile = fopen(out, "w");
        if (outfile == NULL) {
            fprintf(stderr, "tsp:  unknown or poorly formatted option -o\n");
            exit(1);
        }
    }
    uint32_t num_vertices = 0;
    uint32_t num_edges = 0;
    char location[1024]; // assuming that 256 is the max amount of characters in a location
    uint32_t start_vertex = 0;
    uint32_t end_vertex = 0;
    uint32_t weight_between = 0;
    char *newline;
    // finding the number of locations and storing them in the graph
    if (fscanf(infile, "%u\n", &num_vertices) != 1) {
        fprintf(stderr, "tsp:  error reading number of vertices\n");
        exit(1);
    }

    // creating a graph
    struct graph;
    Graph *adj_matrix = graph_create(num_vertices, directed);
    //printf("num_vertices: %u\n", num_vertices);

    for (int i = 0; i < (int) num_vertices; i++) {
        // keep reading line until \n is detected. treat \n as the only delimiter
        if (fgets(location, 1024, infile) == NULL) {
            fprintf(stderr, "tsp:  error reading cities[%d]\n", i);
            graph_free(&adj_matrix);
            exit(1);
        }
        // remove trailing \n from fgets()
        newline = strchr(location, '\n');
        *newline = '\0';

        graph_add_vertex(adj_matrix, location, (uint32_t) i);
    }

    // finding the number of edges and storing the start vertex, end vertex, and weight that info in the graph
    if (fscanf(infile, "%u\n", &num_edges) != 1) {
        fprintf(stderr, "tsp: error reading number of edges\n");
        graph_free(&adj_matrix);
        exit(1);
    }

    for (int i = 0; i < (int) num_edges; i++) {
        if (fscanf(infile, "%u %u %u", &start_vertex, &end_vertex, &weight_between) != 3) {
            fprintf(stderr, "tsp:  error reading edge\n");
            graph_free(&adj_matrix);
            exit(1);
        }
        graph_add_edge(adj_matrix, start_vertex, end_vertex, weight_between);
    }

    if (infile != stdin) {
        fclose(infile);
    }

    // dfs
    // remember to make two paths. one for computing and one for storing the current best path
    struct path;
    Path *current_path = path_create(num_vertices + ((uint32_t) 1));
    Path *best_path = path_create(num_vertices + ((uint32_t) 1));

    dfs(START_VERTEX, adj_matrix, current_path, best_path);

    // if the calculated path is not 0, then print the path
    if (path_distance(best_path)) {
        fprintf(outfile, "Alissa starts at:\n");
        path_print(best_path, outfile, adj_matrix);
        fprintf(outfile, "Total Distance: %u\n", path_distance(best_path));
    } else {
        printf("No path found! Alissa is lost!\n");
    }

    if (outfile != stdout) {
        fclose(outfile);
    }

    path_free(&current_path);
    path_free(&best_path);
    graph_free(&adj_matrix);
    return 0;
}

void dfs(uint32_t node, Graph *g, Path *p, Path *best) {
    // mark node as visited
    graph_visit_vertex(g, node);
    path_add(p, node, g);
    // for every edge that the node can go to...
    for (uint32_t edge = 0; edge < graph_vertices(g); edge++) {

        // find next possible edge, if it's unvisited, then run dfs on that edge
        if ((graph_get_weight(g, node, edge) != (uint32_t) 0)
            && (graph_visited(g, edge) == false)) {
            dfs(edge, g, p, best);
        }
        // check if this is a path that alissa can take (every beach has been visited)
        if (path_vertices(p) == graph_vertices(g)) {
            // check if alissa can get home using this path (there's an edge between the current node and
            // the starting vertex)
            if (graph_get_weight(g, node, START_VERTEX)) {
                path_add(p, START_VERTEX, g);
                // determine if this is our best path by comparing it to the current best path
                // if the best path's distance is still 0 (first time comparing a path with the
                // best path), then copy the full path to best path
                if (path_distance(best) == 0) {
                    path_copy(best, p);
                    path_remove(p, g);
                } else if (path_distance(p) < path_distance(best)) {
                    // if the just computed path is shorter than the the current best path,
                    // then copy the current path to the best path
                    path_copy(best, p);
                    path_remove(p, g);
                } else {
                    path_remove(p, g);
                }
            }
        }
    }
    graph_unvisit_vertex(g, node);
    path_remove(p, g);
}
