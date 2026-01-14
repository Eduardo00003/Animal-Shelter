# Animal Shelter OOP Demo

Small C++17 console app that demonstrates inheritance, polymorphism, and composition. An `Animal` base class is extended by `Dog`, `Cat`, and `Parrot`. A `Shelter` manages available animals while an `Adopter` keeps adopted pets, both owning them via `std::unique_ptr`.

## Prerequisites
- Any C++17 compiler (tested with `g++`/`clang++` on macOS).

## Build and run
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o shelter_demo
./shelter_demo
```

## Commands inside the app
- `list` — show animals currently in the shelter
- `mine` — show who you have adopted
- `adopt <type>` — adopt by species (dog, cat, parrot)
- `intake <type> <name> <age>` — add a new arrival
- `help` — show the command list
- `quit`/`exit` — leave the shelter

## Extending
- Add a new species by creating a new `Animal` subclass and wiring it into `makeAnimal`.
- Seed more starting animals in `seedShelter`.
