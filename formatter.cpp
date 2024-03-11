#include "formatter.h"
#include "print_statements.h"
#include "print_statements.cpp"


/**
 * @brief standard input of ct generated string. 
 * iterates through the string and stores each item within <> and each null to a vector string.
 * 
 */
void formatter::ct_recipe(string input_string) {
    size_t pos = input_string.find('<');
    size_t pos_null = input_string.find("null");

    while (pos != string::npos || pos_null != string::npos) {
        if (pos_null != string::npos && (pos == string::npos || pos_null < pos)) {
            ingredient.push_back("null");
            pos_null = input_string.find("null", pos_null + 4);
        } else if (pos != string::npos) {
            size_t pos_end = input_string.find('>', pos);
            if (pos_end != string::npos) {
                string substring = input_string.substr(pos, pos_end - pos + 1);
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

/**
 * @brief finds the amount to be crafted
 * 
 */
string formatter::ct_amount(string input_string) {
    size_t pos_asterisk = input_string.find("*");
    string craft_amount;

    if (pos_asterisk != string::npos) {
        size_t pos_asterisk_end = input_string.find(",");
        if (pos_asterisk_end != string::npos) {
            craft_amount = input_string.substr(pos_asterisk, pos_asterisk_end - pos_asterisk + 1);
        }
    }
    if (!craft_amount.empty()) {
        craft_amount.erase(craft_amount.size() - 1);
    }
    return craft_amount;
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
 * @brief returns either shaped or shapeless recipe type
 * 
 * @param input input string from standard input
 * @return string recipe type
 */
string formatter::get_recipe_type(string input)
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
void formatter::print_crafting_recipe(string input_string, string recipe_type, string craft_amount) {
    cout << "\n" << "<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>" << "\n\n"
         << setfill(' ') << "#" << get_main_name() << "\n"
         << setw(4) << " " << "recipes.remove(" << ingredient[0] << ");\n"
         << setw(4) << " " << recipe_type << "(\"" << get_recipe_name() << "\",\n"
         << setw(8) << " " << ingredient[0] << craft_amount << ",\n";

    if (recipe_type == shaped) {
        cout << setw(12) << " " << "[[" << ingredient[1] << ", " << ingredient[2] << ", " << ingredient[3] << "],\n"
         << setw(12) << " " << "[" << ingredient[4] << ", " << ingredient[5] << ", " << ingredient[6] << "],\n"
         << setw(12) << " " << "[" << ingredient[7] << ", " << ingredient[8] << ", " << ingredient[9] << "]]);\n"
         << "\n" << "<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>" << "\n";
    } 
    else if (recipe_type == shapeless) {
        cout << setw(12) << " " << "[";
        int count = 0;
        for (int i = 1; i < ingredient.size(); i++) {
            cout << ingredient[i];
            count++;
            if (count < ingredient.size() - 1) {
                cout << ",";
            }
        }
        cout << "]);\n\n" << "<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>=<>" << "\n";
    }
}

void formatter::format()
{
    string input_string;
    string recipe_type;
    string craft_amount;

    print_title_info();
    
    while (true) {
        print_paste_text();
        getline(cin, input_string);

        if (input_string == "exit") {
            break;
        }

        recipe_type = get_recipe_type(input_string);
        craft_amount = ct_amount(input_string);

        if (recipe_type == shaped || recipe_type == shapeless) {
            ct_recipe(input_string);
            print_crafting_recipe(input_string, recipe_type, craft_amount);
            ingredient.clear();
            input_string.clear();
            recipe_type.clear();
            craft_amount.clear();
        } else {
            print_invalid_text();
            ingredient.clear();
            input_string.clear();
            recipe_type.clear();
            craft_amount.clear();
        }
    }
}