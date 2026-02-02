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
    int add(const std::string& name, const std::filesystem::path path) const;
    int load(const std::string& name, const std::filesystem::path path) const;
    int remove(const std::string& name) const;

private:
    static std::filesystem::path platformRoot();

    static size_t maxPath();

    const std::filesystem::path root;

};
