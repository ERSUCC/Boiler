#include <exception>
#include <iostream>
#include <string.h>

#include "boiler.h"
#include "info.h"

int main(int argc, char** argv)
{
    try
    {
        if (argc < 2)
        {
            printUsage();

            return 1;
        }

        if (!strncmp(argv[1], "-h", 3) || !strncmp(argv[1], "--help", 7))
        {
            printUsage();

            return 0;
        }

        if (!strncmp(argv[1], "-v", 3) || !strncmp(argv[1], "--version", 10))
        {
            printVersion();

            return 0;
        }

        if (!strncmp(argv[1], "list", 5))
        {
            if (argc > 2)
            {
                if (!strncmp(argv[2], "-h", 3) || !strncmp(argv[2], "--help", 7))
                {
                    printUsageList();

                    return 0;
                }

                if (!strncmp(argv[2], "-", 1))
                {
                    printUnknownCommand("option", "list", argv[2]);
                }

                else
                {
                    printUsageList();
                }

                return 1;
            }

            return (new Boiler())->list();
        }

        if (!strncmp(argv[1], "add", 4))
        {
            if (argc < 3)
            {
                printUsageAdd();

                return 1;
            }

            if (!strncmp(argv[2], "-h", 3) || !strncmp(argv[2], "--help", 7))
            {
                printUsageAdd();

                return 0;
            }

            if (!strncmp(argv[2], "-", 1))
            {
                printUnknownCommand("option", "add", argv[2]);

                return 1;
            }

            if (argc < 4)
            {
                printUsageAdd();

                return 1;
            }

            return (new Boiler())->add(argv[2], argv[3]);
        }

        if (!strncmp(argv[1], "load", 5))
        {
            if (argc < 3)
            {
                printUsageLoad();

                return 1;
            }

            if (!strncmp(argv[2], "-h", 3) || !strncmp(argv[2], "--help", 7))
            {
                printUsageLoad();

                return 0;
            }

            if (!strncmp(argv[2], "-", 1))
            {
                printUnknownCommand("option", "load", argv[2]);

                return 1;
            }

            const char* path = ".";

            if (argc >= 4)
            {
                path = argv[3];
            }

            return (new Boiler())->load(argv[2], path);
        }

        if (!strncmp(argv[1], "remove", 7))
        {
            if (argc < 3)
            {
                printUsageRemove();

                return 1;
            }

            if (!strncmp(argv[2], "-h", 3) || !strncmp(argv[2], "--help", 7))
            {
                printUsageRemove();

                return 0;
            }

            if (!strncmp(argv[2], "-", 1))
            {
                printUnknownCommand("option", "remove", argv[2]);

                return 1;
            }

            return (new Boiler())->remove(argv[2]);
        }

        if (!strncmp(argv[1], "-", 1))
        {
            printUnknown("option", argv[1]);
        }

        else
        {
            printUnknown("command", argv[1]);
        }

        return 1;
    }

    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";

        return 1;
    }
}
