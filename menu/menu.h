#include "raylib.h"
#include <string>
#include <vector>

enum MenuOption {
    ORDER_CAKE = 1,
    FACTORY_SIMULATION,
    EXIT
};


class Item {
public:
    std::string name;
    int ret;

    Item() : name(""), ret(0) {}

    Item(std::string s1, int a) : name(s1), ret(a) {}

    void display(int x, int y) const;
    void disp_selected(int x, int y) const;
    Rectangle getBoundingBox(int x, int y) const;
};

inline void Item::display(int x, int y) const {
    DrawText(name.c_str(), x, y, 20, BLACK);
}

inline void Item::disp_selected(int x, int y) const {
    DrawText(name.c_str(), x, y, 20, RED);
}

inline Rectangle Item::getBoundingBox(int x, int y) const {
    // Calculate the bounding box of the item for mouse interaction
    return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(MeasureText(name.c_str(), 20)), 20};
}

class Menu {
    Item head;
    std::vector<Item> entries;
    int selected;
    int screenWidth = 800;
    int screenHeight = 600;


public:
    Menu() : selected(0),screenWidth(800),screenHeight(600) {}
    Menu(int screenWidth ,int screenHeight) : selected(0),screenWidth(screenWidth),screenHeight(screenHeight) {}

    void add(std::string, int);
    void menu_head(std::string);
    int display();
    void renderMenu() const;
    void updateResolution();
    int checkMouseHover() const;
};

inline void Menu::add(std::string s, int r = 0) {
    entries.emplace_back(s, (r == 0) ? entries.size() + 1 : r);
}

inline void Menu::menu_head(std::string s) {
    head.name = s;
}



inline void Menu::renderMenu() const {
    int i = 3;
    head.display(GetScreenWidth() / 2 - MeasureText(head.name.c_str(), 20) / 2, GetScreenHeight() / 20);
    for (const auto &entry : entries) {
        if (&entry == &entries[selected])
            entry.disp_selected(GetScreenWidth() / 2 - MeasureText(entry.name.c_str(), 20) / 2,
                                i * (GetScreenHeight() / 10));
        else
            entry.display(GetScreenWidth() / 2 - MeasureText(entry.name.c_str(), 20) / 2,
                          i * (GetScreenHeight() / 10));
        i++;
    }
}

inline void Menu::updateResolution() {
    // Check if window size has changed
    if (IsWindowResized()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
    }
}

inline int Menu::checkMouseHover() const {
    Vector2 mousePos = GetMousePosition();

    for (size_t i = 0; i < entries.size(); ++i) {
        Rectangle boundingBox = entries[i].getBoundingBox(GetScreenWidth() / 2 - MeasureText(entries[i].name.c_str(), 20) / 2,
                                                          (i + 3) * (GetScreenHeight() / 10));

        if (CheckCollisionPointRec(mousePos, boundingBox)) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

inline int Menu::display() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        updateResolution();
        renderMenu();

        int hoveredItem = checkMouseHover();
        if (hoveredItem != -1) {
            selected = hoveredItem;
        }

        EndDrawing();

        // Handle input
        switch (GetKeyPressed()) {
            case KEY_DOWN:
                selected = (selected + 1) % entries.size();
                break;
            case KEY_UP:
                selected = (selected + entries.size() - 1) % entries.size();
                break;
            case KEY_ENTER:
                return entries[selected].ret;
            
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
          return entries[selected].ret;
    }

    CloseWindow();
    return -1;
}