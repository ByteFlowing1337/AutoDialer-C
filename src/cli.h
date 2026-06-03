/* CLI_H */
#ifndef CLI_H
#define CLI_H
#include <stdarg.h>  //va_start, va_end
#include <stdbool.h> // bool, true, false
#include <stdio.h>   // fprintf, stderr, stdout
#include <stdlib.h>  // EXIT_SUCCESS, EXIT_FAILURE, strtoul, setenv
#include <string.h>  // strchr, stderror
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define CYAN "\033[36m"

#define USAGE_FLAGS                                                          \
    BOLD BLUE "usage:" PURPLE " %s\n" RESET "\t[" BOLD GREEN "-h" RESET      \
              "] [" BOLD GREEN "-e" RESET " " YELLOW "<KEY=VAL>" RESET "]\
 [" BOLD GREEN "-v" RESET "] [" BOLD GREEN "-n" RESET " " YELLOW "<N>" RESET \
              "]\
 (" BOLD GREEN "-f" RESET " | " BOLD GREEN "-a" RESET " " YELLOW             \
              "<ASN>" RESET " | " BOLD GREEN "-c" RESET " | " BOLD GREEN     \
              "-d" RESET ")\n\n"

#define DEFAULT_ATTEMPTS 5

#ifndef VERSION
#define VERSION "0.1.0"
#endif

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "autodialer"
#endif

#define DESCRIPTION                                                          \
    "The autodialer command line utility is designed to streamline "         \
    "router "                                                                \
    "interactions.\n\n"

#define OPTIONS                                                              \
    BOLD BLUE                                                                \
        "options:" RESET "\n" BOLD "  " GREEN "-h, " CYAN "--help" RESET     \
        "           "                                                        \
        "show this help message and exit\n" BOLD "  " GREEN "-e, " CYAN      \
        "--env " YELLOW "<KEY=VAL>" RESET "  "                               \
        "Set environment variables (e.g., -e PANEL_PASSWORD=secret)\n" BOLD  \
        "  " GREEN "-v, " CYAN "--version" RESET "        "                  \
        "Show the current version of AutoDialer\n" BOLD "  " GREEN           \
        "-n, " CYAN "--attempts " YELLOW "<N>" RESET "   "                   \
        "Number of reconnection attempts before giving up (default: "        \
        "5)\n" BOLD "  " GREEN "-f, " CYAN "--force" RESET "          "      \
        "Force a reconnection regardless of current ASN.\n" BOLD "  " GREEN  \
        "-a, " CYAN "--asn " YELLOW "<ASN>" RESET "      "                   \
        "Reconnect until connected to the specified target ASN.\n" BOLD      \
        "  " GREEN "-c, " CYAN "--change" RESET "         "                  \
        "Reconnect until the public IP address changes.\n" BOLD "  " GREEN   \
        "-d, " CYAN "--devices" RESET "        "                             \
        "Display connected devices.\n\n"

typedef enum
{
    ACTION_FORCE,
    ACTION_ASN,
    ACTION_CHANGE,
    ACTION_DEVICES,
    ACTION_INVALID = -1,
} Action_type;

// CLI flags (-f, -a, -c, -d) actions are mutually exclusive
typedef struct
{
    Action_type action;
    unsigned int attempts;
    unsigned int asn;
} Config;

int cli_main(int argc, char **argv);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 1, 2)))
#endif
static inline void print_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Print the red "Error: " prefix
    fprintf(stderr, RED "Error: ");

    // Print your custom message and arguments
    vfprintf(stderr, format, args);

    // Print the reset sequence and newlines
    fprintf(stderr, RESET "\n\n");

    va_end(args);
}

static inline void print_help()
{
    fprintf(stdout, "%s %s\n\n", "AutoDialer", VERSION);
    fprintf(stdout, "%s", DESCRIPTION);
    fprintf(stdout, USAGE_FLAGS, PROGRAM_NAME);
    fprintf(stdout, "%s", OPTIONS);
}

static inline void print_usage(void)
{
    fprintf(stdout, USAGE_FLAGS, PROGRAM_NAME);
}
#endif // CLI_H