#include "Environment.h"
#include "GlobalVariables.h"

Environment::Environment() : commandsHistory(), fs() {} // Constructor.
Environment::Environment(const Environment& other) : commandsHistory(), fs() { // Copy Constructor.
	if (verbose == 1 || verbose == 3) {
		cout << "Environment::Environment(const Environment& other)" << endl;
	}
	copy(other);
}
Environment& Environment::operator=(const Environment& other) { // Copy Assignment Operator.
	if (verbose == 1 || verbose == 3) {
		cout << "Environment& Environment::operator=(const Environment& other)" << endl;
	}
	if (this == &other) {
		return *this;
	}
	remove();
	copy(other);
	return *this;
}
Environment& Environment:: operator=(Environment&& other) { // Move Assignment Operator. 
	if (verbose == 1 || verbose == 3) {
		cout << "Environment& Environment:: operator=(Environment&& other)" << endl;
	}
	remove();
	steal(other);
	return *this;
}
Environment::Environment(Environment&& other) : commandsHistory(), fs() { // Move Constructor.
	if (verbose == 1 || verbose == 3) {
		cout << "Environment::Environment(Environment&& other)" << endl;
	}
	steal(other);
}
Environment:: ~Environment() { // Destructor.
	if (verbose == 1 || verbose == 3) {
		cout << "Environment:: ~Environment()" << endl;
	}
	remove();
	commandsHistory.clear();
}
void Environment::start() {
	string input;
	cout << fs.getWorkingDirectory().getAbsolutePath() << ">"; // Printing the working directory's absolute path as requested.
	getline(cin, input);
	while (input != "exit") { //Receving input from the end-user untill the input will be "exit":
		int tempIndex = input.find_first_of(' ');
		bool isSpaceExists = (tempIndex != -1);
		string commandName; 
		string args; //Will receive the command's args without the command's name.
		if (isSpaceExists) {
			commandName = input.substr(0, input.find_first_of(' '));
			args = input.substr(input.find_first_of(' ') + 1);
		}
		else {
			commandName = input;
			args = "";
		}
		BaseCommand* commandType;
		if (commandName == "pwd") {
			commandType = new PwdCommand(args);
		}
		else if (commandName == "cd") {
			commandType =new CdCommand(args);
		}
		else if (commandName == "ls") {
			commandType =new LsCommand(args);
		}
		else if (commandName == "mkdir") {
			commandType =new MkdirCommand(args);
		}
		else if (commandName == "mkfile") {
			commandType =new MkfileCommand(args);
		}
		else if (commandName == "cp") {
			commandType =new CpCommand(args);
		}
		else if (commandName == "mv") {
			commandType=new MvCommand(args);
		}
		else if (commandName == "rename") {
			commandType=new RenameCommand(args);
		}
		else if (commandName == "rm") {
			commandType =new RmCommand(args);
		}
		else if (commandName == "history") {
			commandType= new HistoryCommand("", getHistory());

		}
		else if (commandName == "verbose") {
			commandType=new VerboseCommand(args);
		}
		else if (commandName == "exec") {
			commandType =new ExecCommand(args, getHistory());
		}
		else {
			commandType =new ErrorCommand(input);
		}
		commandType->execute(fs);
		addToHistory(commandType);
		cout << fs.getWorkingDirectory().getAbsolutePath() << ">"; // Printing the working directory's absolute path as requested.
		getline(cin, input);
	}
	return;
}
FileSystem& Environment::getFileSystem() { // Get a reference to the file system
	return fs;
}
void Environment::addToHistory(BaseCommand *command) { // Add a new command to the history
	commandsHistory.push_back(command);
}
const  vector<BaseCommand*>& Environment::getHistory() const { // Return a reference to the history of commands
	return commandsHistory;
}

void Environment::steal(Environment& other) { // Internal function for move constructor and move operator.
	fs = other.getFileSystem();
	commandsHistory = other.getHistory();
	delete &fs;
	vector<BaseCommand*> otherVector;
	other.commandsHistory = otherVector;
}

void Environment::copy(const Environment& other) { // Internal function for copy constructor and copy operator.
	fs = other.fs;
	int size = other.commandsHistory.size();
	for (int index = 0; index < size; index++) {
		commandsHistory.push_back(other.commandsHistory[index]->clone());
	}
}

void Environment::remove() { // // Internal function for destructor. 
	int size = commandsHistory.size();
	for (int index = 0; index < size; index++) { // Deleting the content of the commandHistory.
		delete commandsHistory[index];
		commandsHistory[index] = nullptr;
	}
}


