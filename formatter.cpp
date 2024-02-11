#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cctype>

using namespace std;

void ct_recipe();
void ct_print(string, string, string, const vector<string>&);
string get_main_name(const string&);
string get_recipe_name(const string&);

/**
 * @brief standard input for recipe type selection.
 * 
 */
int main()
{
    int val;
    cout << "\n<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>\n"
         << "CT Formatter\n"
         << "<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>\n"
         << "[1] CraftTweaker Table Recipe\n"
         << "[2] Extended Crafting 3x3\n"
         << "[3] Extended Crafting 5x5\n"
         << "[4] Extended Crafting 7x7\n"
         << "[5] Extended Crafting 9x9\n"
         << "<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>\n\n"
         << "Select Recipe Type: ";
    
    cin >> val;
    switch (val)
    {
        case 1:
            cin.ignore();
            ct_recipe();
            break;
        default:
            break;
    }
    

    return 0;
}


/**
 * @brief standard input of ct generated string. 
 * parses through the string and stores each item within <> to a vector string.
 * 
 */
void ct_recipe()
{
    string input_string;
    string item_name_main;
    string item_name_recipe;
    string item_name;
    string substring;

    vector<string> ingredient;

    size_t pos;
    size_t end_pos;
    size_t pos_null;

    while (true)
    {
        cout << "\nPaste in the unformatted recipe below, type exit to end program:\n";
        getline(cin, input_string);

        if (input_string == "exit")
        {
            break;
        }
        else
        {
            pos = input_string.find('<');
            pos_null = input_string.find("null");

            ingredient.clear();

            while (pos != string::npos || pos_null != string::npos)
            {
                if (pos_null != string::npos && (pos == string::npos || pos_null < pos))
                {
                    ingredient.push_back("null");
                    pos_null = input_string.find("null", pos_null + 4);
                }
                else if (pos != string::npos)
                {
                    end_pos = input_string.find('>', pos);
                    if (end_pos != string::npos)
                    {
                        substring = input_string.substr(pos, end_pos - pos + 1);

                        if (item_name.empty())
                        {
                            item_name = substring;
                        }
                        else
                        {
                            ingredient.push_back(substring);
                        }

                        pos = input_string.find('<', end_pos);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            item_name_main = get_main_name(item_name);
            item_name_recipe = get_recipe_name(item_name);
            ct_print(item_name, item_name_main, item_name_recipe, ingredient);
        }
    }
}


/**
 * @brief Get the main name object
 * 
 * @param item_name item that the recipe is crafting
 * @return string default name
 */
string get_main_name(const string& item_name)
{
    size_t colon_pos = item_name.find(':');
    if (colon_pos == string::npos)
    {
        return "";
    }

    string item_name_main = item_name.substr(colon_pos + 1, item_name.size() - colon_pos - 2);

    for (size_t i = 0; i < item_name_main.length(); ++i)
    {
        if (item_name_main[i] == '_')
        {
            item_name_main[i] = ' ';
        }
    }

    bool capitalize_next = true;
    for (size_t i = 0; i < item_name_main.length(); ++i)
    {
        if (capitalize_next && islower(item_name_main[i]))
        {
            item_name_main[i] = toupper(item_name_main[i]);
            capitalize_next = false;
        }
        else if (isspace(item_name_main[i]))
        {
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
string get_recipe_name(const string& item_name)
{
    string item_name_recipe;

    for (size_t i = 1; i < item_name.size() - 1; ++i)
    {
        char ch = item_name[i];
        if (ch == ':' || ch == '_')
        {
            continue;
        }
        item_name_recipe += ch;
    }

    bool capitalize_next = true;
    for (char& ch : item_name_recipe)
    {
        if (capitalize_next && islower(ch))
        {
            ch = toupper(ch);
            capitalize_next = false;
        }
        else if (isspace(ch))
        {
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
void ct_print(string item_name, string item_name_main, string item_name_recipe, const vector<string>& ingredient)
{
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