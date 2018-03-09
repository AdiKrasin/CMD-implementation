#ifndef FILES_H_
#define FILES_H_

#include <string>
using namespace std;
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <iostream>

class BaseFile {
private:
	string name;

public:
	BaseFile(string name);
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
	virtual int getType() const = 0;
	virtual ~BaseFile()=0;
};

class File : public BaseFile {
private:
	int size;

public:
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
	int getType() const;
};

class Directory : public BaseFile {
private:
	vector<BaseFile*> children;
	Directory *parent;

public:
	int getType() const;
	Directory(string name, Directory *parent); // Constructor.
	Directory(const Directory& other); // Copy Constructor.
	Directory& operator=(const Directory& other); // Copy Assignment Operator.
	Directory& operator=(Directory&& other); // Move Assignment Operator.
	Directory(Directory&& other); // Move Constructor.
	virtual ~Directory(); // Destructor.
	Directory *getParent() const; // Return a pointer to the parent of this directory.
	void setParent(Directory *newParent); // Change the parent of this directory.
	void addFile(BaseFile* file); // Add the file to children.
	void removeFile(string name); // Remove the file with the specified name from children.
	void removeFile(BaseFile* file); // Remove the file from children.
	static bool compareByName(BaseFile* file1, BaseFile* file2); // Comparator for sorting by name.
	void sortByName(); // Sort children by name alphabetically (not recursively)
	static bool compareBySize(BaseFile* file1, BaseFile* file2); // Comparator for sorting by size.
	vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	void sortBySize(); // Sort children by size (not recursively)
	string getAbsolutePath();  //Return the path from the root to this
	void steal(Directory& other); // Internal function for move constructor and move operator.
	void copy(const Directory& aDirectory);  // Internal function for copy constructor and copy operator.
	void remove(); // // Internal function for destructor. 
};
#endif