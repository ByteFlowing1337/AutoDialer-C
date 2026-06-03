/* AutoDialer CLI main program*/
#include "cli.h"
#include "network.h"
#include <errno.h>
#include <limits.h>

static int parse_flag(int argc, char **argv, Flags *flags)
{
    const char *program_name = argc > 0 ? argv[0] : PROGRAM_NAME;
    if (argc < 2 ||
        (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        flags->type = FLAG_HELP;
        print_help(program_name);
        return EXIT_SUCCESS;
    }
    // Iterate through arguments and parse flags
    for (int i = 1; i < argc; i++)
    {

        /* -h | --help */
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            // Ignore any other flags if help is requested
            flags->type = FLAG_HELP;
            print_help(program_name);
            return EXIT_SUCCESS;
        }

        /* -v | --version */
        else if (strcmp(argv[i], "-v") == 0 ||
                 strcmp(argv[i], "--version") == 0)
        {
            // Ignore any other flags if version is requested
            flags->type = FLAG_VERSION;
            fprintf(stdout, "%s\n", VERSION);
            return EXIT_SUCCESS;
        }

        /* -e | --env <KEY=VAL> */
        else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--env") == 0)
        {
            if (i + 1 >= argc)
            {
                print_error("Expected argument after -e/--env.");
                return EXIT_FAILURE;
            }

            char *env_arg = argv[++i];
            char *equal_sign = strchr(env_arg, '=');

            if (!equal_sign || equal_sign == env_arg ||
                *(equal_sign + 1) == '\0')
            {
                print_error(
                    "Invalid environment variable format. Expected KEY=VAL.");
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
            continue;
        } /* end of -e | --env */

        /* -n | --attempts <N> */
        else if (strcmp(argv[i], "-n") == 0 ||
                 strcmp(argv[i], "--attempts") == 0)
        {
            if (i + 1 >= argc)
            {
                print_error("Expected argument after -n/--attempts.");
                return EXIT_FAILURE;
            }

            char *endptr;
            errno = 0; // Check overflow of strtol
            long long attempts = strtoll(argv[++i], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0' || attempts <= 0 ||
                (unsigned long long)attempts > UINT_MAX)
            {
                print_error(
                    "Invalid number of attempts. Valid range is 1 to %u.",
                    UINT_MAX);
                return EXIT_FAILURE;
            }

            flags->attempts = (unsigned int)attempts;
            continue;
        }

        /* -a | --asn <ASN> */
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--asn") == 0)
        {
            if (flags->actions_selected)
            {
                print_error(
                    "Flags -f, -a, -c, and -d are mutually exclusive.");
                return EXIT_FAILURE;
            }

            if (i + 1 >= argc)
            {
                print_error("Expected argument after -a/--asn.");
                return EXIT_FAILURE;
            }

            char *endptr;
            errno = 0; // Check overflow of strtol
            long long asn = strtoll(argv[++i], &endptr, 10);

            if (errno == ERANGE || *endptr != '\0' || asn <= 0 ||
                (unsigned long long)asn > UINT_MAX)
            {
                print_error("Invalid ASN value. Valid range is 1 to %u.",
                            UINT_MAX);
                return EXIT_FAILURE;
            }

            flags->type = FLAG_ASN;
            flags->asn = (unsigned int)asn;
            flags->actions_selected = true;
            continue;
        }
        /* end of -a | --asn */

        /* -f | --force */
        else if (strcmp(argv[i], "-f") == 0 ||
                 strcmp(argv[i], "--force") == 0)
        {
            if (flags->actions_selected)
            {
                print_error(
                    "Flags -f, -a, -c, and -d are mutually exclusive.");
                return EXIT_FAILURE;
            }
            flags->type = FLAG_FORCE;
            flags->actions_selected = true;
            continue;
        }

        /* -c | --change */
        else if (strcmp(argv[i], "-c") == 0 ||
                 strcmp(argv[i], "--change") == 0)
        {
            if (flags->actions_selected)
            {
                print_error("Flags [-f | --force], [-a | --asn], [-c | "
                            "--change], and [-d | --devices] "
                            "are mutually exclusive.");
                return EXIT_FAILURE;
            }
            flags->type = FLAG_CHANGE;
            flags->actions_selected = true;
            continue;
        }

        /* -d | --devices */
        else if (strcmp(argv[i], "-d") == 0 ||
                 strcmp(argv[i], "--devices") == 0)
        {
            if (flags->actions_selected)
            {
                print_error(
                    "Flags [-f | --force], [-a | --asn], [-c | --change], "
                    "and [-d | --devices] are mutually exclusive.");
                return EXIT_FAILURE;
            }
            flags->type = FLAG_DEVICES;
            flags->actions_selected = true;
            continue;
        }

        /* Invalid flag */
        else
        {
            print_error("%s is not a valid option.", argv[i]);
            print_help(program_name);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

static int process_flags(Flags *flags)
{
    switch (flags->type)
    {
    case FLAG_FORCE:
        return force_reconnect(flags->attempts);
    case FLAG_ASN:
        return asn_reconnect(flags->attempts, flags->asn);
    case FLAG_CHANGE:
        return change_ip_reconnect(flags->attempts);
    case FLAG_DEVICES:
        return display_connected_devices();
    default:
        if (flags->type == FLAG_INVALID)
        {
            print_error("No valid flag provided. Use -h or --help for usage "
                        "information.");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}

int cli_main(int argc, char **argv)
{
    Flags flags = {.type = FLAG_INVALID,
                   .actions_selected = false,
                   .attempts = DEFAULT_ATTEMPTS,
                   .asn = 0};
    asm("int3");
    int result = parse_flag(argc, argv, &flags);
    if (result != EXIT_SUCCESS)
    {
        return result;
    }
    return process_flags(&flags);
}