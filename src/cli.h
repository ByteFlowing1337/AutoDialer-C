#ifndef CLI_H
#define CLI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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


const char* VERSION = "0.1.0";

const char* DESCRIPTION = 
        "The autodialer command line utility is designed to streamline router interactions.\n\n";

const char* OPTIONS = 
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
    FLAG_ENV,
    FLAG_VERSION,
    FLAG_ATTEMPTS,
    FLAG_FORCE,
    FLAG_ASN,
    FLAG_CHANGE,
    FLAG_DEVICES,
    FLAG_INVALID = -1,
    FLAG_COUNT
} Flag;

Flag parse_flag(const char* arg) {
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) return FLAG_HELP;
        if (strcmp(arg, "-e") == 0 || strcmp(arg, "--env") == 0) return FLAG_ENV;
        if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) return FLAG_VERSION;
        if (strcmp(arg, "-n") == 0 || strcmp(arg, "--attempts") == 0) return FLAG_ATTEMPTS;
        if (strcmp(arg, "-f") == 0 || strcmp(arg, "--force") == 0) return FLAG_FORCE;
        if (strcmp(arg, "-a") == 0 || strcmp(arg, "--asn") == 0) return FLAG_ASN;
        if (strcmp(arg, "-c") == 0 || strcmp(arg, "--change") == 0) return FLAG_CHANGE;
        if (strcmp(arg, "-d") == 0 || strcmp(arg, "--devices") == 0) return FLAG_DEVICES;
        return FLAG_INVALID; // Invalid flag
    }

void print_info() {
    fprintf(stdout, "%s %s\n\n", "AutoDialer", VERSION);
    fprintf(stdout, "%s", DESCRIPTION);
}

void print_invalid_flag(const char* flag) {
    fprintf(stderr, RED"Error:"RESET" Invalid flag '%s'. Use -h or --help for usage information.\n\n", flag);
}

void print_help(const char* program_name) {
    fprintf(stdout, USAGE_FLAGS, program_name);
    fprintf(stdout, "%s", OPTIONS);
}
#endif // CLI_H