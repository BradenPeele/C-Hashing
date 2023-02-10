#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

// structure used for each element in the array (hash table)
struct Person
{
	int key;
	string name;
};

void getCollisionOption(); // gets and sets collisionResolution (global)
int getMenuOption(); // gets user input from user

void insert_remove_search(Person table[], int); // handles user input changes to hash table
void buildFromText(Person table[]); // fills hash table from text file
void display(Person table[]); // displays entire hash table

int hashFunction(Person table[], int, bool); // hash function (returns position)

int collisionResolution;

int main()
{
	int option; // menu option
	Person table[19]; // array of structures (hash table)

	// initialize values to -1 (indicates cell is empty)
	for(int n = 0; n < 19; n++)
		table[n].key = -1;

	getCollisionOption();

	// driver loop
	do
	{
		option = getMenuOption();

		if(option == 1 || option == 2 || option == 3)
			insert_remove_search(table, option);
		if(option == 4)
			buildFromText(table);
		if(option == 5)
			display(table);
	}
	while(option != 6); // (6) exit

	cout << endl << "program ending";

	return 0;
}


// function only called once at beginning of program execution
// gets and sets collisionResolution (global)
void getCollisionOption()
{
	int option;

	cout << "Option 1: linear" << endl
		 << "Option 2: quadratic" << endl
		 << "Option 3: double hashing" << endl;

	// prime read
	cout << endl << "Option: ";
	cin >> option;

	// validate user input
	while(option < 1|| option > 3)
	{
		cout << "Invalid option" << endl << "Option: ";
		cin >> option;
	}

	collisionResolution = option;
}


// called every cycle through driver loop
int getMenuOption()
{
	int option;

	cout << endl
		 << "Option 1: insert" << endl
		 << "Option 2: remove" << endl
		 << "Option 3: search" << endl
		 << "Option 4: build from text file" << endl
		 << "Option 5: display all records" << endl
		 << "Option 6: quit" << endl;

	// prime read
	cout << endl << "Option: ";
	cin >> option;

	// validate user input
	while(option < 1 || option > 6)
	{
		cout << "Invalid option" << endl << "Option: ";
		cin >> option;
	}

	return option;
}


// prompts user for key / name to insert / remove / search records
void insert_remove_search(Person table[], int operation)
{
	int key; // (local) used for user input/ validation of the key
	string name; // (local) used for user input/ validation of the name
	int position; // indicates the index of the array (hash table)

	// prime read
	cout << endl << "key(0 - 999): ";
	cin >> key;

	// validate user input
	while(key < 0 || key > 999)
	{
		cout << "Invalid key" << endl << "key: ";
		cin >> key;
	}

	// block of code for insertion a record from console (operation / option 1)
	if(operation == 1)
	{
		// prime read
		cin.ignore();
		cout << "name(40 char max): ";
		getline(cin, name);

		// validate user input
		while(name.length() > 40)
		{
			cout << "Invalid name" << endl << "name: ";
			getline(cin, name);
		}

		// hash function called, passed true (bool insert)
		position = hashFunction(table, key, true);

		// checking for error (hashFunction returns -1 if the key can't be placed)
		if(position != -1)
		{
			cout << "placed " << key << ", " << name << " in position " << position << endl;
			table[position].key = key; // sets array (hash table) key to local (user input) key
			table[position].name = name; // sets array (hash table) name to local (user input) name
		}
		else
			cout << "ERROR: "<< "key: " << key << " could not be placed" << endl;
	}

	// block of code for deleting a record (operation 2 / option 2)
	if(operation == 2)
	{
		// hash function called, passed false (bool insert)
		position = hashFunction(table, key, false);

		// checking for error (hashFunction returns -1 if the record can't be found)
		if(position != -1)
		{
			cout << "removed " << table[position].key << ", " << table[position].name << " from position " << position << endl;
			table[position].key = -1; // sets array (hash table) key to -1 indicating empty
			table[position].name = ""; // sets array (hash table) key to empty string "" indicating empty
		}
		else
			cout << "ERROR: "<< "key: " << key << " could not be removed" << endl;
	}

	// block of code for searching for a record (operation 3 / option 3)
	if(operation == 3)
	{
		// hash function called, passed false (bool insert)
		position = hashFunction(table, key, false);

		// checking for error (hashFunction returns -1 if the record can't be found)
		if(position != -1)
			cout << "found "<< table[position].key << ", " << table[position].name << " in position " << position << endl;
		else
			cout << "ERROR: "<< "key: " << key << " could not be found" << endl;
	}
}

// fills array (hash table) with records from text file
void buildFromText(Person table[])
{
	int key;
	char name[41];
	int position; // indicates the index of the array (hash table)
	string filename;

	cout << endl << "filename: ";
	cin >> filename;

	ifstream inFile(filename);

	// validate filename
	if(!inFile)
	{
		cout << endl << "ERROR: file name can't be found" << endl;
		return;
	}

	// validate filename

	while(!inFile.eof())
	{
		inFile.get(name, 41); // gets name, local name set
		inFile >> key; // gets key, local key set
		inFile.ignore(100, '\n');

		// hash function called, passed true (bool insert)
		position = hashFunction(table, key, true);

		// checking for error (hashFunction returns -1 if the key can't be placed)
		if(position != -1)
		{
			cout << "placed " << key << ", " << name << endl;
			table[position].key = key; // set array (hash table) key to local key
			table[position].name = name; // set array (hash table) name to local name
		}
		else
			cout << "ERROR: "<< "key: " << key << " could not be placed" << endl;
	}
}


// displays all records
void display(Person table[])
{
	for(int n = 0; n < 19; n++)
		cout << "position: " << setw(2) << n  << "   key: " << setw(3)<< table[n].key << "   name: " << table[n].name << endl;
}


int hashFunction(Person table[], int key, bool insert)
{
	int increment = 1; // increment used for all 3 probing methods
	int numProbes = 0; // counter
	int position; // index of array (hash table)

	// for double hashing
	if(collisionResolution == 3)
	{
		increment = key / 19;

		if(increment == 0)
			increment = 1;

		cout << endl << "increment: " << increment;
	}

	cout << endl << "key: " << key << endl;

	do
	{
		// sets increment to numProbes so position = key + increment^2 % 19
		if(collisionResolution == 2)
			increment = numProbes;

		position = (key + increment * numProbes) % 19; // finding next position
		numProbes++;

		cout << "probed: " << position << endl;

		// return position if found (condition for inserting) (condition for searching / deleting)
		if((insert && table[position].key == -1) || (!insert && table[position].key == key))
			return position;
	}
	while(numProbes < 19 || (collisionResolution == 2 && numProbes < 10));
	// will exit loop (error) if every possible position was probed
	// 19 (array size) for linear and double hashing and 10 ((array size + 1) / 2) for quadratic)

	// will only return from here if while loop condition not met (error)
	return position = -1;
}
