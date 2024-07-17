#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "json_reader.h"
#include "request_handler.h"

using namespace std;
namespace fs = std::filesystem;

int main() {
    
    TransportDb database;
    MapRenderer map_renderer;
    BusRouter router(database);
    RequestHandler request_handler(database, map_renderer, router);
    JsonReader jreader(database, request_handler);
//    
    auto& in = std::cin;
    auto& out = std::cout;
//    fs::path in_file = "/Users/ps/Documents/cpp-course/input_files/bus_sprint12_2_in.json"s;
//    ifstream in(in_file);
    
//    std::string out_filename = in_file.stem().string();
//    out_filename += "_tc_output.json";
//    std::ofstream out (in_file.replace_filename(out_filename), std::ios_base::out);
    
    jreader.ParseInput(in);
    jreader.ProcessStatRequests();
    jreader.PrintRequestAnswers(out);
    
    CERR << "Finished!" << endl;
}
