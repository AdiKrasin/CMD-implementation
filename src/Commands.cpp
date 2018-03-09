#include "Commands.h"
#include "GlobalVariables.h"
#include "Files.h"
#include "FileSystem.h"
using namespace std;

//BaseCommand// 
BaseCommand::BaseCommand(string _args) : args(_args) {} // Constructor
BaseCommand::~BaseCommand() {} // Virtual Destructor.


string BaseCommand::getArgs() {
	return args;
}

vector<string> BaseCommand::stringIntoVector(string s, char c) { //The following function receives the args and turns it into a vector of strings
	vector<string> vector;
	string next = "";
	for (unsigned int index = 0; index <= s.length(); index++) {
		if (s[index] == c || index == s.length()) {
			if (next != "") {
				vector.push_back(next);
				next = "";
			}
		}
		else {
			next = next + s[index];
		}
	}
	return vector;
}

BaseFile* BaseCommand::lastInPath2(string s, FileSystem & fs) { //This function returns the directory/file which is the last in path
	Directory * originalDir = &fs.getWorkingDirectory();
	if (s == "") {
		return &fs.getWorkingDirectory();
	}
	else {
		vector<string> pathVector = stringIntoVector(s, '/');
		if (s.at(0) == '/') {// Absolute path
			fs.setWorkingDirectory(&fs.getRootDirectory());
			if (s.length() == 1) { // Only "/" in args
				fs.setWorkingDirectory(originalDir);
				return &fs.getRootDirectory();
			}
		}
		bool stillSearching = true; //A bool which will indicates whether or not we are still searching
		int size = pathVector.size();
		for (int index = 0; index < (size - 1) && stillSearching; index++) {
			string next = pathVector.at(index);
			if (next == "..") { // Checking the dots. 
				if (fs.getWorkingDirectory().getParent() == nullptr) { // If we have reached the root, reverting the changes and print the proper message accordingly.
					cout << "No such file or directory" << endl;
					stillSearching = false;
					fs.setWorkingDirectory(originalDir);
					return nullptr;
				}
				else { // Setting the working directory to its parent.
					fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
				}
			}
			else { // in case there are no ".."
				vector<BaseFile*> children = fs.getWorkingDirectory().getChildren(); //Extracting the children vector from the current working dir
				bool isFoundLocalDir = false;
				int size = children.size();
				for (int index2 = 0; index2 < size && stillSearching && !isFoundLocalDir; index2++) { //going through the children's vector in order to find the next dirdctory in path
					if (children.at(index2)->getName() == next && children.at(index2)->getType() == 1) {
						isFoundLocalDir = true;
						fs.setWorkingDirectory(((Directory*)(children.at(index2))));
					}
				}
				if (isFoundLocalDir == false) { // in case the path is illegal (the next directory in the path is not found) printing the proper message
					cout << "No such file or directory" << endl;
					stillSearching = false;
					fs.setWorkingDirectory(originalDir);
					return nullptr;
				}
			}
		}
		if (stillSearching) { //looking for the last directory/file (the one we would return)
			bool foundTheOne = false;
			string theNameOfTheOne = pathVector.at(pathVector.size() - 1);
			if (theNameOfTheOne == "..") { // Case 1: .. in the end:
				if (fs.getWorkingDirectory().getParent() == nullptr) { // If we have reached the root, reverting the changes and print the proper message accordingly.
					cout << "No such file or directory" << endl;
					stillSearching = false;
					fs.setWorkingDirectory(originalDir);
					return nullptr;
				}
				else { // Setting the working directory to its parent.
					Directory * directoryToReturn = (&fs.getWorkingDirectory())->getParent();
					fs.setWorkingDirectory(originalDir);
					return directoryToReturn; // In case we have found the requested file/directory returning it.		
				}
			}
			else { // Case 2: NOT .. in the end:
				vector<BaseFile*> children = fs.getWorkingDirectory().getChildren(); //Extracting the children vector from the current working dir (the one before the last one which should be returned)
				int size = children.size();
				for (int index3 = 0; index3 < size && !foundTheOne && stillSearching; index3++) {
					if (children.at(index3)->getName() == theNameOfTheOne) {
						stillSearching = false;
						foundTheOne = true;
						fs.setWorkingDirectory((Directory*)(children.at(index3)));
						Directory * directoryToReturn = &fs.getWorkingDirectory();
						fs.setWorkingDirectory(originalDir);
						return directoryToReturn; // In case we have found the requested file/directory returning it.				
					}
				}
			}
		}
		fs.setWorkingDirectory(originalDir);
		cout << "No such file or directory" << endl; // In case we did not find the last directory/file the proper message will be printed.
		return nullptr; // In case we did not find the last directory/file we will return nullptr
	}
}


BaseFile* BaseCommand::lastInPath(string s, FileSystem & fs) { //This function returns the directory/file which is the last in path
	Directory * originalDir = &fs.getWorkingDirectory();
	if (s == "") {
		return &fs.getWorkingDirectory();
	}
	else {
		vector<string> pathVector = stringIntoVector(s, '/');
		if (s.at(0) == '/') {// Absolute path
			fs.setWorkingDirectory(&fs.getRootDirectory());
			if (s.length() == 1) { // Only "/" in args
				fs.setWorkingDirectory(originalDir);
				return &fs.getRootDirectory();
			}
		}
		bool stillSearching = true; //A bool which will indicates whether or not we are still searching
		int size = pathVector.size();
		for (int index = 0; index < (size - 1) && stillSearching; index++) {
			string next = pathVector.at(index);
			if (next == "..") { // Checking the dots. 
				if (fs.getWorkingDirectory().getParent() == nullptr) { // If we have reached the root, reverting the changes and print the proper message accordingly.
					cout << "The system cannot find the path specified" << endl;
					stillSearching = false;
					fs.setWorkingDirectory(originalDir);
					return nullptr;
				}
				else { // Setting the working directory to its parent.
					fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
				}
			}
			else { // in case there are no ".."
				vector<BaseFile*> children = fs.getWorkingDirectory().getChildren(); //Extracting the children vector from the current working dir
				bool isFoundLocalDir = false;
				int size = children.size();
				for (int index2 = 0; index2 < size && stillSearching && !isFoundLocalDir; index2++) { //going through the children's vector in order to find the next dirdctory in path
					if (children.at(index2)->getName() == next && children.at(index2)->getType() == 1) {
						isFoundLocalDir = true;
						fs.setWorkingDirectory(((Directory*)(children.at(index2))));
					}
				}
				if (isFoundLocalDir == false) { // in case the path is illegal (the next directory in the path is not found) printing the proper message
					cout << "The system cannot find the path specified" << endl;
					stillSearching = false;
					fs.setWorkingDirectory(originalDir);
					return nullptr;
				}
			}
		}
		if (stillSearching) { //looking for the last directory/file (the one we would return)
			bool foundTheOne = false;
			string theNameOfTheOne = pathVector.at(pathVector.size() - 1);
			if (theNameOfTheOne == "..") {  // Case 1: .. in the end:
				if (fs.getWorkingDirectory().getParent() == nullptr) { // If we have reached the root, reverting the changes and print the proper message accordingly.
					cout << "The system cannot find the path specified" << endl;
					stillSearching = false;
					fs.setWorkingDirectory(originalDir);
					return nullptr;
				}
				else { // Setting the working directory to its parent.
					Directory * directoryToReturn = (&fs.getWorkingDirectory())->getParent();
					fs.setWorkingDirectory(originalDir);
					return directoryToReturn; // In case we have found the requested file/directory returning it.		
				}
			}
			else { // Case 2: NOT .. in the end:
				vector<BaseFile*> children = fs.getWorkingDirectory().getChildren(); //Extracting the children vector from the current working dir (the one before the last one which should be returned)
				int size = children.size();
				for (int index3 = 0; index3 < size && !foundTheOne && stillSearching; index3++) {
					if (children.at(index3)->getName() == theNameOfTheOne) {
						stillSearching = false;
						foundTheOne = true;
						fs.setWorkingDirectory((Directory*)(children.at(index3)));
						Directory * directoryToReturn = &fs.getWorkingDirectory();
						fs.setWorkingDirectory(originalDir);
						return directoryToReturn; // In case we have found the requested file/directory returning it.				
					}
				}
			}
		}
			fs.setWorkingDirectory(originalDir);
			cout << "The system cannot find the path specified" << endl; // In case we did not find the last directory/file the proper message will be printed.
			return nullptr; // In case we did not find the last directory/file we will return nullptr
	}
}


//PwdCommand//

PwdCommand::PwdCommand(string args) : BaseCommand(args) {} // Constructor
void PwdCommand::execute(FileSystem & fs) { // Printing the working directory path.
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() { return "pwd"; }

BaseCommand* PwdCommand::clone() {
	return new PwdCommand(*this);
}

//CdCommand//

CdCommand::CdCommand(string args) : BaseCommand(args) {} // Constructor

string CdCommand::toString() { return "cd"; }

BaseCommand* CdCommand::clone() {
	return new CdCommand(*this);
}

void CdCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	BaseFile* theNewOne = lastInPath(this->getArgs(), fs); //Searching for the directory we would like to change the working directory to
	if (theNewOne != nullptr && theNewOne->getType() == 1) { // In case it's a directory
		
		fs.setWorkingDirectory((Directory*)(theNewOne));
	}
	if (theNewOne != nullptr && theNewOne->getType() == 0) { // In case it's a file
		cout << "The system cannot find the path specified" << endl;
	}
}

//LsCommand//

LsCommand::LsCommand(string args) : BaseCommand(args) {} // Constructor

string LsCommand::toString() { return "ls"; }

BaseCommand* LsCommand::clone() {
	return new LsCommand(*this);
}

void LsCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	BaseFile* theNewOne;
	bool sortBySize = false;
	string args = this->getArgs();
	if (args == "") { // Case 1: Working directory with empty args:
		theNewOne = &fs.getWorkingDirectory();
	}
	else { // Case 2:  The args are not empty:
		if (args.at(0) == '-' && args.at(1) == 's') { // Checking for the existance of '-s', setting the bool and removeing the additional spaces after it. 
			sortBySize = true;
			if (args.size()==2) { // Case 2.1: args contains only -s
				args = args.substr(2);
			}
			else { // Case 2.2: args does not contain only -s
				args = args.substr(3);
			}
		}
		if (args == "") { // Case 3: -s + empty args:
			theNewOne = &fs.getWorkingDirectory();
		}
		else { // Case 4: -s + args:
			theNewOne = lastInPath(args, fs); //Searching for the directory we would like print it's children
		}
	}

	if (theNewOne != nullptr && theNewOne->getType() == 1) { // In case we have found the directory and it's in fact a directory:
		if (sortBySize) { // In case we should sort by size
			((Directory*)theNewOne)->sortBySize();
		}
		else {
			((Directory*)theNewOne)->sortByName();
		}
		if (theNewOne != nullptr && theNewOne->getType() == 1) {
			std::vector<BaseFile*> children = ((Directory*)theNewOne)->getChildren(); // Printing the children as requested.
			int size = children.size();
			for (int index = 0; index < size; index++) {
				string output;
				if (children.at(index)->getType() == 1) {
					cout << "DIR\t" << children.at(index)->getName();
				}
				else {
					cout << "FILE\t" << children.at(index)->getName();
				}
				cout << "\t" << children.at(index)->getSize() << endl;
			}
		}
	}
		if (theNewOne != nullptr && theNewOne->getType() == 0) { // In case we have found the "directory" but it happened to be a file
			cout << "The system cannot find the path specified" << endl;
		}
	}



//MkdirCommand//

MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {}  // Constructor

string MkdirCommand::toString() { return "mkdir"; }

BaseCommand* MkdirCommand::clone() {
	return new MkdirCommand(*this);
}

void MkdirCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	bool didSomething = false;
	string s = this->getArgs();
	Directory* originalDir = &fs.getWorkingDirectory();
	vector<string> pathVector = stringIntoVector(s, '/');
		if (s.at(0) == '/') {// Absolute path
			fs.setWorkingDirectory(&fs.getRootDirectory());
		}
	bool stillSearching = true; //A bool which will indicates whether or not we are still searching
	int size = pathVector.size();
	for (int index = 0; index < size && stillSearching; index++) {
		string next = pathVector.at(index);
		if (next == "..") { // Checking the dots. 
			if (fs.getWorkingDirectory().getParent() == nullptr) { // If we have reached the root, reverting the changes and print the proper message accordingly.
				cout << "The system cannot find the path specified" << endl;
				stillSearching = false;
			}
			else { // Setting the working directory to its parent.
				fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
			}
		}
		else { // in case there are no ".."
			vector<BaseFile*> children = fs.getWorkingDirectory().getChildren(); //Extracting the children vector from the current working dir
			bool isFoundLocalDir = false;
			int size = children.size();
			for (int index2 = 0; index2 < size && stillSearching && !isFoundLocalDir; index2++) { //going through the children's vector in order to find the next directory in path
				if (children.at(index2)->getName() == next && children.at(index2)->getType() == 1) { // Directory 
					isFoundLocalDir = true;
					fs.setWorkingDirectory(((Directory*)(children.at(index2))));
				}
				if (children.at(index2)->getName() == next && children.at(index2)->getType() == 0) { // Specific case where the name of the directory already exists within the same directory, and it's a file
					cout << "The directory already exists" << endl;
					isFoundLocalDir = true;
					stillSearching = false;
					didSomething = true;
				}
			}
			if (isFoundLocalDir == false) { // If we have not found the relevant dir within the children vector, the path cannot be found, therefore creating a new Directory and pointing the directory to it accordingly.
				Directory* current = new Directory(next, &fs.getWorkingDirectory());
				int originalSizeOfChildren =fs.getWorkingDirectory().getChildren().size();
				fs.getWorkingDirectory().addFile(current);
				int currentSizeOfChildren = fs.getWorkingDirectory().getChildren().size();
				if (originalSizeOfChildren == currentSizeOfChildren) {
					delete current;
				}
				fs.setWorkingDirectory(current);
				didSomething = true;
			}
		}
	}
	if (!didSomething) {
		cout << "The directory already exists" << endl;
	}
	fs.setWorkingDirectory(originalDir); // As requested in the example, setting the workingDirectory to the original working directory, once finished.
}

//MkfileCommand//

MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {}

string MkfileCommand::toString() { return "mkfile"; }

BaseCommand* MkfileCommand::clone() {
	return new MkfileCommand(*this);
}

void MkfileCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	string s = this->getArgs(); // args
	int index = s.find_last_of(" "); // Locating the last space and removing the size's string from the args.
	int size = s.size(); // size of args
	string stringOfSize = "";
	for (int j = index + 1; j < size; j++) { // merging the chars of size. 
		stringOfSize = stringOfSize + s.at(j);
	}
	s = s.substr(0, index);  // removing the size's chars from the args. 
	int stoiSize = stoi(stringOfSize); // Converting the string of size to int.
	int index2 = s.find_last_of("/");
	bool onlySpace = (index2 == (-1));
	if (!onlySpace) { // There are "/".
		string nameOfFile = s.substr(index2 + 1);
		s = s.substr(0, index2);
		BaseFile* theOneBeforeTheLastOne = lastInPath(s, fs); //Searching for the directory we would like to add the new File into it's children vector
		if (theOneBeforeTheLastOne != nullptr && theOneBeforeTheLastOne->getType() == 1) {
			bool foundTheOne = false;
			vector<BaseFile*> children = ((Directory*)(theOneBeforeTheLastOne))->getChildren(); //Extracting the children vector from the directory which is the one before the file which should be entered to it's children's vector
			int size = children.size();
			for (int index3 = 0; index3 < size && !foundTheOne; index3++) { // Locating the requested file. 
				if (children.at(index3)->getName() == nameOfFile) {
					foundTheOne = true;
					cout << "File already exists" << endl;
				}
			}
			if (!foundTheOne) { // If we have not found it, creating new file. 
				File* newFile = new File(nameOfFile, stoiSize);
				int originalSizeOfChildren = ((Directory*)(theOneBeforeTheLastOne))->getChildren().size();
				((Directory*)(theOneBeforeTheLastOne))->addFile(newFile);
				int currentSizeOfChildren = ((Directory*)(theOneBeforeTheLastOne))->getChildren().size();
				if (originalSizeOfChildren == currentSizeOfChildren) {
					delete newFile;
				}
			}
		}
	}
	else { // There are no "/" at all.
		BaseFile* theOneBeforeTheLastOne = &fs.getWorkingDirectory();
		string nameOfFile = s;
		bool foundTheOne = false;
		vector<BaseFile*> children = ((Directory*)(theOneBeforeTheLastOne))->getChildren(); //Extracting the children vector from the directory which is the one before the file which should be entered to it's children's vector
		int size = children.size();
		for (int index4 = 0; index4 < size && !foundTheOne; index4++) { // Locating the requested file. 
			if (children.at(index4)->getName() == nameOfFile) {
				foundTheOne = true;
				cout << "File already exists" << endl;
			}
		}
		if (!foundTheOne) { // If we have not found it, creating new file. 
			File* newFile = new File(nameOfFile, stoiSize);
			int originalSizeOfChildren = ((Directory*)(theOneBeforeTheLastOne))->getChildren().size();
			((Directory*)(theOneBeforeTheLastOne))->addFile(newFile);
			int currentSizeOfChildren = ((Directory*)(theOneBeforeTheLastOne))->getChildren().size();
			if (originalSizeOfChildren == currentSizeOfChildren) {
				delete newFile;
			}
		}
	}
}

//CpCommand//

CpCommand::CpCommand(string args) :BaseCommand(args) {}

string CpCommand::toString() { return "cp"; }

BaseCommand* CpCommand::clone() {
	return new CpCommand(*this);
}

void CpCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	string source = this->getArgs();
	int index = source.find_last_of(" "); // Locating the last space and removing the destination path's string from the args.
	string destination = "";
	int size = source.size();
	for (int j = index + 1; j < size; j++) {
		destination = destination + source.at(j);
	}
	source = source.substr(0, index);
	BaseFile* lastInSource = lastInPath2(source, fs); //Searching for the directory we would like to add the new File into it's children vector
	if (lastInSource != nullptr) {
		BaseFile* lastInDestination = lastInPath2(destination, fs); //Searching for the directory/file we would like to add into the source's children's vector
		if (lastInDestination != nullptr && lastInDestination->getType() == 1 && lastInSource != nullptr) {
			if (lastInSource->getType() == 0) {
				File * FileToCopy = new File(lastInSource->getName(), lastInSource->getSize());
				int originalSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
				((Directory*)lastInDestination)->addFile(FileToCopy);
				int currentSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
				if (originalSizeOfChildren == currentSizeOfChildren) {
					delete FileToCopy;
				}
			}
			else {
				Directory * DirectoryToCopy = new Directory(*((Directory*)lastInSource));
				int originalSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
				((Directory*)lastInDestination)->addFile(DirectoryToCopy);
				int currentSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
				if (originalSizeOfChildren == currentSizeOfChildren) {
					delete DirectoryToCopy;
				}
			}
		}
	}
}

//MvCommand//

MvCommand::MvCommand(string args) : BaseCommand(args) {}

string MvCommand::toString() { return "mv"; }

BaseCommand* MvCommand::clone() {
	return new MvCommand(*this);
}

void MvCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	bool validMove = true;
	string source = this->getArgs();
	int index = source.find_last_of(" "); // Locating the last space and removing the destination path's string from the args.
	int sourceSize = source.size();
	bool doublePrint = false;
	string destination = "";
	for (int j = index+1; j < sourceSize; j++) {
		destination = destination + source.at(j);
	}
	BaseFile* theOneBeforeTheLastOneInSource = nullptr;
	source = source.substr(0, index); // Excluding the destianiton from the source. 
	File * FileToMove = nullptr;
	Directory * DirectoryToMove = nullptr;
	BaseFile* lastInSource = lastInPath2(source, fs); //Searching for the directory we would like to add the new File into it's children vector
	if (lastInSource != nullptr) {
		BaseFile* lastInDestination = lastInPath2(destination, fs); //Searching for the directory/file we would like to add into the source's children's vector
		if (lastInDestination != nullptr && lastInDestination->getType() == 1 && lastInSource != nullptr) {
			if (lastInSource->getType() == 0) { // In case it is a valid move adding the file to it's destination.
				FileToMove = new File(lastInSource->getName(), lastInSource->getSize());
				int originalSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
				((Directory*)lastInDestination)->addFile(FileToMove);
				int currentSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
				if (originalSizeOfChildren == currentSizeOfChildren) {
					delete FileToMove;
				}
				int index2 = source.find_last_of("/");
				if (index2 != (-1)) {
					source = source.substr(0, index2);
					theOneBeforeTheLastOneInSource = lastInPath2(source, fs); //Searching for the directory that is the parent of the base file we would like to remove
				}
				else {
					theOneBeforeTheLastOneInSource = &fs.getWorkingDirectory();
				}
				((Directory*)(theOneBeforeTheLastOneInSource))->removeFile(lastInSource);
			}
			else { // The file which is about to be moved is a directory:
				if (lastInSource == &fs.getWorkingDirectory() || lastInSource == &fs.getRootDirectory() || lastInSource == fs.getWorkingDirectory().getParent()) { //Verifying it's a valid move according to the instrucations
					validMove = false;
					doublePrint = true;
					cout << "Can't move directory" << endl;
				}
				if (validMove) {
					Directory * workingDirectory = &fs.getWorkingDirectory();
					while (workingDirectory != &fs.getRootDirectory()) { //Verifying it's a valid move according to the instrucations
						if (workingDirectory == lastInSource) {
							validMove = false;
						}
						else {
							workingDirectory = workingDirectory->getParent();
						}
					}
				}

				if (!validMove && doublePrint==false) { // In case it's not a valid move printing the proper message
					cout << "Can't move directory" << endl;
				}
				else { // In case it is a valid move, moving the directory to it's destination
					DirectoryToMove = new Directory(*((Directory*)lastInSource));
					int index3 = source.find_last_of("/");
					if (index3 != (-1)) {
						source = source.substr(0, index3);
						theOneBeforeTheLastOneInSource = lastInPath2(source, fs); //Searching for the directory that is the parent of the base file we would like to remove
					}
					else {
						theOneBeforeTheLastOneInSource = &fs.getWorkingDirectory();
					}

					int originalSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
					((Directory*)(lastInDestination))->addFile(DirectoryToMove);
					int currentSizeOfChildren = ((Directory*)(lastInDestination))->getChildren().size();
					if (originalSizeOfChildren == currentSizeOfChildren) {
						delete DirectoryToMove;
					}
					((Directory*)(theOneBeforeTheLastOneInSource))->removeFile(lastInSource);
				}
			}
		}
	}
}

//RenameCommand//

RenameCommand::RenameCommand(string args) : BaseCommand(args) {}

string RenameCommand::toString() { return "rename"; }

BaseCommand* RenameCommand::clone() {
	return new RenameCommand(*this);
}

void RenameCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	string s = this->getArgs(); // args.
	int index = s.find_last_of(" "); // Locating the last space and removing the new name's string from the args.
	int size = s.size(); // args' size.
	string newName = ""; 
	for (int j = index + 1; j < size; j++) { // Merging the newName's chars. 
		newName = newName + s.at(j);
	}
	s = s.substr(0, index);
	BaseFile* baseFile = lastInPath2(s, fs); //Searching for the base file we would like to change the name of
	if (baseFile != nullptr) { // Checking whether basefile was found:
		bool nameExsists = false;
		int index2 = s.find_last_of("/");
		bool onlySpace = (index2 == (-1));
		if (!onlySpace) { // There are "/".
			s = s.substr(0, index2);
			BaseFile* theOneBeforeTheLastOne = lastInPath2(s, fs); //Searching for the directory that is the parent of the base file we would like to change it's name
			vector<BaseFile*> children = ((Directory*)theOneBeforeTheLastOne)->getChildren(); //Extracting the children vector and locating the name. 
			int size = children.size();
			for (int index = 0; index < size; index++) {
				if (children.at(index)->getName() == newName) {
					nameExsists = true;
				}
			}
			if (!nameExsists) { // If we have not found the name, renaming accordingly:
				if (baseFile->getType() == 1) {
					nameExsists = (((Directory*)baseFile) == &fs.getWorkingDirectory()); // In case the baseFile is a directory, verifying that it is not the working directory. 
				}
				if (!nameExsists) { // If it is not the working directory, it is another directory or file, therefore changing the name. 
					baseFile->setName(newName);
				}
				else {
					cout << "Can't rename the working directory" << endl;
				}
			}
		}
		else { // There are no "/"
			BaseFile* theOneBeforeTheLastOne = &fs.getWorkingDirectory();
			vector<BaseFile*> children = ((Directory*)theOneBeforeTheLastOne)->getChildren(); //Extracting the children vector and locating the name. 
			int size = children.size();
			for (int index = 0; index < size; index++) {
				if (children.at(index)->getName() == newName) {
					nameExsists = true;
				}
			}
			if (!nameExsists) { // If we have not found the name, renaming accordingly:
				if (baseFile->getType() == 1) {
					nameExsists = (((Directory*)baseFile) == &fs.getWorkingDirectory()); // In case the baseFile is a directory, verifying that it is not the working directory. 
				}
				if (!nameExsists) { // If it is not the working directory, it is another directory or file, therefore changing the name. 
					baseFile->setName(newName);
				}
				else {
					cout << "Can't rename the working directory" << endl;
				}
			}
		}
	}
}

//RmCommand//

RmCommand::RmCommand(string args) : BaseCommand(args) {}

string RmCommand::toString() { return "rm"; }

BaseCommand* RmCommand::clone() {
	return new RmCommand(*this);
}

void RmCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	string s = this->getArgs();
	BaseFile* baseFile = lastInPath2(s, fs); //Searching for the base file we would like to delete
	if (baseFile != nullptr) {
		if (baseFile->getType() == 1) {
			if (((Directory*)baseFile) == &fs.getWorkingDirectory() || ((Directory*)baseFile) == &fs.getRootDirectory()) {
				cout << "Can't remove directory" << endl;
			}
			else {
				int index = s.find_last_of("/");
				bool onlySpace = (index == (-1));
				if (!onlySpace) { // There are "/".
					s = s.substr(0, index);
					BaseFile* theOneBeforeTheLastOne = lastInPath2(s, fs); //Searching for the directory that is the parent of the base file we would like to change it's name
					((Directory*)(theOneBeforeTheLastOne))->removeFile(baseFile);
				}
				else { // There are no "/" at all.
					(&fs.getWorkingDirectory())->removeFile(baseFile);
				}
			}
		}
		else { // The base file is a File. 
			int index2 = s.find_last_of("/");
			bool onlySpace = (index2 == (-1));
			if (!onlySpace) { // There are "/".
				s = s.substr(0, index2);
				BaseFile* theOneBeforeTheLastOne = lastInPath2(s, fs); //Searching for the directory that is the parent of the base file we would like to change it's name
				vector<BaseFile*> children = ((Directory*)theOneBeforeTheLastOne)->getChildren();
				((Directory*)(theOneBeforeTheLastOne))->removeFile(baseFile);
			}
			else { // There are no "/" at all.
				(&fs.getWorkingDirectory())->removeFile(baseFile);
			}
		}
	}
}

//HistoryCommand//

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}

void HistoryCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	int size = history.size();
	for (int index = 0; index < size; index++) {
		cout << index << "\t"  << history[index]->toString() << " " << history[index]->getArgs() << endl;
	}
}
string HistoryCommand::toString() { return "history"; }
BaseCommand* HistoryCommand::clone() {
	return new HistoryCommand(*this);
}

//VerboseCommand//

VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {}
void VerboseCommand::execute(FileSystem & fs) {
	string args = this->getArgs();
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << args << endl;
	}
	verbose = stoi(args);
	if (!(verbose == 0 || verbose == 1 || verbose == 2 || verbose == 3)) { // In case the verbose is not 0,1,2,3 printing the proper message.
		cout << "Wrong verbose input" << endl;
	}
}
string VerboseCommand::toString() { return "verbose"; }

BaseCommand* VerboseCommand::clone() {
	return new VerboseCommand(*this);
}

//ErrorCommand//

ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {}

void ErrorCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	string args = this->getArgs(); //Extracting the args.
	string commandName = args.substr(0, args.find_first_of(' '));
	cout << commandName << ": Unknown command" << endl;
}

string ErrorCommand::toString() { return ""; }

BaseCommand* ErrorCommand::clone() {
	return new ErrorCommand(*this);
}

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem & fs) {
	if (verbose == 2 || verbose == 3) { // If verbose is either 2 or 3, printing full input command.
		cout << this->toString() << " " << this->getArgs() << endl;
	}
	string args = this->getArgs();
	int commandNumber = stoi(args); // Converting the string to int. 
	bool isFound = false;
	int size = history.size();
	for (int index = 0; index < size && isFound == false; index++) {
		if (commandNumber == index) { // In case the commandNumber matches the index of history changing the bool and executing the command.
			isFound = true;
			history[index]->execute(fs);
		}
	}
	if (isFound == false) { // In case we have not found the command number within the history, printing the proper message.
		cout << "Command not found" << endl;
	}
}
string ExecCommand::toString() { return "exec"; }

BaseCommand* ExecCommand::clone() {
	return new ExecCommand(*this);
}