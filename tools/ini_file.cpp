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


#include <cctype>
#include <cstdio>
#include <fstream>
#include <istream>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "ini_file.hpp"


using std::filesystem::path;
using std::ifstream;
using std::istream;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::optional;
using std::ostream;
using std::out_of_range;
using std::runtime_error;
using std::string;
using std::vector;
using std::ws;
using std::map;

using namespace std::literals;


struct SyntaxError : runtime_error {
    SyntaxError(const string& msg) :
        runtime_error{"Syntax error: " + msg}
    {}
};


static
string
read_key(istream& input)
{
    input >> ws;

    string result;

    int c;
    while ((c = input.get()) != EOF) {
        if (!std::isalnum(c) && c != '_') {
            input.unget();
            break;
        }
        result += c;
    }

    if (result.empty())
        throw SyntaxError{"no identifier found."};

    input >> ws;

    return result;
}


static
string
read_assign(istream& input)
{
    input >> ws;

    int c = input.get();
    if (c == EOF)
        throw SyntaxError{"assignment not found."};

    if (c == '=')
        return "=";

    if (c != '-' || input.get() != '>')
        throw SyntaxError{"assignment should be '=' or '->'"};

    input >> ws;

    return "->";
}


IniFile::IniFile() = default;


IniFile::IniFile(const path& filename)
{
    load(filename);
}


void
IniFile::load(const path& filename)
{
    ifstream input{filename};
    try {
        load(input);
    }
    catch (runtime_error& e) {
        throw runtime_error{filename.string() + ":"s + string(e.what())};
    }
}


void
IniFile::load(istream& input)
{
    unsigned line_num = 0;
    string group;
    string line;
    string comment;

    try {

        while (getline(input, line)) {
            ++line_num;

            if (line.empty() || line.front() == '#') {
                // comment
                comment += line + '\n';
                continue;
            }

            if (line.front() == '[' && line.back() == ']') {
                // group
                group = line.substr(1, line.size() - 2);
                Entry e;
                e.comment = std::move(comment);
                comment.clear();
                e.line_num = line_num;
                data[group][""] = std::move(e);
                continue;
            }

            // otherwise, assume it's a key=value assignment
            istringstream line_in{line};
            Entry e;
            string key = read_key(line_in);
            e.op = read_assign(line_in);
            line_in >> ws;
            getline(line_in, e.value);
            e.comment = std::move(comment);
            comment.clear();
            e.line_num = line_num;
            data[group][key] = std::move(e);
        }
    }
    catch (std::exception& e) {
        throw runtime_error{std::to_string(line_num) + ": "s + string(e.what())};
    }
}



void
IniFile::save(const path& filename,
              bool with_comments) const
{
    ofstream output{filename};
    save(output, with_comments);
}


void
IniFile::save(ostream& output,
              bool with_comments) const
{
    for (auto& [group, dict] : data) {

        if (with_comments)
            output << comment(group);

        if (!group.empty())
            output << '[' << group << "]\n";

        for (auto& [key, entry] : dict) {

            if (key.empty())
                continue;

            if (with_comments)
                output << entry.comment;

            output << key
                   << " " << entry.op << " "
                   << entry.value << "\n";

        }
    }
}


vector<string>
IniFile::groups() const
{
    vector<string> result;
    for (const auto& i : data)
        result.push_back(i.first);
    return result;
}


bool
IniFile::has(const string& group) const
{
    return data.count(group);
}


vector<string>
IniFile::keys(const string& group) const
{
    vector<string> result;

    auto it = data.find(group);
    if (it != data.end())
        for (auto& item : it->second)
            result.push_back(item.first);

    return result;
}


const map<string, IniFile::Entry>*
IniFile::group(const string& name) const
{
    auto it = data.find(name);
    if (it == data.end())
        return nullptr;
    return &it->second;
}


optional<string>
IniFile::value(const string& group,
               const string& key) const
{
    try {
        return data.at(group).at(key).value;
    }
    catch (out_of_range&) {
        return {};
    }
}


optional<IniFile::Entry>
IniFile::entry(const string& group,
               const string& key) const
{
    try {
        return data.at(group).at(key);
    }
    catch (out_of_range&) {
        return {};
    }
}


string
IniFile::value_or(const string& group,
                  const string& key,
                  const string& def_value) const
{
    auto val = value(group, key);
    return val ? *val : def_value;
}


string
IniFile::comment(const string& group) const
{
    return comment(group, "");
}


string
IniFile::comment(const string& group,
                 const string& key) const
{
    try {
        return data.at(group).at(key).comment;
    }
    catch (out_of_range&) {
        return "";
    }
}
