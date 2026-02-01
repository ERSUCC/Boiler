#pragma once

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <system_error>
#include <vector>

struct Boiler
{
    Boiler();

    int list() const;
    int add(const char* name, const std::filesystem::path path) const;
    int load(const char* name, const std::filesystem::path path) const;
    int remove(const char* name) const;

private:
    static std::filesystem::path platformRoot();

    const std::filesystem::path root;

};
