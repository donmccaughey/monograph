#include <libgen.h>
#include <stdio.h>
#include <stdlib.h> 

#include <monograph/monograph.h>

#include "options.h"


int
main(int argc, char *argv[])
{
    struct mg_options *options = mg_options_alloc(argc, argv);
    if (!options) {
        perror(basename(argv[0]));
        return EXIT_FAILURE;
    }

    if (mg_options_should_exit(options)) {
        int exit_status = mg_options_exit_status(options);
        mg_options_free(options);
        return exit_status;
    }

    struct mg_graph *graph = mg_graph_alloc_from_file(options->abs_path);
    if (!graph) {
        perror(options->command_name);
        mg_options_free(options);
        return EXIT_FAILURE;
    }
    
    struct mg_canvas *canvas = mg_canvas_alloc(options->canvas_size);
    if (!canvas) {
        perror(options->command_name);
        mg_graph_free(graph);
        mg_options_free(options);
        return EXIT_FAILURE;
    }
    
    mg_graph_draw(graph, canvas);
    printf("%s", canvas->text);

    mg_canvas_free(canvas);
    mg_graph_free(graph);
    mg_options_free(options);
    return EXIT_SUCCESS;
}

