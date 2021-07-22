/*
 *  libevdevxx - a C++ wrapper for libevdev
 *  Copyright (C) 2021  Daniel K. O.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INI_FILE_HPP
#define INI_FILE_HPP

#include <filesystem>
#include <iosfwd>
#include <map>
#include <optional>
#include <string>
#include <vector>


// minimalist .ini file parser

class IniFile {

public:


    struct Entry {
        std::string op, value, comment;
        unsigned line_num = 0;
    };


    IniFile();
    IniFile(const std::filesystem::path& filename);

    void load(const std::filesystem::path& filename);
    void load(std::istream& input);

    void save(const std::filesystem::path& filename, bool with_comments = true) const;
    void save(std::ostream& output, bool with_comments = true) const;

    std::vector<std::string> groups() const;

    bool has(const std::string& group) const;


    std::vector<std::string>
    keys(const std::string& group) const;

    const std::map<std::string, Entry>*
    group(const std::string& name) const;


    std::optional<std::string>
    value(const std::string& group,
          const std::string& key) const;

    bool has(const std::string& group,
             const std::string& key) const;

    std::optional<Entry>
    entry(const std::string& group,
          const std::string& key) const;


    std::string
    value_or(const std::string& group,
             const std::string& key,
             const std::string& def_value) const;

    void
    set_value(const std::string& group,
              const std::string& key,
              const std::string& value);


    std::string
    comment(const std::string& group) const;

    std::string
    comment(const std::string& group, const std::string& key) const;


private:

    std::map<std::string,
             std::map<std::string, Entry>> data;

};

#endif
