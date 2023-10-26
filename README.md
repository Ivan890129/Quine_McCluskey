# Quine_McCluskey
all:Quine_McCluskey.cpp

	g++ -std=c++11 Quine_McCluskey.cpp -o Quine_McCluskey.o

clean:

	rm -f Quine_McCluskey.o


./Quine_McCluskey.cpp "input1.txt" "output.txt"
