#All Targets
all: fs

#Executable "fs"
fs: bin/Main.o bin/Environment.o bin/Files.o bin/FileSystem.o bin/Commands.o
	@echo 'Building target:fs'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/fs bin/Main.o bin/Environment.o bin/Files.o bin/FileSystem.o bin/Commands.o
	@echo 'Finished building target: fs'
	@echo ' '

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp

bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp

bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp

#Clean the build directory
clean:
	rm -f bin/*
