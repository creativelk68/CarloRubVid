#include "CarloRubVid/optional_loop.hpp"

static bool update_optional_new_img(CarloRubVid::OptionalWindow&, CarloRubVid::OptionalGUIContainer&, std::vector<sf::Image>&, std::vector<std::string>&);
static bool update_optional_select_img(CarloRubVid::OptionalWindow&, CarloRubVid::OptionalGUIContainer&, std::vector<sf::Image>&, std::vector<std::string>&);
static bool update_optional_select_song(CarloRubVid::OptionalWindow&, CarloRubVid::OptionalGUIContainer&, CarloRubVid::GUIContainer::PracticeModeGUI&);

static void draw_optional_new_img(CarloRubVid::OptionalWindow&, const CarloRubVid::OptionalGUIContainer&);
static void draw_optional_select_img(CarloRubVid::OptionalWindow&, const CarloRubVid::OptionalGUIContainer&);
static void draw_optional_select_song(CarloRubVid::OptionalWindow&, const CarloRubVid::OptionalGUIContainer::SelectSongGUI&);


void CarloRubVid::OptionalWindow::poll_events(OptionalWindow& optional_window) {

    if (optional_window.type_idx == 0u) return;

    for (sf::Event e; optional_window.window->pollEvent(e);) {
        switch (e.type) {
            case sf::Event::Closed:
                optional_window.window->close();
                delete optional_window.window;
                optional_window.window = nullptr;
                optional_window.type_idx = 0u;
                return;
        
            default: break;
        }
    }
}


bool CarloRubVid::OptionalWindow::update(OptionalWindow& optional_window, CarloRubVid::GUIContainer& gui, CarloRubVid::OptionalGUIContainer& optional_gui,
std::vector<sf::Image>& imgs, std::vector<std::string>& args) {

    switch (optional_window.type_idx) {
        case 0u: return false;
        case 1u: return update_optional_new_img(optional_window, optional_gui, imgs, args);
        case 2u: return update_optional_select_img(optional_window, optional_gui, imgs, args);
        case 3u: return update_optional_select_song(optional_window, optional_gui, gui.gui_practice_mode);
    
        default: return false;
    }   
}


void CarloRubVid::OptionalWindow::draw(OptionalWindow& optional_window, const OptionalGUIContainer& guis) {

    switch (optional_window.type_idx) {
        case 0u: return;
        case 1u: draw_optional_new_img(optional_window, guis); return;
        case 2u: draw_optional_select_img(optional_window, guis); return;
        case 3u: draw_optional_select_song(optional_window, guis.gui_select_song); return;
    
        default: return;
    }
}



static bool update_optional_new_img(CarloRubVid::OptionalWindow& optional_window, CarloRubVid::OptionalGUIContainer& guis, std::vector<sf::Image>& imgs,
std::vector<std::string>& args) {

    guis.gui_new_img.button_down_width.set_hovered( guis.gui_new_img.button_down_width.is_hovered( sf::Mouse::getPosition(*optional_window.window)));
    guis.gui_new_img.button_up_width.set_hovered(   guis.gui_new_img.button_up_width.is_hovered(   sf::Mouse::getPosition(*optional_window.window)));
    guis.gui_new_img.button_down_height.set_hovered(guis.gui_new_img.button_down_height.is_hovered(sf::Mouse::getPosition(*optional_window.window)));
    guis.gui_new_img.button_up_height.set_hovered(  guis.gui_new_img.button_up_height.is_hovered(  sf::Mouse::getPosition(*optional_window.window)));
    guis.gui_new_img.button_confirm.set_hovered(    guis.gui_new_img.button_confirm.is_hovered(    sf::Mouse::getPosition(*optional_window.window)));

    static bool did_click = false;

    if (did_click == false && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        did_click = true;

        bool clicked = false;

        if (guis.gui_new_img.button_down_width.is_hovered() && guis.gui_new_img.width > 0u) {
            guis.gui_new_img.width--;
            clicked = true;
        }

        if (guis.gui_new_img.button_up_width.is_hovered()) {
            guis.gui_new_img.width++;
            clicked = true;
        }

        if (guis.gui_new_img.button_down_height.is_hovered() && guis.gui_new_img.height > 0u) {
            guis.gui_new_img.height--;
            clicked = true;
        }

        if (guis.gui_new_img.button_up_height.is_hovered()) {
            guis.gui_new_img.height++;
            clicked = true;
        }

        if (guis.gui_new_img.button_confirm.is_hovered() && guis.gui_new_img.width > 0u && guis.gui_new_img.height > 0u) {
            sf::RenderTexture rtexture_new_img;
            rtexture_new_img.create(guis.gui_new_img.width, guis.gui_new_img.height);
            rtexture_new_img.clear(sf::Color::Transparent);
            rtexture_new_img.display();
            imgs.push_back(rtexture_new_img.getTexture().copyToImage());
            imgs[imgs.size() - 1u].saveToFile("Untitled image.png");
            args.emplace_back("Untitled image.png");

            optional_window.window->close();
            delete optional_window.window;
            optional_window.type_idx = 0u;
            optional_window.window = nullptr;
            return true;
        }

        if (clicked) {
            guis.gui_new_img.txt_width.setString("Width: " + std::to_string(guis.gui_new_img.width) + "px");
            guis.gui_new_img.button_down_width.set_position(guis.gui_new_img.txt_width.getLocalBounds().width + 20, 60);
            guis.gui_new_img.button_up_width.set_position(  guis.gui_new_img.txt_width.getLocalBounds().width + 20, 35);

            guis.gui_new_img.txt_height.setString("Height: " + std::to_string(guis.gui_new_img.height) + "px");
            guis.gui_new_img.button_down_height.set_position(guis.gui_new_img.txt_height.getLocalBounds().width + 20, 110);
            guis.gui_new_img.button_up_height.set_position(  guis.gui_new_img.txt_height.getLocalBounds().width + 20, 85);
        }
    }

    if (did_click && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) did_click = false;

    return false;
}


static bool update_optional_select_img(CarloRubVid::OptionalWindow& optional_window, CarloRubVid::OptionalGUIContainer& guis, std::vector<sf::Image>& imgs,
std::vector<std::string>& args) {

    // Check each button with the filename if it is hovered
    for (register uint32_t i = 0u; i < guis.gui_select_img.listed_files.size(); i++) {
        guis.gui_select_img.listed_files[i].set_hovered(guis.gui_select_img.listed_files[i].is_hovered(sf::Mouse::getPosition(*optional_window.window)));
    }

    // Reacting if one of the buttons with the filenames is clicked
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for (register uint32_t i = 0u; i < guis.gui_select_img.listed_files.size(); i++) {
            if (guis.gui_select_img.listed_files[i].is_hovered()) {
                std::string s("../images_in/");
                s += guis.gui_select_img.listed_files[i].get_text();

                // Check if s already is in args
                bool match = false;
                for (register uint32_t j = 0u; j < args.size(); j++) {
                    if (s == args[j]) {
                        match = true;
                        break;
                    }
                }

                if (match) return false;

                args.push_back(s);
                
                sf::Image new_img;
                new_img.loadFromFile(s);
                imgs.push_back(new_img);

                return true;
            }
        }
    }
    return false;
}

static void replace_str(std::string& str, const char& targeted, const char& replaced) {
    for (register uint32_t i = 0u; i < str.length(); i++) {
        str[i] = std::tolower(str[i]);
        if (str[i] == targeted) str[i] = replaced;
    }
}

static bool update_optional_select_song(CarloRubVid::OptionalWindow& optional_window, CarloRubVid::OptionalGUIContainer& optional_gui,
CarloRubVid::GUIContainer::PracticeModeGUI& gui) {

    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(*optional_window.window);

    for (register uint32_t i = 0u; i < optional_gui.gui_select_song.listed_songs.size(); i++) {
        optional_gui.gui_select_song.listed_songs[i].set_hovered(optional_gui.gui_select_song.listed_songs[i].is_hovered(mouse_pos));

        if (optional_gui.gui_select_song.listed_songs[i].is_hovered() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            gui.txt.setString(optional_gui.gui_select_song.listed_songs[i].get_text());
            gui.txt.setPosition(750.0f - gui.txt.getLocalBounds().width / 2, 450.0f - gui.txt.getLocalBounds().height / 2);

            std::string songname = gui.txt.getString();
            std::string filename = songname;
            replace_str(filename, ' ', '_');

            gui.song.set_song("../assets/songs/" + songname + '/' + filename);
            gui.song.play();

            optional_window.type_idx = 0u;
            optional_window.window->close();
            delete optional_window.window;
            optional_window.window = nullptr;
            optional_gui.type_idx = 0u;
            return false;
        }
    }

    return false;
}


static void draw_optional_new_img(CarloRubVid::OptionalWindow& optional_window, const CarloRubVid::OptionalGUIContainer& guis) {

    optional_window.window->clear(sf::Color(0x11, 0x11, 0x11));

    optional_window.window->draw(guis.gui_new_img.txt_width);
    optional_window.window->draw(guis.gui_new_img.button_down_width);
    optional_window.window->draw(guis.gui_new_img.button_up_width);
    optional_window.window->draw(guis.gui_new_img.txt_height);
    optional_window.window->draw(guis.gui_new_img.button_down_height);
    optional_window.window->draw(guis.gui_new_img.button_up_height);
    optional_window.window->draw(guis.gui_new_img.txt_demo);
    optional_window.window->draw(guis.gui_new_img.demo);
    optional_window.window->draw(guis.gui_new_img.button_confirm);

    optional_window.window->display();
}


static void draw_optional_select_img(CarloRubVid::OptionalWindow& optional_window, const CarloRubVid::OptionalGUIContainer& gui) {

    optional_window.window->clear(sf::Color(0x11, 0x11, 0x11));

    // Drawing lines between the entries
    optional_window.window->draw(gui.gui_select_img.vertices_lines.data(), gui.gui_select_img.vertices_lines.size(), sf::PrimitiveType::Lines);

    // Drawing the filename of each entry
    for (register uint32_t i = 0u; i < gui.gui_select_img.listed_files.size(); i++) { optional_window.window->draw(gui.gui_select_img.listed_files[i]); }

    optional_window.window->display();
}


static void draw_optional_select_song(CarloRubVid::OptionalWindow& optional_window, const CarloRubVid::OptionalGUIContainer::SelectSongGUI& gui) {
    optional_window.window->clear(sf::Color(0x11, 0x11, 0x11));

    optional_window.window->draw(gui.vertices_lines.data(), gui.vertices_lines.size(), sf::PrimitiveType::Lines);

    for (register uint32_t i = 0u; i < gui.listed_songs.size(); i++) { optional_window.window->draw(gui.listed_songs[i]); }

    optional_window.window->display();
}