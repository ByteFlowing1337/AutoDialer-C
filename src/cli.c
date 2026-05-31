#include "cli.h"

int main(int argc, char **argv) {
    if (argc <2) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }
    switch (parse_flag(argv[1])) {
        case FLAG_VERSION:
            fprintf(stdout, "AutoDialer version: %s\n", VERSION);
            break;
        case FLAG_HELP:
            print_help(argv[0]);
            break;
        case FLAG_ENV:
            // Handle environment variable setting
            break;
        case FLAG_ATTEMPTS:
            // Handle attempts setting
            break;
        case FLAG_FORCE:
            // Handle force reconnection
            break;
        case FLAG_ASN:
            // Handle ASN targeting
            break;
        case FLAG_CHANGE:
            // Handle IP change targeting
            break;
        case FLAG_DEVICES:
            // Handle displaying connected devices
            break;
        case FLAG_INVALID:
            print_info();
            print_invalid_flag(argv[1]);
            print_help(argv[0]);
            return EXIT_FAILURE;
        default:
            print_info();
            print_help(argv[0]);
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}