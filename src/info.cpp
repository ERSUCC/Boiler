#include "info.h"

void printUnknown(const char* what, const char* str)
{
    std::cout << "Unknown " << what << " \"" << str << "\". Run \"boiler --help\" for usage instructions.\n";
}

void printUnknownCommand(const char* what, const char* command, const char* str)
{
    std::cout << "Unknown " << what << " \"" << str << "\" for command \"" << command << "\". Run \"boiler " << command << " --help\" for usage instructions.\n";
}

void printVersion()
{
    std::cout << "boiler v0.1.0\n";
}

void printUsage()
{
    std::cout << "Usage: boiler [-v | --version] [-h | --help] <command> [<options>]\n\n";
    std::cout << "Available commands:\n\n";
    std::cout << "list: List all saved boilerplates\n";
    std::cout << "add: Add a new boilerplate\n";
    std::cout << "load: Load an existing boilerplate\n";
    std::cout << "remove: Remove an existing boilerplate\n\n";
    std::cout << "Run \"boiler <command> --help\" for more information about a specific command.\n";
}

void printUsageList()
{
    std::cout << "Usage: boiler list [-h | --help]\n\n";
    std::cout << "Lists all saved boilerplates.\n";
}

void printUsageInspect()
{
    std::cout << "Usage: boiler inspect [-h | --help] <name>\n\n";
    std::cout << "Displays the contents of the boilerplate with the specified name.\n";
}

void printUsageAdd()
{
    std::cout << "Usage: boiler add [-h | --help] <name> <path>\n\n";
    std::cout << "Adds a new boilerplate with the specified name, using the contents of the specified path.\n\n";
    std::cout << "If a boilerplate already exists with the specified name, boiler will ask for overwrite confirmation.\n";
    std::cout << "If the specified path is a directory, its contents will be used for the new boilerplate, but not the directory itself.\n";
}

void printUsageLoad()
{
    std::cout << "Usage: boiler load [-h | --help] <name> [<directory>]\n\n";
    std::cout << "Copies the contents of the boilerplate with the specified name into the specified directory.\n\n";
    std::cout << "If no directory is specified, the contents will be copied into the current directory.\n";
    std::cout << "If a directory is specified but does not exist, it will be created.\n";
    std::cout << "If the target directory is not empty, boiler will ask for overwrite confirmation.\n";
}

void printUsageRemove()
{
    std::cout << "Usage: boiler remove [-h | --help] <name>\n\n";
    std::cout << "Removes the boilerplate with the specified name.\n";
}
