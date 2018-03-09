#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"
#include <string>
using namespace std;
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <iostream>


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
public:
	FileSystem(); // Constructor.
	FileSystem(const FileSystem& other); // Copy Constructor.
	FileSystem& operator=(const FileSystem& other); // Copy Assignment Operator.
	FileSystem& operator=(FileSystem&& other); // Move Assignment Operator.
	FileSystem(FileSystem&& other); // Move Constructor.
	virtual ~FileSystem(); // Destructor.
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system
	void steal(FileSystem& other); // Internal function for move constructor and move operator.
	void copy(const FileSystem& other); // Internal function for copy constructor and copy operator.
	void remove(); // // Internal function for destructor. 

};
#endif
