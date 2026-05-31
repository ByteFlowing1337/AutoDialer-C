#ifndef CLI_H
#define CLI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"

#define USAGE_FLAGS BLUE"usage:"PURPLE" %s\n"RESET"\t["BLUE"-h"RESET"] ["BLUE"-e"RESET" "YELLOW"<KEY=VAL>"RESET"]\
 ["BLUE"-v"RESET"] ["BLUE"-n"RESET" "YELLOW"<N>"RESET"]\
 ("BLUE"-f"RESET" | "BLUE"-a"RESET" "YELLOW"<ASN>"RESET" | "BLUE"-c"RESET" | "BLUE"-d"RESET")\n\n"\


const char* VERSION = "0.1.0";

const char* DESCRIPTION = 
        "The autodialer command line utility is designed to streamline router interactions.\n\n";

const char* OPTIONS = 
        BLUE"options:"RESET"\n"
        "  "GREEN"-h, --help"RESET"           show this help message and exit\n"
        "  "GREEN"-e, --env "YELLOW"<KEY=VAL>"RESET"  Set environment variables (e.g., -e PANEL_PASSWORD=secret)\n"
        "  "GREEN"-v, --version"RESET"        Show the current version of AutoDialer\n"
        "  "GREEN"-n, --attempts "YELLOW"<N>"RESET"   Number of reconnection attempts before giving up (default: 5)\n"
        "  "GREEN"-f, --force"RESET"          Force a reconnection regardless of current ASN.\n"
        "  "GREEN"-a, --asn "YELLOW"<ASN>"RESET"      Reconnect until connected to the specified target ASN.\n"
        "  "GREEN"-c, --change"RESET"         Reconnect until the public IP address changes.\n"
        "  "GREEN"-d, --devices"RESET"        Display connected devices.\n\n";

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
#define INVALID (bool)1
#define VALID (bool)0

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