#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

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
        return mg_options_exit_status(options);
    }

    printf("Hello world!\n");

    mg_options_free(options);
    return EXIT_SUCCESS;
}

