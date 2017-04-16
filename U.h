// Name: Maxwell You
// Date: 2017-03-27
// Purpose: Implement program that read Unicode properties from a properties file and reads input files; implemented as a class

#ifndef U_H
#define U_H

#include <fstream>
#include <map>
#include <set>
#include <string>

class U {

	public:

		// Ctors
		U();
		U(const U &rhs);
		U(std::string filename);

		// Assignment operator=
		const U &operator=(const U &rhs);

		// Dtor
		~U();

		// Error check ifstream for failure
		void streamFail(std::ifstream &in, int byteNum, std::string filename);

		// Error check for invalid continuation byte
		void contByteFail(int byte, std::string filename);

		// Modified convUTF for reading from a stream and checking valid chars
		void readUTF(int byte1, std::ifstream &in, std::string filename);

		// Read characters from an input file
		void readfile(std::string filename);

		// Adds string to accumulated string
		void append(std::string extra);

		// State how many characters read thus far
		int size() const;

		// Get all chars read thus far
		std::string get() const;

		// Get char at index
		std::string get(int index) const;

		// Determines byte length of character
		int bytes(int index) const;

		// Get chars from start to end
		std::string get(int start, int end) const;

		// Convert UTF8 encoding to Unicode
		int convUTF(int byte1, std::string charac) const;

		// Return the Unicode codepoint at the index in the accumulated string
		int codepoint(int index) const;

		// Return true if accumulated string is empty
		bool empty() const;

		// Removes all data from the obj
		void clear();

	private:

		std::string charsRead; 		// stores all characters read thus far
};

#endif
