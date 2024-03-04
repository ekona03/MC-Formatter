#include "print_statements.h"

void print_title_info() {
    std::cout << "\n+-----------------------------------------------------------+\n"
         << "| ###################### CT Formatter ##################### |\n"
         << "+-----------------------------------------------------------+\n"
         << "| Supports CraftTweaker Shaped & Shapeless Crafting Recipes |\n"
         << "+-----------------------------------------------------------+\n\n";
}

void print_invalid_text() {
    std::cout << "\n----------------------- Invalid Input -----------------------\n";
}

void print_paste_text() {
    std::cout << "\nPaste in unformatted recipe below, type 'exit' to end program\n"
         << "v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v\n\n";
}