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
		U(std::string propfile, std::string data);

		// Assignment operator=
		const U &operator=(const U &rhs);

		// Dtor
		~U();

		// decodes UTF8 into Unicode
		int convUTF(int byte1, std::string charsRead, unsigned int &index) const;

		// modified convUTF for reading from a stream and checking valid chars
		void readUTF(int byte1, std::ifstream &in, std::string filename);

		// Read characters from an input file
		void readfile(std::string filename);

		// Parse and store properties from a properties file
		void propfile(std::string filename);

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

		// Find occurence of a property
		int propcount(std::string prop) const;

		// State all possible properties
		std::set<std::string> props() const;

	private:

		std::string charsRead; 		// stores all characters read thus far
		std::map<int, std::string> propsMap;	// stores unicode and properties from propfile
		std::set<std::string> propsSet;	// stores properties from propfile
		bool propOpened = false;	// bool to ensure propfile is only called once
};

#endif
