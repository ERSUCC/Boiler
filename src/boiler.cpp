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

int Boiler::inspect(const std::string& name) const
{
    if (name.empty())
    {
        std::cout << "The specified name is empty.\n";

        return 1;
    }

    const std::filesystem::path path = root / name;

    if (!std::filesystem::exists(path))
    {
        std::cout << "No boilerplate exists with the name \"" << name << "\".\n";

        return 1;
    }

    listFilesRecursive(path, 0);

    return 0;
}

int Boiler::add(const std::string& name, const std::filesystem::path path) const
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "The specified path does not exist.\n";

        return 1;
    }

    if (name.empty())
    {
        std::cout << "The specified name is empty.\n";

        return 1;
    }

    const std::filesystem::path dest = root / name;

    if (dest.string().size() >= maxPath())
    {
        std::cout << "The specified name exceeds the maximum allowable path length.\n";

        return 1;
    }

    if (std::filesystem::exists(dest))
    {
        std::cout << "A boilerplate already exists with the name \"" << name << "\". Would you like to overwrite it? [y/n] ";
        std::cout.flush();

        if (getchar() != 'y')
        {
            return 0;
        }
    }

    std::filesystem::create_directories(dest);

    std::vector<std::filesystem::path> paths;

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dest))
    {
        paths.push_back(entry.path());
    }

    for (const std::filesystem::path& path : paths)
    {
        std::filesystem::remove_all(path);
    }

    std::filesystem::copy(path, dest, std::filesystem::copy_options::recursive);

    std::cout << "Saved new boilerplate \"" << name << "\".\n";

    return 0;
}

int Boiler::load(const std::string& name, const std::filesystem::path path) const
{
    if (name.empty())
    {
        std::cout << "The specified name is empty.\n";

        return 1;
    }

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
        std::cout.flush();

        if (getchar() != 'y')
        {
            return 0;
        }
    }

    std::vector<std::filesystem::path> paths;

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        paths.push_back(entry.path());
    }

    for (const std::filesystem::path& path : paths)
    {
        std::filesystem::remove_all(path);
    }

    std::filesystem::copy(source, path, std::filesystem::copy_options::recursive);

    std::cout << "Loaded boilerplate \"" << name << "\".\n";

    return 0;
}

int Boiler::remove(const std::string& name) const
{
    if (name.empty())
    {
        std::cout << "The specified name is empty.\n";

        return 1;
    }

    const std::filesystem::path path = root / name;

    if (!std::filesystem::exists(path))
    {
        std::cout << "No boilerplate exists with the name \"" << name << "\".\n";

        return 1;
    }

    std::cout << "Are you sure you want to remove boilerplate \"" << name << "\"? [y/n] ";
    std::cout.flush();

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

std::filesystem::path Boiler::platformRoot() const
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

size_t Boiler::maxPath() const
{
    return MAX_PATH;
}

#elif __APPLE__

#include <limits.h>
#include <pwd.h>
#include <unistd.h>

std::filesystem::path Boiler::platformRoot() const
{
    struct passwd* pw = getpwuid(getuid());

    if (!pw)
    {
        throw std::system_error(std::error_code(errno, std::system_category()), "Fatal: Failed to locate user home directory.");
    }

    return std::filesystem::path(pw->pw_dir) / "Library" / "Boiler";
}

size_t Boiler::maxPath() const
{
    return PATH_MAX;
}

#else

#include <limits.h>
#include <pwd.h>
#include <unistd.h>

std::filesystem::path Boiler::platformRoot() const
{
    struct passwd* pw = getpwuid(getuid());

    if (!pw)
    {
        throw std::system_error(std::error_code(errno, std::system_category()), "Fatal: Failed to locate user home directory.");
    }

    return std::filesystem::path(pw->pw_dir) / ".boiler";
}

size_t Boiler::maxPath() const
{
    return PATH_MAX;
}

#endif

void Boiler::listFilesRecursive(const std::filesystem::path& path, const size_t depth) const
{
    std::vector<std::filesystem::path> paths;

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        paths.push_back(entry.path());
    }

    std::sort(paths.begin(), paths.end(), [](const std::filesystem::path& a, const std::filesystem::path& b)
    {
        if (std::filesystem::is_directory(a) == std::filesystem::is_directory(b))
        {
            return a.filename().string() < b.filename().string();
        }

        return std::filesystem::is_directory(a);
    });

    for (const std::filesystem::path& path : paths)
    {
        for (size_t i = 0; i < depth; i++)
        {
            std::cout << "|  ";
        }

        std::cout << path.filename().string() << "\n";

        if (std::filesystem::is_directory(path))
        {
            listFilesRecursive(path, depth + 1);
        }
    }
}
