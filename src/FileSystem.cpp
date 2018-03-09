#include "FileSystem.h"
#include "Files.h"
#include "GlobalVariables.h"
#include "Commands.h"

FileSystem::FileSystem() : rootDirectory(), workingDirectory(){  // Constructor.
	rootDirectory = new Directory("", nullptr);  // Creating the root directory, setting parent as null
	workingDirectory = rootDirectory; //Creating the default path for the working directory
}

FileSystem::FileSystem(const FileSystem& other) : rootDirectory(), workingDirectory() { // Copy Constructor.
	if (verbose == 1 || verbose == 3) {
		cout << "FileSystem::FileSystem(const FileSystem& other)" << endl;
	}
	copy(other);
}

FileSystem& FileSystem::operator=(const FileSystem& other) { // Copy Assignment Operator.
	if (verbose == 1 || verbose == 3) {
		cout << "FileSystem& FileSystem::operator=(const FileSystem& other)" << endl;
	}
	if (this == &other) { // check for "self assignment" and do nothing in that case
		return *this;
	}
	remove(); //Deleting the existing root and working directories and setting their pointers as nullptr.
	copy(other);
	return *this;
}
FileSystem& FileSystem::operator=(FileSystem&& other) { // Move Assignment Operator.
	if (verbose == 1 || verbose == 3) {
		cout << "FileSystem& FileSystem::operator=(FileSystem&& other)" << endl;
	}
	remove();
	steal(other);
	return *this;
}
FileSystem::FileSystem(FileSystem&& other) : rootDirectory(), workingDirectory() { // Move Constructor.
	if (verbose == 1 || verbose == 3) {
		cout << "FileSystem::FileSystem(FileSystem&& other)" << endl;
	}
	steal(other);
}
FileSystem::~FileSystem() { // Destructor.
	if (verbose == 1 || verbose == 3) {
		cout << "FileSystem::~FileSystem()" << endl;
	}
	remove();
}
Directory& FileSystem::getRootDirectory() const { // Return reference to the root directory
	return *rootDirectory;
}
Directory& FileSystem::getWorkingDirectory() const { // Return reference to the working directory
	return *workingDirectory;
}
void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) { // Change the working directory of the file system
	workingDirectory = newWorkingDirectory;
}

void FileSystem::steal(FileSystem& other) { // Internal function for move constructor and move operator.
	rootDirectory = &other.getRootDirectory();
	workingDirectory = &other.getWorkingDirectory();
	other.setWorkingDirectory(nullptr);
	//the next line complete the stealing
	other.rootDirectory = nullptr;
}

void FileSystem::copy(const FileSystem& other) {
	rootDirectory = new Directory(other.getRootDirectory()); //Creating a new root and setting it accordingly using the copy assignment operator.
	string path = other.getWorkingDirectory().getAbsolutePath(); //Locating the exact absolute path of the working directory, locating it within the new root directory and set it accordinlgy. 
	vector<string> pathVector = BaseCommand::stringIntoVector(path, '/');
	Directory* current = rootDirectory;
	int pathSize = pathVector.size();
	for (int index = 0; index < pathSize; index++) {
		string next = pathVector.at(index);
		vector<BaseFile*> children = current->getChildren(); //Extracting the children vector from the current working dir
		bool isFoundLocalDir = false;
		int childrenSize = children.size();
		for (int index2 = 0; index2 < childrenSize && !isFoundLocalDir; index2++) { //Going through the children's vector in order to find the next dirdctory in path
			if (children.at(index2)->getName() == next && children.at(index2)->getType() == 1) {
				isFoundLocalDir = true;
				current = (Directory*)children.at(index2);
			}
		}
	}
	workingDirectory = (Directory*)current; // In case we have found the requested file/directory returning it.
}

void FileSystem::remove() { // // Internal function for destructor. 
	delete rootDirectory; // Recursively deleting the entire root and its directories, afterwards setting the root and working directories as nullptr.
	rootDirectory = nullptr;
	workingDirectory = nullptr;
}
