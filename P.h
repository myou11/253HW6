// Name: Maxwell You
// Date: 2017-04-15
// Purpose: Handle properties

#ifndef P_H
#define P_H

#include <map>
#include <set>
#include <string>

class P {
	public:
		// Ctors
		P();				// default
		P(const P &rhs);	// copy
		P(std::string);		// takes a string
		
		// Assignment operator=
		const P &operator=(const P &rhs);

		// Dtor
		~P();

		// Reads property information from a file
		void readfile(std::string filename);

		// Returns all possible properties
		std::set<std::string> props() const;

		// Increments running count of prop assoc with the codepoint (if prop exists) 
		void count(int codepoint);

		// Returns occurences of times that characters with prop have been encountered
		int count(std::string prop) const;

		// Returns number of unique properties read
		int size() const;

		// Returns true if no property names
		bool empty() const;

		// Removes all data from the obj
		void clear();

	private:
		std::map<int, std::string> propsMap;	// stores unicode and properties from propfile
		std::map<std::string, int> countMap;	// stores occurences of the properties
		std::set<std::string> propsSet;			// stores properties from propfile
		bool propOpened = false;				// bool to ensure only one properties file is read
};
#endif
