#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

////// SET UP PRIMARY GLOBAL VARIABLES AND SETS

bool pterm;                    // TERMINATOR FOR MAIN POLLING
bool pterm_alt;                // TERMINATOR FOR ANY SECONDARY POLLINGS
bool err;                      // INDICATOR FOR POSSIBLE ERROR RETURN AT SELECTION MENUS
int opt;                       // STORES USER-CHOSEN OPTION AT SELECTION MENUS
std::string bopt;              // STORES USER-CHOSEN "Y/N" SELECTION
std::vector<int> opt_list;     // STORES THE LEGAL SELECTION OF OPTIONS AT A GIVEN PROMPT

int grid_size;                 // STORES THE SIZE OF THE BOARD           - 1 = NORMAL SIZE (7 x 7), 2 = LARGE SIZE (9 x 9)
int game_mode;                 // STORES THE GAMEMODE                    - 1 = COMPUTER MULTIPLAYER, 2 = VERSUS MULTIPLAYER
std::string player_a;          // STORES THE NAME OF PLAYER ONE
std::string player_b;          // STORES THE NAME OF PLAYER TWO
int marker_a;                  // STORES THE MARKER USED BY PLAYER ONE   - 1 = RED, 2 = YELLOW, 3 = BLUE, 4 = GREEN
int marker_b;                  // STORES THE MARKER USED BY PLAYER TWO   - 1 = RED, 2 = YELLOW, 3 = BLUE, 4 = GREEN

std::vector<std::vector<std::string>> a_grid;    // STORES THE ACTIVE GAME GRID

////// SET UP SPRITES AND GRAPHICAL ITEMS

// CLASS FOR MARKER ATTRIBUTE SETS
class AttrSet {
    public:
        std::vector<std::string> col;
        void set(int index, std::string n_item) { col[index] = n_item; }
};
        
// INDIVIDUAL MARKER ATTRIBUTES
AttrSet labels;
AttrSet tags;
labels.col = {"NULL", "red", "yellow", "blue", "green"};    // STORES THE NAMES OF THE MARKER TYPES
tags.col = {"[ ]", "[#]", "[$]", "[%]", "[&]"};             // STORES THE SYMBOLS WHICH REPRESENT EACH MARKER TYPE

// SIZED GRIDS
std::vector<std::vector<std::string>> ns_grid = {{"|A|", "|B|", "|C|", "|D|", "|E|", "|F|", "|G|"}, 
                                                 {"---", "---", "---", "---", "---", "---", "---"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"---", "---", "---", "---", "---", "---", "---"}, 
                                                 {"|=|", "   ", "   ", "|=|", "   ", "   ", "|=|"}};
std::vector<std::vector<std::string>> ls_grid = {{"|A|", "|B|", "|C|", "|D|", "|E|", "|F|", "|G|", "|H|", "|I|"}, 
                                                 {"---", "---", "---", "---", "---", "---", "---", "---", "---"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}, 
                                                 {"---", "---", "---", "---", "---", "---", "---", "---", "---"}, 
                                                 {"|=|", "   ", "|=|", "   ", "|=|", "   ", "|=|", "   ", "|=|"}};

// RENDERING FOR GAME BOARDS + ADDITIONAL CUSTOM SCREEN COMMANDS
void s_pause() { std::cin.get(); }                         // SHORTCUT FOR PAUSING THE SCREEN
void s_clear() { std::cout << "\033[2J\033[1;1H"; }        // SHORTCUT FOR CLEARING THE SCREEN

void ren_grid() {
    for (int i = 0; i < a_grid.size(); i++) {
        std::cout << "::::\t";
        for (int j = 0; j < a_grid[i].size(); j++) { std::cout << a_grid[i][j]; }
        std::cout << "\t::::" << std::endl;
    }
}

////// SET UP SCREENS

// SOME ADDITIONAL SELECTION COMMANDS
bool in_opt(int n) {
    bool qu = false;
    for (int i = 0; i < opt_list.size(); i++) { if (opt_list[i] == n) { qu = true; } }
    return qu;
}

void ren_s(int o) {
    err = false;
    opt_list = {};
    for (int i = 0; i < o; i++) { opt_list.push(i + 1); }
    s_clear();
}

void ch_err(int c) { if (!in_opt(c)) { err = true; } }

// SCREEN RENDERS
void print_titlescr() {
    ren_s(1);
    std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
    std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
    std::cout << "::::                                                                     ::::\n";
    std::cout << "::::      ___    ___                  _____   ___  _______               ::::\n";
    std::cout << "::::     /   \\  /   \\  |\\   | |\\   | |       /   \\    |       |   |      ::::\n";
    std::cout << "::::    |      |     | | \\  | | \\  | |_____ |         |       |   |      ::::\n";
    std::cout << "::::    |      |     | |  \\ | |  \\ | |      |         |       |___|__    ::::\n";
    std::cout << "::::     \\___/  \\___/  |   \\| |   \\| |_____  \\___/    |           |      ::::\n";
    std::cout << "::::    _____________________________________________________________    ::::\n";
    std::cout << "::::    _____________________________________________________________    ::::\n";
    std::cout << "::::                                                                     ::::\n";
    std::cout << "::::                                                                     ::::\n";
    std::cout << "::::                      Programmed by GalacticSand                     ::::\n";
    std::cout << "::::                (c) 2020 - Galaxius Computer Software                ::::\n";
    std::cout << "::::                                                                     ::::\n";
    std::cout << "::::                                                                     ::::\n";
    std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
    std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n"; 
    std::cout << "\n\n";
    std::cout << " -=-=-=-=  Press Enter to Start  =-=-=-=- \n";
    s_pause();
}

void print_menuscr() {
    ren_s(3);
    std::cout << "::::\n";
    std::cout << "::::\n";
    std::cout << "::::    === Connect 4 - Main Menu ===\n";
    std::cout << "::::\n";
    std::cout << "::::\n";
    std::cout << "::::    1. Begin Game \n";
    std::cout << "::::    2. Settings \n";
    std::cout << "::::    3. Quit \n";
    std::cout << "::::\n";
    std::cout << "::::\n";
    std::cout << "::::\n";
    std::cout << "::::\n";
    std::cout << "::::    Option: ";
    std::cin >> opt;
    ch_err(opt);
}

void print_gsetup(int scr) {
    switch(scr) {
      case 1 :
        ren_s(2);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Select Board Size \n";
        std::cout << "::::\n";
        std::cout << "::::    1. Normal (7 x 7)\n";
        std::cout << "::::    2. Large (9 x 9)\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Option: ";
        std::cin >> grid_size;
        ch_err(grid_size);
        break;
      case 2 :
        ren_s(2);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Select Gamemode \n";
        std::cout << "::::\n";
        std::cout << "::::    1. CPU Multiplayer\n";
        std::cout << "::::    2. Versus Multiplayer\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Option: ";
        std::cin >> gamemode;
        ch_err(gamemode);
        break;
      case 3 :
        ren_s(1);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Current Players \n";
        std::cout << "::::\n";
        std::cout << "::::    Player 1 - \n";
        std::cout << "::::    Player 2 - \n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Enter Player 1 Name: ";
        std::cin >> player_a;
        break;
      case 4 :
        ren_s(1);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Current Players \n";
        std::cout << "::::\n";
        std::cout << "::::    Player 1 - " << player_a << " \n";
        std::cout << "::::    Player 2 - \n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Enter Player 2 Name: ";
        std::cin >> player_b;
        break;
      case 5 :
        ren_s(1);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Current Players \n";
        std::cout << "::::\n";
        std::cout << "::::    Player 1 - " << player_a << " \n";
        std::cout << "::::    Player 2 - " << player_b << " \n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Continue with these settings? (Y/N) ";
        std::cin >> bopt;
        break;
      case 6 :
        ren_s(1);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Current Players \n";
        std::cout << "::::\n";
        std::cout << "::::    Player 1 - \n";
        std::cout << "::::    Player 2 - The Computer \n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Enter Player 1 Name: ";
        std::cin >> player_a;
        break;
      case 7 :
        ren_s(1);
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    === Begin Game ===\n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Current Players \n";
        std::cout << "::::\n";
        std::cout << "::::    Player 1 - " << player_a << " \n";
        std::cout << "::::    Player 2 - The Computer \n";
        std::cout << "::::\n";
        std::cout << "::::\n";
        std::cout << "::::    Continue with these settings? (Y/N) ";
        std::cin >> bopt;
        break;
    }
}

////// SET UP FUNCTIONS AND GAMEPLAY

////// MAIN SPACE WHERE GAME INITIALIZES AND PRIMARY POLLING TAKES PLACE

int main() {
    pterm = false;
    while (true) {
        if (pterm) { break; }
    }
}
