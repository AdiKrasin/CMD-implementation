#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
using namespace std;
#include "FileSystem.h"
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Files.h"



class BaseCommand {
private:
	string args;

public:
	BaseCommand(string args);
	string getArgs();
	virtual void execute(FileSystem & fs) = 0;
	virtual string toString() = 0;
	static vector<string> stringIntoVector(string s, char c);
	virtual BaseCommand* clone() = 0; // CopyHelper
	BaseFile* lastInPath(string s, FileSystem & fs);
	BaseFile* lastInPath2(string s, FileSystem & fs); 
	virtual ~BaseCommand();

};

class PwdCommand : public BaseCommand {
private:
public:
	PwdCommand(string args);
	void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
	virtual string toString();
	BaseCommand* clone(); // CopyHelper
};

class CdCommand : public BaseCommand {
private:
public:
	CdCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
}; 

class LsCommand : public BaseCommand {
private:
public:
	LsCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class MkdirCommand : public BaseCommand {
private:
public:
	MkdirCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class MkfileCommand : public BaseCommand {
private:
public:
	MkfileCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class CpCommand : public BaseCommand {
private:
public:
	CpCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class MvCommand : public BaseCommand {
private:
public:
	MvCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class RenameCommand : public BaseCommand {
private:
public:
	RenameCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class RmCommand : public BaseCommand {
private:
public:
	RmCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class HistoryCommand : public BaseCommand {
private:
	const vector<BaseCommand *> & history;
public:
	HistoryCommand(string args, const vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};


class VerboseCommand : public BaseCommand {
private:
public:
	VerboseCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class ErrorCommand : public BaseCommand {
private:
public:
	ErrorCommand(string args);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

class ExecCommand : public BaseCommand {
private:
	const vector<BaseCommand *> & history;
public:
	ExecCommand(string args, const vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	string toString();
	BaseCommand* clone(); // CopyHelper
};

#endif