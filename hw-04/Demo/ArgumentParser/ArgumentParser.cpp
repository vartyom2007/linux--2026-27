#include <iostream>
#include "ArgumentParser.h"
int main(int argc, char* argv[]){
    ArgumentParser parser;

    parser.addArgument("--name");
    parser.addArgument("--age");
    parser.addArgument("--city");

    parser.addFlag("--verbose");
    parser.addFlag("--debug");

    parser.parse(argc, argv);

    if (parser.has("--name"))
    {
        std::cout << "Name: "
                  << parser.get("--name")
                  << std::endl;
    }

    if (parser.has("--age"))
    {
        std::cout << "Age: "
                  << parser.get("--age")
                  << std::endl;
    }

    if (parser.has("--city"))
    {
        std::cout << "City: "
                  << parser.get("--city")
                  << std::endl;
    }

    if (parser.has("--verbose"))
    {
        std::cout << "Verbose: true"
                  << std::endl;
    }

    if (parser.has("--debug"))
    {
        std::cout << "Debug: true"
                  << std::endl;
    }

    return 0;
}

