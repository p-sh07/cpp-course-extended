//
//  05.Производительность и оптимизация
//  Тема 3.Работа с файлами
//  Задача 08.Свой препроцессор с регулярными выражениями и сырыми литералами
//
//  Created by Pavel Sh on 11.01.2024.
//

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using std::filesystem::path;
namespace fs = std::filesystem;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

//returns empty path if file not found
path CheckPathsForFile(const string& filename, const vector<path>& include_directories) {
    for(const auto& search_path : include_directories) {
        path abs_path = search_path / filename;
        if(fs::exists(abs_path) && fs::is_regular_file(abs_path)) {
            return {abs_path};
        }
    }
    return {};
}

// напишите эту функцию
bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories) {
    ifstream input(in_file);
    if(!input) {
        return false;
    }
    ofstream output(out_file, ios::app);
    if(!output) {
        return false;
    }
    string line;
    size_t line_num = 1;
    bool success = true;
    smatch match_result;
    static regex include_local(R"/(\s*#\s*include\s*"([^"]*)"\s*)/");
    static regex include_lib(R"/(\s*#\s*include\s*<([^>]*)>\s*)/");
    
    while(getline(input,line)) {
        path include_file_path; //absolute path
        string filename; //relative path(name)
        
        if(regex_match(line, match_result, include_local)) { //"", search local dir
            filename = match_result[1].str();
            include_file_path = CheckPathsForFile(filename, {in_file.parent_path()});
        } else { //check for #include <>
            regex_match(line, match_result, include_lib);
        }
        //Matched with #include <> or file was not found in local dir
        if(!match_result.empty() && include_file_path.empty()) {
            filename = match_result[1].str();
            include_file_path = CheckPathsForFile(filename, include_directories);
        }

        if(match_result.empty()) { //It's a normal line
            output << line << endl;
        } else if(include_file_path.empty()) { //file not found error
            cout << "unknown include file " << filename
            << " at file "s << in_file.string()
            << " at line "s << line_num << endl;
            output.close();
            return false;
        } else { //matched & found, process the included files recurvively
            success = Preprocess(include_file_path, out_file, include_directories);
        }
        ++line_num;
    }
    output.close();
    return success;
}

string GetFileContents(string file) {
    ifstream stream(file);
    
    // конструируем string по двум итераторам
    return {(istreambuf_iterator<char>(stream)), istreambuf_iterator<char>()};
}

void Test() {
    error_code err;
    filesystem::remove_all("sources"_p, err);
    filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
    filesystem::create_directories("sources"_p / "include1"_p, err);
    filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);
    
    {
        ofstream file("sources/a.cpp");
        file << "// this comment before include\n"
        "#include \"dir1/b.h\"\n"
        "// text between b.h and c.h\n"
        "#include \"dir1/d.h\"\n"
        "\n"
        "int SayHello() {\n"
        "    cout << \"hello, world!\" << endl;\n"
        "#   include<dummy.txt>\n"
        "}\n"s;
    }
    {
        ofstream file("sources/dir1/b.h");
        file << "// text from b.h before include\n"
        "#include \"subdir/c.h\"\n"
        "// text from b.h after include"s;
    }
    {
        ofstream file("sources/dir1/subdir/c.h");
        file << "// text from c.h before include\n"
        "#include <std1.h>\n"
        "// text from c.h after include\n"s;
    }
    {
        ofstream file("sources/dir1/d.h");
        file << "// text from d.h before include\n"
        "#include \"lib/std2.h\"\n"
        "// text from d.h after include\n"s;
    }
    {
        ofstream file("sources/include1/std1.h");
        file << "// std1\n"s;
    }
    {
        ofstream file("sources/include2/lib/std2.h");
        file << "// std2\n"s;
    }
    
    assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
                        {"sources"_p / "include1"_p,"sources"_p / "include2"_p})));
    
    ostringstream test_out;
    test_out << "// this comment before include\n"
    "// text from b.h before include\n"
    "// text from c.h before include\n"
    "// std1\n"
    "// text from c.h after include\n"
    "// text from b.h after include\n"
    "// text between b.h and c.h\n"
    "// text from d.h before include\n"
    "// std2\n"
    "// text from d.h after include\n"
    "\n"
    "int SayHello() {\n"
    "    cout << \"hello, world!\" << endl;\n"s;
    
    assert(GetFileContents("sources/a.in"s) == test_out.str());
}

int main() {
    Test();
}

