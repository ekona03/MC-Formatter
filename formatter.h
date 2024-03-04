#ifndef FORMATTER_H
#define FORMATTER_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class formatter {
    public:
        const string shaped = "recipes.addShaped";
        const string shapeless = "recipes.addShapeless";
        vector<string> ingredient;

        void ct_recipe(string);
        string ct_amount(string);
        void print_crafting_recipe(string, string, string);
        string get_main_name();
        string get_recipe_name();
        string get_recipe_type(string);

        void format();
};

#endif