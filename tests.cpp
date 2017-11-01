#include "klib/std.h"
#include "klib/dict.h"

int main() {

    dict *stack = new dict(DEFUALT_VALUE, DEFUALT_VALUE);
    stack->scanAndFixBrokenBlocks();

    stack->append("Test", "Stuff");// Boi

    cout << "This will print out some " << stack->get("Test") << endl;

    return 0;
}
