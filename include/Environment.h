#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
using namespace std;
#include "FileSystem.h"
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <iostream>

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

public:
	Environment(); // Constructor.
	Environment(const Environment& other); // Copy Constructor.
	Environment& operator=(const Environment& other); // Copy Assignment Operator.
	Environment& operator=(Environment&& other); // Move Assignment Operator.
	Environment(Environment&& other); // Move Constructor.
	virtual ~Environment(); // Destructor.
	void start();
	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
	void steal(Environment& other); // Internal function for move constructor and move operator.
	void copy(const Environment& other); // Internal function for copy constructor and copy operator.
	void remove(); // // Internal function for destructor. 
};

#endif