#include <SFML/Graphics.hpp>

#include <vector>

#ifndef CARLO_RUB_VID_LOADER
#define CARLO_RUB_VID_LOADER

namespace CarloRubVid {

    std::vector<std::string> list_files(const char* directory);
    
    std::vector<sf::Font>  load_fonts(const char*);
    std::vector<sf::Image> load_images(const std::vector<std::string>&);

} // namespace CarloRubVid

#endif // #ifndef CARLO_RUB_VID_LOADER