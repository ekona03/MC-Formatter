#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class formatter {
    public:
        int select_val;
        const string shaped = "recipes.addShaped";
        const string shapeless = "recipes.addShapeless";
        string input_string;
        string substring;
        string craft_amount;
        size_t pos;
        size_t pos_end;
        size_t pos_null;
        size_t pos_asterisk;
        size_t pos_asterisk_end;
        vector<string> ingredient;

        void ct_recipe();
        void ct_amount();
        void print_title_info();
        void print_crafting_recipe();
        string get_main_name();
        string get_recipe_name();
        string get_recipe_type(const string&);
        bool failsafe(const string&);
        void print_text_invalid();
        void print_text_recipe();
        string print_line();
};


/**
 * @brief standard input of ct generated string. 
 * iterates through the string and stores each item within <> and each null to a vector string.
 * 
 */
void formatter::ct_recipe() {
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
                ingredient.push_back(substring);
                pos = input_string.find('<', pos_end);
            }
            else
                break;
        }
        else
            break;
    }
}


void formatter::ct_amount() {
    pos_asterisk = input_string.find("*");

    if (pos_asterisk != string::npos) {
        pos_asterisk_end = input_string.find(",");
        if (pos_asterisk_end != string::npos) {
            craft_amount = input_string.substr(pos_asterisk, pos_asterisk_end - pos_asterisk + 1);
        }
    }
}


/**
 * @brief Get the main name object
 * 
 * @param item_name_main item that the recipe is crafting
 * @param temp stored ingredient 0 string (item being crafted)
 * @return string default name
 */
string formatter::get_main_name() {
    string item_name_main;
    string temp = ingredient[0];
    size_t colon_pos = temp.find(':');
    if (colon_pos == string::npos) {
        return "";
    }

    item_name_main = temp.substr(colon_pos + 1, temp.size() - colon_pos - 2);

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
 * @param item_name_recipe item that the recipe is crafting
 * @param temp string to store item thats being crafted (ingredient 0)
 * @return string unique recipe identifier name
 */
string formatter::get_recipe_name() {
    string item_name_recipe;
    string temp = ingredient[0];
    for (size_t i = 1; i < temp.size() - 1; ++i) {
        char ch = temp[i];
        if (ch == ':' || ch == '_') {
            continue;
        }
        item_name_recipe += ch;
    }
    return item_name_recipe;
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
    return count >= 2;
}

void formatter::print_title_info() {
    cout << "\n+-----------------------------------------------------------+\n"
         << "| ###################### CT Formatter ##################### |\n"
         << "+-----------------------------------------------------------+\n"
         << "| Supports CraftTweaker Shaped & Shapeless Crafting Recipes |\n"
         << "+-----------------------------------------------------------+\n\n";
}

void formatter::print_text_invalid() {
    cout << "\n----------------------- Invalid Input -----------------------\n";
}

void formatter::print_text_recipe() {
    cout << "\nPaste in unformatted recipe below, type 'exit' to end program\n"
         << "v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v\n\n";
}

string formatter::print_line() {
    string line = "<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>";
    return line;
}

/**
 * @brief returns either shaped or shapeless recipe type
 * 
 * @param input input string from standard input
 * @return string recipe type
 */
string formatter::get_recipe_type(const string& input)
{
    char ch;
    string recipe_type = "";

    std::istringstream ss(input);

    while (ss.get(ch)) {
        if (ch == '(') {
            break;
        }
        recipe_type += ch;
    }
    return recipe_type;
}

/**
 * @brief prints the formatted string
 * 
 * @param item_name registry name
 * @param item_name_main default name
 * @param item_name_recipe unique recipe identifier name
 * @param ingredient item used in recipe
 */
void formatter::print_crafting_recipe() {
    cout << "\n" << print_line() << "\n\n"
         << setfill(' ') << "#" << get_main_name() << "\n"
         << setw(4) << " " << "recipes.remove(" << ingredient[0] << ");\n"
         << setw(4) << " " << get_recipe_type(input_string) << "(\"" << get_recipe_name() << "\",\n"
         << setw(8) << " " << ingredient[0] << " " << craft_amount << "\n";

    if (get_recipe_type(input_string) == shaped) {
        cout << setw(12) << " " << "[[" << ingredient[1] << ", " << ingredient[2] << ", " << ingredient[3] << "],\n"
         << setw(12) << " " << "[" << ingredient[4] << ", " << ingredient[5] << ", " << ingredient[6] << "],\n"
         << setw(12) << " " << "[" << ingredient[7] << ", " << ingredient[8] << ", " << ingredient[9] << "]]);\n"
         << "\n" << print_line() << "\n";
    } 
    else if (get_recipe_type(input_string) == shapeless) {
        cout << setw(12) << " " << "[";
        int count = 0;
        for (int i = 1; i < ingredient.size(); i++) {
            cout << ingredient[i];
            count++;
            if (count < ingredient.size() - 1) {
                cout << ",";
            }
        }
        cout << "]);\n\n" << print_line() << "\n";
    }
}

/**
 * @brief standard input for recipe type selection.
 * 
 */
int main() {
    formatter obj;
    obj.print_title_info();
    
    while (true) {
        obj.print_text_recipe();
        getline(cin, obj.input_string);
        if (obj.input_string == "exit") {
            obj.ingredient.clear();
            break;
        }
        if (obj.failsafe(obj.input_string)) {
            obj.ct_recipe();
            obj.ct_amount();
            if (!obj.ingredient.empty()) {
                obj.print_crafting_recipe();
                obj.ingredient.clear();
                obj.craft_amount.clear();
            } else {
                obj.print_text_invalid();
            }
            obj.ingredient.clear();
            obj.craft_amount.clear();
        } else {
            obj.print_text_invalid();
        }
    }
    return 0;
}