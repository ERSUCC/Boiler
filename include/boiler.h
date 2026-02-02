#pragma once

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <system_error>
#include <vector>

struct Boiler
{
    Boiler();

    int list() const;
    int inspect(const std::string& name) const;
    int add(const std::string& name, const std::filesystem::path path) const;
    int load(const std::string& name, const std::filesystem::path path) const;
    int remove(const std::string& name) const;

private:
    std::filesystem::path platformRoot() const;

    size_t maxPath() const;

    void listFilesRecursive(const std::filesystem::path& path, const size_t depth) const;

    const std::filesystem::path root;

};
