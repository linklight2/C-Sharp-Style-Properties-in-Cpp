#include <iostream>
#include <string>
#include "CSharpStyleProperties.h"

using std::string;
using std::cout;

class Animal {
protected:
	int _age = 0;
	string _name;
	string _species;
	string _animalSound;

public:
	// Polymorphic properties must be constructed with the class, otherwise there is a complicated
	// template error. 
	Animal(const string& name) : _name(name), animalSound(this), species(this) {}

	// Custom Property(VarName, ClassType, ReturnType (of get function), Get Function Code,
	// PassType (for 'value' in the set function), Set Function Code);
	CUSTOM_PROPERTY(age, Animal, int, { return _age; }, int, { _age = value; });
	// 'value' is the name of the parameter passed to the Set Function

	// ReadOnly Property(VarName, ClassType, ReturnType (of get function), Get Function Code);
	READ_ONLY_PROPERTY(name, Animal, const string&, { return _name; });

	// Same thing as Custom Property, but it can be used polymorphically
	POLYMORPHIC_PROPERTY(animalSound, Animal, string, { return "I say " + _animalSound + "?"; }, const string&, { _animalSound = value; });

	// Same thing as ReadOnly property, but it can be used polymorphically
	READ_ONLY_POLYMORPHIC_PROPERTY(species, Animal, string, { return "A Generic Animal"; });
};

class Dog : public Animal {
public:
	Dog(const string& name) : Animal(name) {}

	// Get Override(VarName (that you used for the base class)) { getter }
	GET_OVERRIDE(animalSound) {
		return "I say " + _animalSound + "!";
	}
	// Set Override(VarName (that you used for the base class)) { setter }
	SET_OVERRIDE(animalSound) {
		_animalSound = value + "!!";
	}
	// Get Override(VarName (that you used for the base class)) { getter }
	GET_OVERRIDE(species) {
		return "Dog";
	}
};

int main() {
	Animal* Froggy = new Animal("Fred");
	Animal* Doggy = new Dog("Gary");

	// This presents a big problem with my solution. Because of the way C++ implicitly 
	// copies objects, the property's 'this' pointer incorrectly points to the other object's 'this'
	Animal problem = *Doggy;

	char ch;

	Froggy->animalSound = "Ribbit"; // animalSound returns "I say Ribbit?"
	Froggy->age = 3; // age returns '3'
	//Froggy->species = "Dog"; // Causes error, read-only properties cannot be assigned to
	//Froggy->name = "Gary"; // Causes error, read-only properties cannot be assigned to

	problem.animalSound = "Moo"; // animalSound will be overriden by Doggy->animalSound = "Woof"
	problem.age = 10; // age will be overriden by Doggy->age = 4

	Doggy->animalSound = "Woof"; // animalSound returns "I say Woof!!!"
	Doggy->age = 4; // age returns 4;

	// The * operator can easily be used to retrieve the content's of the get function 
	// if it is not automatically converting. 
	cout << "(Froggy) My name is " << *Froggy->name << ", I am " << Froggy->age
		<< " years old, my species is \"" << *Froggy->species << "\", and "
		<< *Froggy->animalSound << "\n";

	cout << "(Problem) My name is " << *problem.name << ", I am " << problem.age // 'age' should be 10
		<< " years old, my species is \"" << *problem.species << "\", and " // 'species' should be "A Generic Animal"
		<< *problem.animalSound << "\n"; // 'animalSound' should be "I say Moo?"

	cout << "(Doggy) My name is " << *Doggy->name << ", I am " << Doggy->age
		<< " years old, my species is \"" << *Doggy->species << "\", and "
		<< *Doggy->animalSound << "\n";

	std::cin >> ch; // Used to wait till the user presses a key before console exit

	return 0;
}