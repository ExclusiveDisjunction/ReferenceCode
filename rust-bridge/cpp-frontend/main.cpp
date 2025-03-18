#include <iostream>

// clang --std=c++20  main.cpp -o main -L. -lbackend -lstdc++

extern "C" {
    struct Passing {
        unsigned int num;
        unsigned short thing;
    };

    Passing rust_process(Passing data);
    Passing create_passing();
}

int main() {
    Passing first = create_passing();
    Passing second = rust_process(first);

    std::cout << second.thing << std::endl;
}