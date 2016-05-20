#ifndef MONOGRAPH_OPTIONS_H_INCLUDED
#define MONOGRAPH_OPTIONS_H_INCLUDED


#include <stdbool.h>

#include <monograph/monograph.h>


struct mg_options {
    char *abs_path;
    struct mg_size canvas_size;
    char *command_name;
    bool error;
    char *given_path;
    bool help;
    bool version;
    char *working_dir;
};


struct mg_options *
mg_options_alloc(int argc, char *argv[]);

void
mg_options_free(struct mg_options *options);

int
mg_options_exit_status(struct mg_options const *options);

bool
mg_options_should_exit(struct mg_options const *options);


#endif

