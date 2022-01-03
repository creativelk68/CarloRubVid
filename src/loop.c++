#include "CarloRubVid/loop.hpp"
#include "CarloRubVid/tabbar.hpp"
#include "CarloRubVid/loader.hpp"

#include <iostream>

static void update_main_menu(CarloRubVid::PolledEventSet&, CarloRubVid::GUIContainer&, CarloRubVid::OptionalWindow&, CarloRubVid::OptionalGUIContainer&);
static void update_edit_img(CarloRubVid::PolledEventSet&, CarloRubVid::GUIContainer&, CarloRubVid::OptionalGUIContainer&, CarloRubVid::OptionalWindow&);
static void update_practice_mode(CarloRubVid::GUIContainer::PracticeModeGUI&);

static void draw_main_menu(sf::RenderWindow&, const CarloRubVid::GUIContainer::MainMenuGUI&);
static void draw_edit_img(sf::RenderWindow&, const CarloRubVid::GUIContainer::EditImgGUI&, const std::vector<sf::Image>&);
static void draw_practice_beat(sf::RenderWindow&, const CarloRubVid::GUIContainer::PracticeModeGUI&);


CarloRubVid::PolledEventSet CarloRubVid::poll_events(sf::RenderWindow& window, CarloRubVid::GUIContainer& gui) {

    CarloRubVid::PolledEventSet polled_events = { false, sf::Vector2i() };

    for (sf::Event e; window.pollEvent(e);) {
        switch (e.type) {
            case sf::Event::Closed: window.close(); return polled_events;
            case sf::Event::MouseMoved: polled_events.mouse_moved = true; polled_events.new_mouse_pos = sf::Mouse::getPosition(window); break;
            case sf::Event::KeyPressed: if (e.key.code == sf::Keyboard::Escape && gui.type_idx == 2u) {
                gui.type_idx = 1u;
                gui.gui_practice_mode.txt.setString("Select a song to practice");
                gui.gui_practice_mode.txt.setPosition(750.0f - gui.gui_practice_mode.txt.getLocalBounds().width / 2,
                                                      450.0f - gui.gui_practice_mode.txt.getLocalBounds().height / 2
                );
                gui.gui_practice_mode.song.destroy();
                return polled_events;
                } break;
            
            default: break;
        }
    }
    
    return polled_events; 
}

void CarloRubVid::update(PolledEventSet& events, GUIContainer& gui, OptionalGUIContainer& optional_gui, OptionalWindow& optional_window) {

    switch (gui.type_idx) {
        case 0u: update_main_menu(events, gui, optional_window, optional_gui); return;
        case 1u: update_edit_img(events, gui, optional_gui, optional_window); return;
        case 2u: update_practice_mode(gui.gui_practice_mode); return;
    
        default: return;
    }
}


void CarloRubVid::draw(sf::RenderWindow& window, const CarloRubVid::GUIContainer& gui, const std::vector<sf::Image>& imgs) {
    switch (gui.type_idx) {
        case 0u: draw_main_menu(window, gui.gui_main_menu); return;
        case 1u: draw_edit_img(window, gui.gui_edit_img, imgs); return;
        case 2u: draw_practice_beat(window, gui.gui_practice_mode); return;
    
        default: return;
    }
}


static void update_main_menu(CarloRubVid::PolledEventSet& events, CarloRubVid::GUIContainer& gui, CarloRubVid::OptionalWindow& optional_window,
    CarloRubVid::OptionalGUIContainer& optional_gui) {
    gui.gui_main_menu.video->update();

    if (events.mouse_moved) {
        gui.gui_main_menu.button_edit_img.set_hovered(gui.gui_main_menu.button_edit_img.is_hovered(events.new_mouse_pos));
        gui.gui_main_menu.button_practice_beat.set_hovered(gui.gui_main_menu.button_practice_beat.is_hovered(events.new_mouse_pos));
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (gui.gui_main_menu.button_edit_img.is_hovered()) {
            gui.type_idx = 1u;
            delete gui.gui_main_menu.video;
        }
        else if (gui.gui_main_menu.button_practice_beat.is_hovered()) {
            gui.type_idx = 2u;
            delete gui.gui_main_menu.video;

            optional_window.type_idx = 3u;
            optional_window.window = new sf::RenderWindow(sf::VideoMode(700u, 900u), "Select a song to practice", sf::Style::Close);
            optional_gui.type_idx = 3u;
        }
    }
}


static void update_edit_img(CarloRubVid::PolledEventSet& events, CarloRubVid::GUIContainer& gui, CarloRubVid::OptionalGUIContainer& optional_gui,
CarloRubVid::OptionalWindow& optional_window) {

    constexpr time_t target_date = 1646262000u;

    gui.gui_edit_img.txt_days_rem.setString(std::to_string((target_date - time(nullptr)) / 86400) + " days remaining");
    gui.gui_edit_img.txt_days_rem.setPosition(1495.0f - gui.gui_edit_img.txt_days_rem.getLocalBounds().width, .0f);

    if (events.mouse_moved) {
        gui.gui_edit_img.button_import_imgs.set_hovered(gui.gui_edit_img.button_import_imgs.is_hovered(events.new_mouse_pos));
        gui.gui_edit_img.button_new_img.set_hovered(gui.gui_edit_img.button_new_img.is_hovered(events.new_mouse_pos));
        gui.gui_edit_img.button_practice_mode.set_hovered(gui.gui_edit_img.button_practice_mode.is_hovered(events.new_mouse_pos));
    }

    gui.gui_edit_img.tabbar.update(events, gui.gui_edit_img.open_image_idx);

    if (optional_window.window == nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (gui.gui_edit_img.button_new_img.is_hovered()) {
            optional_window.type_idx = 1u;
            optional_window.window = new sf::RenderWindow(sf::VideoMode(630u, 470u), "Create a new image", sf::Style::Close);
            optional_gui.type_idx = 1u;
        } else if (gui.gui_edit_img.button_import_imgs.is_hovered()) {
            optional_window.type_idx = 2u;
            optional_window.window = new sf::RenderWindow(sf::VideoMode(700u, 900u), "Select an image to edit", sf::Style::Close);
            optional_gui.type_idx = 2u;
        } else if (gui.gui_edit_img.button_practice_mode.is_hovered()) {
            gui.type_idx = 2u;
            optional_window.type_idx = 3u;
            optional_window.window = new sf::RenderWindow(sf::VideoMode(700u, 900u), "Select a song to practice", sf::Style::Close);
            return;
        }
    }   
}

static void update_practice_mode(CarloRubVid::GUIContainer::PracticeModeGUI& gui) {
    gui.song.add_beat_box(gui.beat_boxes);
}


static void draw_main_menu(sf::RenderWindow& window, const CarloRubVid::GUIContainer::MainMenuGUI& gui) {

    window.clear();
    window.draw(*gui.video);
    window.draw(gui.button_edit_img);
    window.draw(gui.button_practice_beat);
    window.display();
}


static void draw_edit_img(sf::RenderWindow& window, const CarloRubVid::GUIContainer::EditImgGUI& gui, const std::vector<sf::Image>& imgs) {
    sf::Texture bg_texture;
    bg_texture.loadFromImage(gui.bg_image);

    sf::Sprite bg(bg_texture);
    bg.setOrigin(365.0f, 365.0f);
    bg.setPosition(750.0f, 545.0f);

    sf::Texture texture_coordinate_system;
    if (!texture_coordinate_system.loadFromImage(gui.coordinate_system)) {
        std::cerr << "Couldn't load coordinate system!\n";
    }

    sf::Sprite coordinate_system(texture_coordinate_system);
    coordinate_system.setPosition(291.0f, 130.0f);

    sf::Texture texture;
    sf::Sprite sprite;
    if (imgs.size() > 0u && imgs[gui.open_image_idx].getSize().x > 0 && texture.loadFromImage(imgs[gui.open_image_idx])) {
        sprite.setTexture(texture);
        sprite.setPosition(bg.getGlobalBounds().left, bg.getGlobalBounds().top);
        sprite.setScale(700.0f / texture.getSize().x, 700.0f / texture.getSize().y);
    }

    window.clear(sf::Color(0x11, 0x11, 0x11));

    window.draw(coordinate_system);

    window.draw(bg);

    if (texture.getSize().x > 0) window.draw(sprite);

    window.draw(gui.tabbar);
    window.draw(gui.toolbar);
    window.draw(gui.button_new_img);
    window.draw(gui.button_import_imgs);
    window.draw(gui.button_practice_mode);
    window.draw(gui.txt_days_rem);
    
    window.display();
}

static void draw_practice_beat(sf::RenderWindow& window, const CarloRubVid::GUIContainer::PracticeModeGUI& gui) {
    window.clear(sf::Color(0x11, 0x11, 0x11));
    window.draw(gui.txt);
    
    for (register uint32_t i = 0u; i < gui.beat_boxes.size(); i++) { window.draw(gui.beat_boxes[i]); }
    
    window.display();
}