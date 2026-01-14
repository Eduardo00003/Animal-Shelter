#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::string toLower(std::string text) {
    std::transform(
        text.begin(), text.end(), text.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return text;
}

class Animal {
public:
    Animal(std::string name, int age) : name_(std::move(name)), age_(age) {}
    virtual ~Animal() = default;

    virtual std::string species() const = 0;
    virtual std::string sound() const = 0;
    virtual std::string personality() const { return "is friendly with people"; }

    void describe(std::ostream &out) const {
        out << name_ << " the " << species() << " (" << age_ << "y) "
            << personality() << " and usually sounds like \"" << sound()
            << "\".";
    }

protected:
    const std::string &name() const { return name_; }
    int age() const { return age_; }

private:
    std::string name_;
    int age_;
};

class Dog : public Animal {
public:
    Dog(std::string name, int age) : Animal(std::move(name), age) {}

    std::string species() const override { return "dog"; }
    std::string sound() const override { return "woof"; }
    std::string personality() const override { return "loves fetch and long walks"; }
};

class Cat : public Animal {
public:
    Cat(std::string name, int age) : Animal(std::move(name), age) {}

    std::string species() const override { return "cat"; }
    std::string sound() const override { return "meow"; }
    std::string personality() const override { return "appreciates sunny windows"; }
};

class Parrot : public Animal {
public:
    Parrot(std::string name, int age) : Animal(std::move(name), age) {}

    std::string species() const override { return "parrot"; }
    std::string sound() const override { return "squawk"; }
    std::string personality() const override { return "loves to mimic conversations"; }
};

class Shelter {
public:
    void intake(std::unique_ptr<Animal> pet) {
        if (pet) {
            available_.push_back(std::move(pet));
        }
    }

    std::unique_ptr<Animal> adopt(const std::string &desiredSpecies) {
        auto species = toLower(desiredSpecies);
        auto it = std::find_if(available_.begin(), available_.end(),
                               [&](const auto &pet) {
                                   return pet && toLower(pet->species()) == species;
                               });
        if (it == available_.end()) {
            return nullptr;
        }
        std::unique_ptr<Animal> adopted = std::move(*it);
        available_.erase(it);
        return adopted;
    }

    void list(std::ostream &out) const {
        if (available_.empty()) {
            out << "Shelter is empty right now.\n";
            return;
        }

        for (std::size_t i = 0; i < available_.size(); ++i) {
            out << i + 1 << ") ";
            available_[i]->describe(out);
            out << '\n';
        }
    }

private:
    std::vector<std::unique_ptr<Animal>> available_;
};

class Adopter {
public:
    explicit Adopter(std::string name) : name_(std::move(name)) {}

    void adopt(std::unique_ptr<Animal> pet) {
        if (pet) {
            myPets_.push_back(std::move(pet));
        }
    }

    void listMine(std::ostream &out) const {
        if (myPets_.empty()) {
            out << name_ << " has not adopted anyone yet.\n";
            return;
        }
        out << name_ << "'s adopted pals:\n";
        for (const auto &pet : myPets_) {
            out << " - ";
            pet->describe(out);
            out << '\n';
        }
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<Animal>> myPets_;
};

std::unique_ptr<Animal> makeAnimal(const std::string &species, const std::string &name,
                                   int age) {
    auto lower = toLower(species);
    if (lower == "dog" || lower == "puppy") {
        return std::make_unique<Dog>(name, age);
    }
    if (lower == "cat" || lower == "kitten") {
        return std::make_unique<Cat>(name, age);
    }
    if (lower == "parrot" || lower == "bird") {
        return std::make_unique<Parrot>(name, age);
    }
    return nullptr;
}

void seedShelter(Shelter &shelter) {
    shelter.intake(std::make_unique<Dog>("Nova", 3));
    shelter.intake(std::make_unique<Cat>("Kumo", 2));
    shelter.intake(std::make_unique<Parrot>("Rio", 4));
    shelter.intake(std::make_unique<Dog>("Pepper", 5));
}

void showHelp() {
    std::cout << "\nCommands:\n"
              << "  list                    - view animals in the shelter\n"
              << "  mine                    - view who you adopted\n"
              << "  adopt <type>            - adopt by species (dog/cat/parrot)\n"
              << "  intake <type> <name> <age> - add a new arrival\n"
              << "  help                    - show this help\n"
              << "  quit                    - leave the shelter\n";
}

int main() {
    Shelter shelter;
    Adopter player("You");
    seedShelter(shelter);

    std::cout << "Small OOP demo: Animal Shelter\n";
    showHelp();

    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::istringstream input(line);
        std::string cmd;
        input >> cmd;
        if (cmd.empty()) {
            continue;
        }
        cmd = toLower(cmd);

        if (cmd == "help") {
            showHelp();
        } else if (cmd == "list") {
            shelter.list(std::cout);
        } else if (cmd == "mine") {
            player.listMine(std::cout);
        } else if (cmd == "adopt") {
            std::string desired;
            input >> desired;
            if (desired.empty()) {
                std::cout << "Usage: adopt <type>\n";
                continue;
            }
            auto pet = shelter.adopt(desired);
            if (!pet) {
                std::cout << "No " << desired << "s are available right now.\n";
                continue;
            }
            std::cout << "You adopted a " << desired << "! Take good care of them.\n";
            player.adopt(std::move(pet));
        } else if (cmd == "intake") {
            std::string species, name;
            int age{};
            if (!(input >> species >> name >> age)) {
                std::cout << "Usage: intake <type> <name> <age>\n";
                continue;
            }
            auto pet = makeAnimal(species, name, age);
            if (!pet) {
                std::cout << "I don't recognize that species yet. Try dog/cat/parrot.\n";
                continue;
            }
            shelter.intake(std::move(pet));
            std::cout << name << " checked in to the shelter.\n";
        } else if (cmd == "quit" || cmd == "exit") {
            std::cout << "Thanks for visiting!\n";
            break;
        } else {
            std::cout << "Unknown command. Type 'help' for options.\n";
        }
    }
}
