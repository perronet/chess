#include <iostream>
#include <vector>
#include "state.h"

using namespace std;

int main() {
    state::State b;

    b.print();
    if (b.move(Position{6, 5}, Position{4, 5})) {
        b.print();
    }

    return 0;
}
