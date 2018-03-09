#include "Files.h"
#include "GlobalVariables.h"

//BaseFile//
BaseFile::BaseFile(string name) : name(name) {}  // Constructor

string BaseFile::getName() const {
	return name;
}
void BaseFile::setName(string newName) {
	name = newName;
}
BaseFile::~BaseFile() {} // Destructor.

//File//
File::File(string _name, int _size) : BaseFile(_name), size(_size) {} // Constructor

int File::getSize() { // Return the size of the file.
	return size;
}
int File::getType() const { // Returns the type of File, which we decided will be 0
	return 0;
}

//Directory//
Directory::Directory(string _name, Directory *_parent) : BaseFile(_name), children(), parent(_parent) {} // Constructor 

Directory::Directory(const Directory& other) : Directory(other.getName(), other.parent) { // Copy Constructor.
	if (verbose == 1 || verbose == 3) {
		cout << "Directory::Directory(const Directory& other)" << endl;
	}
	copy(other);
}

Directory& Directory::operator=(const Directory& other) { // Copy Assignment Operator.
	if (verbose == 1 || verbose == 3) {
		cout << "Directory& Directory::operator=(const Directory& other)" << endl;
	}
	if (this == &other) { // check for "self assignment" and do nothing in that case
		return *this;
	}
	remove();
	copy(other);
	return *this;
}
Directory& Directory::operator=(Directory&& other) { // Move Assignment Operator.
	if (verbose == 1 || verbose == 3) {
		cout << "Directory& Directory::operator=(Directory&& other)" << endl;
	}
	remove();
	steal(other);
	return *this;
}
Directory::Directory(Directory&& other) : BaseFile(other.getName()), children(), parent(nullptr) { // Move Constructor.
	if (verbose == 1 || verbose == 3) {
		cout << "Directory::Directory(Directory&& other)" << endl;
	}
	steal(other);
}
Directory::~Directory() { // Destructor.
	if (verbose == 1 || verbose == 3) {
		cout << "Directory::~Directory()" << endl;
	}
	remove();
}

Directory* Directory::getParent() const {// Return a pointer to the parent of this directory
	return parent;
}

int Directory::getType() const { // Returns the type of Directory, which we decided will be 1
	return 1;
}

void Directory::setParent(Directory *newParent) { // Change the parent of this directory
	parent = newParent;
}
void Directory::addFile(BaseFile* file) {// Add the file to children
	bool exists = false; // Boolean for checking if the file's name already exists.
	int size = children.size();
	for (int index = 0; index < size && exists != true; index++){
		if (children[index]->getName() == file->getName()) {
			exists = true;
		}
	}
	if (exists == false) { // If the file's name does not exist, adding it to the children.
		children.push_back(file);
		if (file->getType() == 1)
			((Directory*)(file))->setParent(this); //In case the file is actually a directory, setting it's parent parameter as "this".
	}
}

void Directory::removeFile(string name) { // Remove the file with the specified name from children
	bool wasRemoved = false; // Boolean for checking if the file has already been removed.
	int size = children.size();
	for (int index = 0; index < size && wasRemoved != true; index++) {
		if (children[index]->getName() == name) { //Checking whether the current BaseFile's name is the same as the name which was provided and delete it accordingly. 
			wasRemoved = true;
			delete children[index];
			children[index] = nullptr;
			children.erase(children.begin()+index);
		}
	}
}

void Directory::removeFile(BaseFile* file) { // Remove the file from children
	bool wasRemoved = false; // Boolean for checking if the file has already been removed.
	int size = children.size();
	for (int index = 0; index < size && wasRemoved != true; index++) {
		if (children[index] == file) { //Checking whether the current BaseFile's name is the same as the name which was provided and delete it accordingly. 
			wasRemoved = true;
			delete children[index];
			children[index] = nullptr;
			children.erase(children.begin()+index);
		}
	}
}

bool Directory::compareByName(BaseFile* file1, BaseFile* file2) { // Comparator for sorting by name.
	string name1 = file1->getName();
	string name2 = file2->getName();
	return (name1<name2); // Comparing the names.			
}

void Directory::sortByName() { // Sort children by name alphabetically (not recursively)
	stable_sort(children.begin(), children.end(), compareByName);
}

vector<BaseFile*> Directory::getChildren() { // Return children
	return children;
}
int Directory::getSize() {// Return the size of the directory (recursively)
	int size = 0;
	if (children.size() == 0) {
		return 0;
	}
	else {
		int childrenSize = children.size();
		for (int index = 0; index < childrenSize; index++) {
			int currentSize = children[index]->getSize();
			size = size + currentSize; // Sum up the size of a file or recursively summing up the total size of the directory.  
		}
	}
	return size;
}

bool Directory::compareBySize(BaseFile* file1, BaseFile* file2) { // Comparator for sorting by size.
	int size1 = file1->getSize();
	int size2 = file2->getSize();
	return (size1<size2); // Comparing the sizes.			
}

void Directory::sortBySize() { //Sort children by size (not recursively)
	stable_sort(children.begin(), children.end(), compareBySize);
}
string Directory::getAbsolutePath() {  //Return the path from the root to this
	string path = "";
	Directory* current = this;
	while (current != nullptr) {
		string name = current->getName();
		path = "/" + name + path;
		current = current->getParent();
	}
	if (path == "/") // In case there are parent directories - We are at the root, thus setting it as /.
		return path;
	else
		return path.substr(1);
}
void Directory::steal(Directory& other) { // Internal function for move constructor and move operator.
	children = other.getChildren();
	parent = other.getParent();
	//the next lines complete the stealing
	vector<BaseFile*> otherVector;
	other.children = otherVector;
}

void Directory::copy(const Directory& other) { // Internal function for copy constructor and copy operator.
	int childrenSize = other.children.size();
	for (int index = 0; index < childrenSize; index++) {
		BaseFile* newChildToBeCopied;
		if ((other.children[index])->getType() == 1) { // The BaseFile is actually a Directory:
			newChildToBeCopied = new Directory(((Directory&)*(other.children[index])));  // Creating new Directory in the heap. 
			((Directory*)newChildToBeCopied)->setParent(this); // Adding the newly created directory to the children. 
		}
		else { // The BaseFile is actually a File:
			newChildToBeCopied = new File(((File&)*(other.children[index]))); // Creating new File in the heap.
		}
		children.push_back(newChildToBeCopied);
	}
}

void Directory::remove() { // // Internal function for destructor. 
	int size = children.size();
	for (int index = 0; index < size; index++) {
		delete children[index];
		children[index] = nullptr;
	}
}
