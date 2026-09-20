#include <filesystem>
#include <string>
#include <iostream>
#include <vector>

#include "ast.h"
#include "parser.h"
#include "translator.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {

    // Set target language (Default to C#)
    std::string targetLanguage;
    if (argc > 1) targetLanguage = argv[1];

    // Set scan directory (Default to unit test directory)
    fs::path scanDir = fs::path(SPLASHKIT_TESTS) / "unit_tests";
    if (argc > 2) scanDir = argv[2];

    std::cout << "Scanning directory: " << scanDir << "\n\n";
 
    // Check the chosen directory exists, and is actually a directory
    std::error_code ec;
    if (!fs::exists(scanDir, ec) || !fs::is_directory(scanDir, ec))
    {
        std::cerr << "Error: Directory does not exist or is inaccessible: " << scanDir << "\n";
        return 1;
    }
 
    // Add every .cpp file in the directory 
    std::vector<std::string> cppFiles;
    for (const auto& entry : fs::directory_iterator(scanDir))
    {
        if (entry.path().extension() == ".cpp")
        {
            std::string stem = entry.path().stem().string();                       // Filename without extension
            if (stem == "unit_test_main" || stem == "logging_handling") continue;  // Skip "main" file
            if (
                // stem != "unit_test_color"
                // && stem != "unit_test_music"
                // && stem != "unit_test_json"
                // && stem != "unit_test_triangle"
                // && stem != "unit_test_graphics"
                // && stem != "unit_test_raspi_gpio"
                stem != "unit_test_text"
                // && stem != "unit_test_web_server"
                // && stem != "unit_test_bitmap"
                // && stem != "unit_test_sound_effect"
                // && stem != "unit_test_sprites"
                // && stem != "unit_test_utilities"
                // && stem != "unit_test_network"
                // && stem != "unit_test_geometry"
            ) continue;

            cppFiles.push_back(entry.path());                          // Add file to list
        }
    }
 
    // No valid files found
    if (cppFiles.empty())
    {
        std::cerr << "No .cpp files found in " << scanDir << "\n";
        return 1;
    }
 
    std::cout << "Found " << cppFiles.size() << " .cpp files\n\n";

    // Parse files
    fs::path debugOutputDir = fs::path(SPLASHKIT_TESTS) / "unit_tests" / "generated" / "json";
    std::vector<CustomAST> *allASTs = parseTestFiles(cppFiles, debugOutputDir);
    
    // Translate the files to the target language
    std::unique_ptr<Translator> translator;

    // C#
    if (targetLanguage == "cs" || targetLanguage == "csharp" || targetLanguage == "C#")
    {
        translator = std::make_unique<CSharpTranslator>();
    }
    // Default to C#
    else
    {
        std::cout << "Target language defaulted to C#" << std::endl;
        translator = std::make_unique<CSharpTranslator>();
    }

    fs::path outputDir = fs::path(SPLASHKIT_TESTS) / "unit_tests" / "generated" / "csharp";
    translator->translateASTSet(*allASTs, outputDir);
    std::cout << "Translated " << cppFiles.size() << " C++ files\n\n";
 
    return 0;
}
