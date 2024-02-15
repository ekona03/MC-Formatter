#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cctype>

using namespace std;

class formatter {
    public:
        int select_val;
        string item_name_main;
        string item_name_recipe;
        string item_name;
        string input_string;
        string substring;
        size_t pos;
        size_t pos_end;
        size_t pos_null;
        vector<string> ingredient;

        void ct_recipe();
        void print_selection();
        void ct_print(string, string, string, const vector<string>&);
        string get_main_name(const string&);
        string get_recipe_name(const string&);
        bool failsafe(const string&);
};


/**
 * @brief standard input of ct generated string. 
 * iterates through the string and stores each item within <> and each null to a vector string.
 * 
 */
void formatter::ct_recipe() {
    item_name.clear();
    pos = input_string.find('<');
    pos_null = input_string.find("null");

    while (pos != string::npos || pos_null != string::npos) {
        if (pos_null != string::npos && (pos == string::npos || pos_null < pos)) {
            ingredient.push_back("null");
            pos_null = input_string.find("null", pos_null + 4);
        } else if (pos != string::npos) {
            pos_end = input_string.find('>', pos);
            if (pos_end != string::npos) {
                substring = input_string.substr(pos, pos_end - pos + 1);
                if (item_name.empty()) {
                    item_name = substring;
                } else {
                    ingredient.push_back(substring);
                }
                pos = input_string.find('<', pos_end);
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
    item_name_main = get_main_name(item_name);
    item_name_recipe = get_recipe_name(item_name);
}


/**
 * @brief Get the main name object
 * 
 * @param item_name item that the recipe is crafting
 * @return string default name
 */
string formatter::get_main_name(const string& item_name) {
    item_name_main = "";
    size_t colon_pos = item_name.find(':');
    if (colon_pos == string::npos) {
        return "";
    }

    item_name_main = item_name.substr(colon_pos + 1, item_name.size() - colon_pos - 2);

    for (size_t i = 0; i < item_name_main.length(); ++i) {
        if (item_name_main[i] == '_') {
            item_name_main[i] = ' ';
        }
    }

    bool capitalize_next = true;
    for (size_t i = 0; i < item_name_main.length(); ++i) {
        if (capitalize_next && islower(item_name_main[i])) {
            item_name_main[i] = toupper(item_name_main[i]);
            capitalize_next = false;
        } else if (isspace(item_name_main[i])) {
            capitalize_next = true;
        }
    }
    return item_name_main;
}


/**
 * @brief Get the recipe name object
 * 
 * @param item_name item that the recipe is crafting
 * @return string unique recipe identifier name
 */
string formatter::get_recipe_name(const string& item_name) {
    item_name_recipe = "";
    for (size_t i = 1; i < item_name.size() - 1; ++i) {
        char ch = item_name[i];
        if (ch == ':' || ch == '_') {
            continue;
        }
        item_name_recipe += ch;
    }

    bool capitalize_next = true;
    for (char& ch : item_name_recipe) {
        if (capitalize_next && islower(ch)) {
            ch = toupper(ch);
            capitalize_next = false;
        } else if (isspace(ch)) {
            capitalize_next = true;
        }
    }
    return item_name_recipe;
}


/**
 * @brief prints the formatted string
 * 
 * @param item_name registry name
 * @param item_name_main default name
 * @param item_name_recipe unique recipe identifier name
 * @param ingredient item used in recipe
 */
void formatter::ct_print(string item_name, string item_name_main, string item_name_recipe, const vector<string>& ingredient) {
    cout << "\n<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>\n\n"
         << setfill(' ') << "#" << item_name_main << "\n"
         << setw(4) << " " << "recipes.remove(" << item_name << ");\n"
         << setw(4) << " " << "recipes.addShaped(\"" << item_name_recipe << "\",\n"
         << setw(8) << " " << item_name << ",\n"
         << setw(12) << " " << "[[" << ingredient[0] << ", " << ingredient[1] << ", " << ingredient[2] << "],\n"
         << setw(12) << " " << "[" << ingredient[3] << ", " << ingredient[4] << ", " << ingredient[5] << "],\n"
         << setw(12) << " " << "[" << ingredient[6] << ", " << ingredient[7] << ", " << ingredient[8] << "]]);\n"
         << "\n<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>\n";
}

/**
 * @brief prints the recipe selection
 * 
 */
void formatter::print_selection() {
    cout << "\n------------------------------\n"
         << "CT Formatter\n"
         << "------------------------------\n"
         << "[1] CT Shaped Table Recipe\n"
         // << "[2] Extended Crafting 3x3\n"
         // << "[3] Extended Crafting 5x5\n"
         // << "[4] Extended Crafting 7x7\n"
         // << "[5] Extended Crafting 9x9\n"
         << "------------------------------\n\n"
         << "Select Recipe Type: ";

    while (true) {
        cin >> select_val;
        cin.ignore();
        if (select_val == 1) {
            break;
        } else {
            cout << "Incorrect value, Select Recipe Type: ";
            select_val = 0;
        }
    }
}

/**
 * @brief failsafe for the input string. makes sure the program doesnt shit bricks when someone inputs in the wrong thing
 * 
 * @param input_string standard input string
 * @return true if number of "<" in the input string is at least 6
 * @return false if number of "<" in the input string is less than 6
 */
bool formatter::failsafe(const string& input_string) {
    int count = 0;
    for (char ch : input_string) {
        if (ch == '<') {
            count++;
        }
    }
    return count >= 6;
}

/**
 * @brief standard input for recipe type selection.
 * 
 */
int main() {
    formatter obj;
    obj.print_selection();
    switch (obj.select_val) {
        case 1:
            while (true) {
                cout << "\nPaste in unformatted recipe below, type 'exit' to end program\n"
                     << "↓---↓---↓---↓---↓---↓---↓---↓---↓---↓---↓---↓---↓---↓---↓---↓\n\n";
                getline(cin, obj.input_string);
                if (obj.input_string == "exit") {
                    obj.ingredient.clear();
                    break;
                }
                if (obj.failsafe(obj.input_string)) {
                    obj.ct_recipe();
                    obj.ct_print(obj.item_name, obj.item_name_main, obj.item_name_recipe, obj.ingredient);
                    obj.ingredient.clear();
                } else {
                    cout << "\n----------------------- Invalid Input -----------------------\n";
                }
            }
            break;
        default:
            break;
    }
    return 0;
}