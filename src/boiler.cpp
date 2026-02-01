#include "boiler.h"

Boiler::Boiler() :
    root(platformRoot())
{
    std::filesystem::create_directories(root);
}

int Boiler::list() const
{
    std::vector<std::filesystem::path> paths;

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(root))
    {
        if (entry.is_directory())
        {
            paths.push_back(entry.path());
        }
    }

    if (paths.empty())
    {
        std::cout << "No boilerplates found.\n";

        return 0;
    }

    std::sort(paths.begin(), paths.end());

    for (const std::filesystem::path& path : paths)
    {
        std::cout << path.filename().string() << "\n";
    }

    return 0;
}

int Boiler::add(const char* name, const std::filesystem::path path) const
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "The specified path does not exist.\n";

        return 1;
    }

    const std::filesystem::path dest = root / name;

    if (std::filesystem::exists(dest))
    {
        std::cout << "A boilerplate already exists with the name \"" << name << "\". Would you like to overwrite it? [y/n] ";

        if (getchar() != 'y')
        {
            return 0;
        }
    }

    std::filesystem::copy(path, dest, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);

    std::cout << "Saved new boilerplate \"" << name << "\".\n";

    return 0;
}

int Boiler::load(const char* name, const std::filesystem::path path) const
{
    const std::filesystem::path source = root / name;

    if (!std::filesystem::exists(source))
    {
        std::cout << "No boilerplate exists with the name \"" << name << "\".\n";

        return 1;
    }

    std::filesystem::create_directories(path);

    if (!std::filesystem::is_directory(path))
    {
        std::cout << "The specified path is not a directory.\n";

        return 1;
    }

    if (!std::filesystem::is_empty(path))
    {
        std::cout << "The target directory is not empty. Would you like to overwrite it? [y/n] ";

        if (getchar() != 'y')
        {
            return 0;
        }
    }

    std::filesystem::remove_all(path);
    std::filesystem::copy(source, path, std::filesystem::copy_options::recursive);

    std::cout << "Loaded boilerplate \"" << name << "\".\n";

    return 0;
}

int Boiler::remove(const char* name) const
{
    const std::filesystem::path path = root / name;

    if (!std::filesystem::exists(path))
    {
        std::cout << "No boilerplate exists with the name \"" << name << "\".\n";

        return 1;
    }

    std::cout << "Are you sure you want to remove boilerplate \"" << name << "\"? [y/n] ";

    if (getchar() != 'y')
    {
        return 0;
    }

    std::filesystem::remove_all(path);

    std::cout << "Removed boilerplate \"" << name << "\".\n";

    return 0;
}

#ifdef _WIN32

#include <objbase.h>
#include <ShlObj.h>

std::filesystem::path Boiler::platformRoot()
{
    PWSTR path;

    if (FAILED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path)))
    {
        CoTaskMemFree(path);

        throw std::system_error(std::error_code(errno, std::system_category()), "Fatal: Failed to locate AppData directory.");
    }

    std::filesystem::path result = std::filesystem::path(path) / "Boiler";

    CoTaskMemFree(path);

    return result;
}

#elif __APPLE__

#include <pwd.h>
#include <unistd.h>

std::filesystem::path Boiler::platformRoot()
{
    struct passwd* pw = getpwuid(getuid());

    if (!pw)
    {
        throw std::system_error(std::error_code(errno, std::system_category()), "Fatal: Failed to locate user home directory.");
    }

    return std::filesystem::path(pw->pw_dir) / "Library" / "Boiler";
}

#else

#include <pwd.h>
#include <unistd.h>

std::filesystem::path Boiler::platformRoot()
{
    struct passwd* pw = getpwuid(getuid());

    if (!pw)
    {
        throw std::system_error(std::error_code(errno, std::system_category()), "Fatal: Failed to locate user home directory.");
    }

    return std::filesystem::path(pw->pw_dir) / ".boiler";
}

#endif
