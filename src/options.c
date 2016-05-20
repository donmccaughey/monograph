#include "options.h"

#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include "config.h"


static int const default_canvas_width = 78;
static int const default_canvas_height = 24;

static struct option long_options[] = {
    {
        .name="help",
        .has_arg=no_argument,
        .flag=NULL,
        .val='h'
    },
    {
        .name="version",
        .has_arg=no_argument,
        .flag=NULL,
        .val='V'
    },
    {
        .name=NULL,
        .has_arg=no_argument,
        .flag=NULL,
        .val=0
    }
};

static char const short_options[] = "hV";


static inline bool
is_abs_path(char const *path);

static inline bool
is_root_dir(char const *path);

static void
print_help(struct mg_options const *options);

static void
print_version(struct mg_options const *options);


static int
get_options(struct mg_options *options, int argc, char *argv[])
{
    int ch;
    int long_option_index;
    while (-1 != (ch = getopt_long(argc, argv, short_options, 
                                    long_options, &long_option_index))) 
    {
        switch (ch) {
            case 'h':
                options->help = true;
                break;
            case 'V':
                options->version = true;
                break;
            default:
                options->error = true;
                break;
        }
    }

    if (options->error || options->help) {
        print_help(options);
        return 0;
    }

    if (options->version) {
        print_version(options);
        return 0;
    }

    int remaining_arg_count = argc - optind;

    if (remaining_arg_count < 1) {
        options->error = true;
        fprintf(stderr, "%s: expected file argument\n", options->command_name);
        print_help(options);
        return 0;
    }

    if (remaining_arg_count > 1) {
        options->error = true;
        fprintf(stderr, "%s: too many file arguments\n", options->command_name);
        print_help(options);
        return 0;
    }

    options->given_path = strdup(argv[optind]);
    if (!options->given_path) return -1;

    if (is_abs_path(options->given_path)) {
        options->abs_path = strdup(options->given_path);
    } else if (is_root_dir(options->working_dir)) {
        asprintf(&options->abs_path, "/%s", options->given_path);
    } else {
        asprintf(&options->abs_path, "%s/%s", 
                options->working_dir, options->given_path);
    }
    if (!options->abs_path) return -1;

    return 0;
}


static inline bool
is_abs_path(char const *path)
{
    return '/' == path[0];
}


static inline bool
is_root_dir(char const *path)
{
    return is_abs_path(path) && '\0' == path[1];
}


struct mg_options *
mg_options_alloc(int argc, char *argv[])
{
    struct mg_options *options = calloc(1, sizeof(struct mg_options));
    if (!options) return NULL;
    
    options->canvas_size.width = default_canvas_width;
    options->canvas_size.height = default_canvas_height;

    char *command_name = basename(argv[0]);
    if (!command_name) {
        mg_options_free(options);
        return NULL;
    }

    options->command_name = strdup(command_name);
    if (!options->command_name) {
        mg_options_free(options);
        return NULL;
    }

    options->working_dir = getcwd(NULL, 0);
    if (!options->working_dir) {
        mg_options_free(options);
        return NULL;
    }

    int result = get_options(options, argc, argv);
    if (-1 == result) {
        mg_options_free(options);
        return NULL;
    }

    if (isatty(STDOUT_FILENO)) {
        struct winsize winsize;
        result = ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
        if (-1 == result) {
            mg_options_free(options);
            return NULL;
        }
        options->canvas_size.width = (int)winsize.ws_col;
        options->canvas_size.height = (int)winsize.ws_row - 2;
    }

    return options;
}


int
mg_options_exit_status(struct mg_options const *options)
{
    return options->error ? EXIT_FAILURE : EXIT_SUCCESS;
}


void
mg_options_free(struct mg_options *options)
{
    if (options) {
        free(options->abs_path);
        free(options->command_name);
        free(options->given_path);
        free(options->working_dir);
        free(options);
    }
}


bool
mg_options_should_exit(struct mg_options const *options)
{
    return options->error || options->help || options->version;
}


static void
print_help(struct mg_options const *options)
{
    FILE *out = options->error ? stderr : stdout;

    fprintf(out, "Usage: %s [options]... file\n", options->command_name);
    fprintf(out, "Print a monograph file\n");
    fprintf(out, "\n");
    fprintf(out, "  -h, --help       display this help message and exit\n");
    fprintf(out, "  -V, --version    display the version and exit\n");
    fprintf(out, "\n");
    fprintf(out, "Report bugs to <%s>\n", PACKAGE_BUGREPORT);
}


static void
print_version(struct mg_options const *options)
{
    if (0 == strcasecmp(options->command_name, PACKAGE_NAME)) {
        printf("%s\n", PACKAGE_STRING);
    } else {
        printf("%s (%s) %s\n", 
                options->command_name, PACKAGE_NAME, PACKAGE_VERSION);
    }
}

