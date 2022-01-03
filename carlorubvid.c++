#include <SFML/Graphics.hpp>

#include <cstring>
#include <vector>
#include <iostream>

#include "CarloRubVid/button.hpp"

#include "CarloRubVid/init.hpp"
#include "CarloRubVid/loop.hpp"
#include "CarloRubVid/optional_loop.hpp"
#include "CarloRubVid/loader.hpp"
#include "CarloRubVid/tabbar.hpp"
#include "CarloRubVid/editing.hpp"

#define VERSION "1.0.0"

void main_gui(std::vector<std::string>& imported_filenames) {

    // Defining two windows: a main one, and an optional one with an undefined type (for example, whether the optional window is for creating a new or selecting a file)
    sf::RenderWindow window(sf::VideoMode(1500u, 900u), "Carlo Rub", sf::Style::Close);
    CarloRubVid::OptionalWindow optional_window;

    // std::vectors for all ressources during the application is running
    const std::vector<sf::Font> fonts         = CarloRubVid::load_fonts("../assets/fonts/fonts.txt");
    std::vector<sf::Image>      loaded_images = CarloRubVid::load_images(imported_filenames);

    // Cheap Loading screen
    {
        sf::Event e;
        while (window.pollEvent(e));
        window.clear(sf::Color(0x11, 0x11, 0x11));
        
        sf::Text txt_tmp("Loading, please wait ...", fonts[0]);
        txt_tmp.setPosition(750.0f - txt_tmp.getLocalBounds().width / 2, 450.0f - txt_tmp.getLocalBounds().height);
        window.draw(txt_tmp);

        window.display();
    }

    // Creating guis
    CarloRubVid::GUIContainer gui = CarloRubVid::create_gui(fonts[0], loaded_images, "../assets/imgs/", "../assets/musics/zemo.ogg");
    CarloRubVid::OptionalGUIContainer optional_gui = CarloRubVid::create_optional_gui(fonts[0], CarloRubVid::list_files("../images_in"), CarloRubVid::list_files("../assets/songs"));

    // Application loop
    while (window.isOpen()) {
        
        // Polling events from main window
        CarloRubVid::PolledEventSet events = CarloRubVid::poll_events(window, gui);

        // Updating main window
        CarloRubVid::update(events, gui, optional_gui, optional_window);

        // Drawing onto main window
        CarloRubVid::draw(window, gui, loaded_images);


        // Handle optional window

        // Setting optional window to active and main window to inactive, if the optional window is not of type 0 (0 = unspecified)
        if (optional_window.type_idx) {
            // Make second window active
            window.setActive(false);
            optional_window.window->setActive(true);
        }

        // Polling events from optional window
        CarloRubVid::OptionalWindow::poll_events(optional_window);

        // Updating optional window and loading new images if new were created or opened
        if (CarloRubVid::OptionalWindow::update(optional_window, gui, optional_gui, loaded_images, imported_filenames)) {
            gui.gui_edit_img = CarloRubVid::create_edit_img_gui(fonts[0], loaded_images, loaded_images.size() - 1);
        }

        // Drawing onto optional window
        CarloRubVid::OptionalWindow::draw(optional_window, optional_gui);
    }
}


inline void version() { std::cout << "CarloRubVid v." << VERSION << std::endl; }

inline void help(const char* app_name) {
    std::cout << "Usage: " << app_name << " [options] [files]\nOptions:\n\t--gui\t\t\tRuns the program in GUI-Mode (non-Terminal mode).\n\t--help\t\t\tPrints this text for"
    " you into the terminal.\n\t-i\t\t\tSets all names separated by space until one of them is 'done' as "
    "input files.\n\t--select-files\t\tDoes the same as '-i'\n\t--resize\t\tResizes the selected images until they fit into the bounds.\n\t\t\t\tExample: " << app_name <<
    " -i communist_tank.png wallpaper.jpg done --resize 400:200\n\t--version\t\tPrints name and version of this program.\n";
}


static std::pair<std::string, std::string> rsplit(const std::string& str, const char& sep) {
    std::pair<std::string, std::string> splitted = std::make_pair(std::string(), std::string());

    bool sep_met = false;
    for (register uint32_t i = str.length() - 1u; i < str.length(); i--) {
        if (str[i] == sep) { sep_met = true; continue; }

        if (sep_met) { std::get<0>(splitted) += str[i]; }
        else         { std::get<1>(splitted) += str[i]; }
    }

    std::reverse(std::get<0>(splitted).begin(), std::get<0>(splitted).end());
    std::reverse(std::get<1>(splitted).begin(), std::get<1>(splitted).end());

    return splitted;
}

static sf::Vector2u split2u(const std::string& str, const char& sep) {
    std::string s1, s2;

    bool sep_met = false;
    for (register uint32_t i = 0u; i < str.length(); i++) {
        if (str[i] == sep) { sep_met = true; continue; }

        if (sep_met) { s2 += str[i]; }
        else         { s1 += str[i]; }
    }

    return sf::Vector2u(std::stoul(s1), std::stoul(s2));
}


int main(int argc, char** argv) {
    if (argc > 1) {

        if (strcmp(argv[1], "--help") == 0) { help(argv[0]); return 0; }
        if (strcmp(argv[1], "--version") == 0) { version(); return 0; }

        std::vector<std::string> args, filepaths;
        args.reserve(argc);

        for (register uint32_t i = 0u; i < argc; i++) { args.emplace_back(argv[i]); }

        bool input_mode = false;
        bool resize_selection = false, resize = false;
        sf::Vector2u resize_new_size;
        std::vector<std::string> file_selection;

        for (register uint32_t i = 1u; i < argc; i++) {
            if (args[i] == "--gui") { main_gui(file_selection); return 0; }
            else if (args[i] == "--help") { std::cerr << argv[0] << ": Invalid context of option --help\nUse " << argv[0] << " --help for more information.\n"; return 1; }
            else if (args[i] == "--version") { std::cerr << argv[0] << ": Invalid context of option --version\nUse " << argv[0] << " --help for more information.\n"; return 1; }
            else if (args[i] == "-i" || args[i] == "--select-files") { input_mode = true; }
            else if (input_mode && args[i] == "done") { input_mode = false; }
            else if (args[i] == "done") { std::cerr << argv[0] << ": Invalid context of option 'done'\nUse " << argv[0] << " --help for more information.\n"; return 1; }
            else if (input_mode) { file_selection.push_back(args[i]); }
            else if (args[i] == "--resize") { resize = true; resize_selection = true; }
            else if (resize_selection) { resize_new_size = split2u(args[i], ':'); resize_selection = false; }
            else {
                std::cerr << argv[0] << ": Unrecognized option " << argv[i] << "\nUsage: " << argv[0] << " [OPTIONS]\nUse " << argv[0] << " --help for more information.\n";
                return 1;
            }
        }

        if (resize) {
            std::vector<sf::Image> imgs = CarloRubVid::load_images(file_selection);

            for (register uint32_t i = 0u; i < imgs.size(); i++) {
                CarloRubVid::resize(imgs[i], resize_new_size);
                imgs[i].saveToFile("CRV_output_" + std::to_string(i) + '.' + std::get<1>(rsplit(file_selection[i], '.')));
            }
        }

    } else {
        std::cerr << "Usage: " << argv[0] << " [OPTIONS]\nUse " << argv[0] << " --help for more information.\n";
    }
}