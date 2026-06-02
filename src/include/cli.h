/* CLI_H */
#ifndef CLI_H
#define CLI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"

#define USAGE_FLAGS BOLD BLUE"usage:"PURPLE" %s\n"RESET"\t["BOLD GREEN"-h"RESET"] ["BOLD GREEN"-e"RESET" "YELLOW"<KEY=VAL>"RESET"]\
 ["BOLD GREEN"-v"RESET"] ["BOLD GREEN"-n"RESET" "YELLOW"<N>"RESET"]\
 ("BOLD GREEN"-f"RESET" | "BOLD GREEN"-a"RESET" "YELLOW"<ASN>"RESET" | "BOLD GREEN"-c"RESET" | "BOLD GREEN"-d"RESET")\n\n"\

#define DEFAULT_ATTEMPTS 5
#ifndef VERSION
    #define VERSION "0.1.0"
#endif

static const char* DESCRIPTION = 
        "The autodialer command line utility is designed to streamline router interactions.\n\n";

static const char* OPTIONS = 
        BOLD BLUE"options:"RESET"\n"
        BOLD"  "GREEN"-h, "CYAN"--help"RESET"           show this help message and exit\n"
        BOLD"  "GREEN"-e, "CYAN"--env "YELLOW"<KEY=VAL>"RESET"  Set environment variables (e.g., -e PANEL_PASSWORD=secret)\n"
        BOLD"  "GREEN"-v, "CYAN"--version"RESET"        Show the current version of AutoDialer\n"
        BOLD"  "GREEN"-n, "CYAN"--attempts "YELLOW"<N>"RESET"   Number of reconnection attempts before giving up (default: 5)\n"
        BOLD"  "GREEN"-f, "CYAN"--force"RESET"          Force a reconnection regardless of current ASN.\n"
        BOLD"  "GREEN"-a, "CYAN"--asn "YELLOW"<ASN>"RESET"      Reconnect until connected to the specified target ASN.\n"
        BOLD"  "GREEN"-c, "CYAN"--change"RESET"         Reconnect until the public IP address changes.\n"
        BOLD"  "GREEN"-d, "CYAN"--devices"RESET"        Display connected devices.\n\n";

typedef enum {
    FLAG_HELP,
    FLAG_VERSION,
    FLAG_FORCE,
    FLAG_ASN,
    FLAG_CHANGE,
    FLAG_DEVICES,
    FLAG_INVALID = -1,
} Flag;

// CLI flags (-f, -a, -c, -d) actions are mutually exclusive
typedef struct {
    Flag type;
    bool actions_selected;
    unsigned int attempts;
    unsigned int asn;
} Flags;

extern int cli_main(int argc, char **argv);

static inline void print_error(const char* format, ...) {
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

static inline void print_help(const char* program_name) {
    fprintf(stdout, "%s %s\n\n", "AutoDialer", VERSION);
    fprintf(stdout, "%s", DESCRIPTION);
    fprintf(stdout, USAGE_FLAGS, program_name);
    fprintf(stdout, "%s", OPTIONS);
}
#endif // CLI_H