#include "CarloRubVid/loader.hpp"

#include <set>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

std::vector<std::string> CarloRubVid::list_files(const char* directory) {
    std::string s("ls ");
    s += directory;
    s += " | sort > tmp";

    system(s.c_str());

    std::ifstream file_lst("tmp");

    s.clear();
    std::vector<std::string> files;

    while (std::getline(file_lst, s)) files.push_back(s);

    file_lst.close();
    system("rm tmp");

    return files;
}

std::vector<sf::Font> CarloRubVid::load_fonts(const char* filename) {

    std::ifstream file_font_paths(filename);
    std::vector<sf::Font> loaded_fonts;
    std::string font_filepath;

    if (!file_font_paths.is_open()) { std::cerr << "Couldn't open file for fonts: " << filename << std::endl; return loaded_fonts; }

    while (std::getline(file_font_paths, font_filepath)) {
        sf::Font font;
        font.loadFromFile(font_filepath);
        loaded_fonts.push_back(font);
    }

    return loaded_fonts;
}


std::vector<sf::Image> CarloRubVid::load_images(const std::vector<std::string>& paths) {

    std::vector<sf::Image> loaded_images;

    for (register uint32_t i = 0u; i < paths.size(); i++) {
        sf::Image img;
        img.loadFromFile(paths[i]);
        loaded_images.push_back(img);
    }

    return loaded_images;
}