/* AutoDialer CLI main program*/
#include "cli.h"
#include "network.h" // force_reconnect, asn_reconnect, change_ip_reconnect, display_connected_devices
#include <errno.h>   // errno, ERANGE
#include <getopt.h> // getopt_long, option, optarg, optind, no_argument, required_argument
#include <limits.h> // UINT_MAX

static int parse_flag(int argc, char **argv, Config *config)
{
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"env", required_argument, 0, 'e'},
        {"attempts", required_argument, 0, 'n'},
        {"force", no_argument, 0, 'f'},
        {"asn", required_argument, 0, 'a'},
        {"change", no_argument, 0, 'c'},
        {"devices", no_argument, 0, 'd'},
        {0, 0, 0, 0},
    };
    int opt;
    int action_flags_count = 0;
    optind = 1;
    while ((opt = getopt_long(argc, argv, "hve:n:fa:cd", long_options,
                              NULL)) != -1)
    {
        switch (opt)
        {
        /* -h | --help */
        case 'h':
            print_help();
            exit(EXIT_SUCCESS);

        /* -v | --version */
        case 'v': {
            fprintf(stdout, "%s\n", VERSION);
            exit(EXIT_SUCCESS);
        }

        /* -e | --env <KEY=VAL> */
        case 'e':
            char *env_arg = optarg;
            char *equal_sign = strchr(env_arg, '=');

            if (!equal_sign || equal_sign == env_arg ||
                *(equal_sign + 1) == '\0')
            {
                print_error("Invalid environment variable format. "
                            "Expected KEY=VAL.");
                fprintf(stderr, "Usage: [-e | --env] <KEY=VAL>\n");
                return EXIT_FAILURE;
            }

            *equal_sign = '\0';
            char *key = env_arg;
            char *value = equal_sign + 1;

#if defined(_WIN32)
            errno_t error_num = _putenv_s(key, value);
            if (error_num != 0)
            {
                print_error("Failed to set environment variable: %s",
                            strerror(error_num));
                return EXIT_FAILURE;
            }
#else
            int error_num = setenv(key, value, 1);
            if (error_num != 0)
            {
                print_error("Failed to set environment variable: %s",
                            strerror(error_num));
                return EXIT_FAILURE;
            }
#endif
            break;
        /* end of -e | --env */

        /* -n | --attempts <N> */
        case 'n': {
            char *endptr;
            errno = 0; // Check overflow of strtol
            long long attempts = strtoll(optarg, &endptr, 10);

            if (errno == ERANGE || *endptr != '\0' || attempts <= 0 ||
                (unsigned long long)attempts > UINT_MAX)
            {
                print_error(
                    "Invalid number of attempts. Valid range is 1 to %u.",
                    UINT_MAX);
                return EXIT_FAILURE;
            }

            config->attempts = (unsigned int)attempts;
            break;
        }

        /* -a | --asn <ASN> */
        case 'a': {
            char *endptr;
            errno = 0; // Check overflow of strtol
            long long asn = strtoll(optarg, &endptr, 10);

            if (errno == ERANGE || *endptr != '\0' || asn <= 0 ||
                (unsigned long long)asn > UINT_MAX)
            {
                print_error("Invalid ASN value. Valid range is 1 to %u.",
                            UINT_MAX);
                return EXIT_FAILURE;
            }

            config->action = ACTION_ASN;
            config->asn = (unsigned int)asn;
            action_flags_count++;
            break;
        }
        /* end of -a | --asn */

        /* -f | --force */
        case 'f':
            config->action = ACTION_FORCE;
            action_flags_count++;
            break;

        /* -c | --change */
        case 'c':
            config->action = ACTION_CHANGE;
            action_flags_count++;
            break;

        /* -d | --devices */
        case 'd':
            config->action = ACTION_DEVICES;
            action_flags_count++;
            break;

        /* Invalid flag */
        default:
            print_usage();
            return EXIT_FAILURE;
        }
    }
    if (optind < argc)
    {
        print_error("Unexpected argument: %s", argv[optind]);
        print_usage();
        return EXIT_FAILURE;
    }
    if (action_flags_count > 1)
    {
        print_error(
            "Multiple action config specified. Please choose only one of "
            "-f/--force, -a/--asn, -c/--change, or -d/--devices.");
        return EXIT_FAILURE;
    }
    if (action_flags_count == 0)
    {
        print_error("No action flag specified. Please provide one of "
                    "-f/--force, -a/--asn, -c/--change, or -d/--devices.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int process_flags(const Config *config)
{
    switch (config->action)
    {
    case ACTION_FORCE:
        return force_reconnect(config->attempts);
    case ACTION_ASN:
        return asn_reconnect(config->attempts, config->asn);
    case ACTION_CHANGE:
        return change_ip_reconnect(config->attempts);
    case ACTION_DEVICES:
        return display_connected_devices();
    default:
        /* This should never happen since we validate config during parsing */
        return EXIT_FAILURE;
    }
}

int cli_main(int argc, char **argv)
{
    if (argc == 1)
    {
        print_help();
        return EXIT_SUCCESS;
    }
    Config config = {
        .action = ACTION_INVALID, .attempts = DEFAULT_ATTEMPTS, .asn = 0};
    int result = parse_flag(argc, argv, &config);
    if (result != EXIT_SUCCESS)
    {
        return result;
    }
    return process_flags(&config);
}