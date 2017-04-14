// Name: Maxwell You
// Date: 2017-03-27
// Purpose: Implement program that read Unicode properties from a properties file and reads input files; implemented as a class

#include "U.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Default ctor using initializer list
U::U() : charsRead("") { }	// initialized charsRead to be empty

// Copy ctor
U::U(const U &rhs) : 
	charsRead(rhs.charsRead), propsMap(rhs.propsMap), 		// copy all rhs's class members (private vars) to
	propsSet(rhs.propsSet), propOpened(rhs.propOpened) { }	// the obj being constructed currently
																													

// Test ctor
U::U(string propFile, string data) { 
	propfile(propFile);
	charsRead += data;
}

// Assignment operator=
const U &U::operator=(const U &rhs) {
	if (this != &rhs) {		// rhs by itself is the actual object, but with the & (&rhs), it get the pointer to rhs, which is also what this is (a pointer to the object)
		charsRead = rhs.charsRead;		// assign contents of rhs's charsRead to this object
		propsMap = rhs.propsMap;		// assign contents of rhs's propsMap to this object
		propOpened = rhs.propOpened;	// assign bool val of rhs's propOpened to this object
		propsSet = rhs.propsSet;		// assign contents of rhs's propsSet to this object
	}
	return *this; // this is just a pointer, *this is the actual object
}

// Dtor
U::~U( ) { }

// Read and check for valid UTF8 characters
void U::readUTF(int byte1, ifstream &in, string filename) {
	char fbyte = byte1;	// used to concatenate first byte to the charsRead string
	char c; // to get the additional UTF bytes

	// 1 byte
	if ( (byte1 & 0x80) == 0 )
		charsRead += fbyte; // add first byte of char to charsRead

	// 2 bytes
	else if ( (byte1 & 0xE0) == 0xC0 ) {
		charsRead += fbyte;	// add first byte of char to charsRead
		
		in.get(c);
		int byte2 = c;
		if ( (byte2 & 0xC0) != 0x80 ) {
			ostringstream oss;
			oss << "0x" << hex << byte2 << " was not a valid continuation byte in file: " << filename;
			throw oss.str();
		}
		charsRead += c;
	}

	// 3 bytes
	else if ( (byte1 & 0xF0) == 0xE0) {
		charsRead += fbyte;	// add first byte of char to charsRead
		
		in.get(c);
		int byte2 = c;
		if ( (byte2 & 0xC0) != 0x80 ) {
			ostringstream oss;
			oss << "0x" << hex << byte2 << " was not a valid continuation byte in file: " << filename;
			throw oss.str();
		}
		charsRead += c;

		in.get(c);
		int byte3 = c;
		if ( (byte3 & 0xC0) != 0x80 ) {
			ostringstream oss;
			oss << "0x" << hex << byte3 << " was not a valid continuation byte in file: " << filename;
			throw oss.str();
		}
		charsRead += c;
	}

	// 4 bytes
	else if ( (byte1 & 0xF8) == 0xF0 ) {
		charsRead += fbyte;	// add first byte of char to charsRead

		in.get(c);
		int byte2 = c;
		if ( (byte2 & 0xC0) != 0x80 ) {
			ostringstream oss;
			oss << "0x" << hex << byte2 << " was not a valid continuation byte in file: " << filename;
			throw oss.str();
		}
		charsRead += c;

		in.get(c);
		int byte3 = c;
		if ( (byte3 & 0xC0) != 0x80 ) {
			ostringstream oss;
			oss << "0x" << hex << byte3 << " was not a valid continuation byte in file: " << filename;
			throw oss.str();
		}
		charsRead += c;

		in.get(c);
		int byte4 = c;
		if ( (byte4 & 0xC0) != 0x80 ) {
			ostringstream oss;
			oss << "0x" << hex << byte4 << " was not a valid continuation byte in file: " << filename;
			throw oss.str();
		}
		charsRead += c;
	}

	else {
		ostringstream oss;
		oss << "0x" << hex << byte1 << " was not a valid leading UTF8 byte in file: " << filename;
		throw oss.str();
	}
}

// Read characters from an input file
void U::readfile(string filename) {
	ifstream in(filename);

	if (!in) {
		ostringstream oss;
		oss << "Unable to read file: " << filename;
		throw oss.str();
	}

	char c;
	while(in.get(c)) {
		readUTF(c, in, filename);
	}
	in.close();
}

// State how many characters read thus far
int U::size() const {
	//char byte;
	int size = 0;

	for (unsigned int i = 0; i < charsRead.length(); i++) {
		//byte = charsRead.at(i);
		// convUTF(byte, charsRead, i);	// will modify i
		if (bytes(i) == 1) {
			// don't incr i since char is 1 byte and loop will do it
			//size++;
		}
		else if (bytes(i) == 2)
			i += 1;
		else if (bytes(i) == 3)
			i += 2;
		else if (bytes(i) == 4)
			i += 3;

		size++;
	}
	
	return size;		// can't return charsRead.length() b/c that is the length in bytes not actual characters
}

// Get all chars read thus far
string U::get() const {
	return charsRead;
}

// Get char at index
string U::get(int index) const {
	if (index > (this->size() - 1) || index < 0) {
		ostringstream oss;
		oss << "Invalid index: " << index << " ( valid range: [0," << this->size() << ") )";
		throw oss.str();
	}
	return this->get(index, index + 1);
}

// Determines byte length of character
int U::bytes(int index) const {
	if ( (charsRead.at(index) & 0x80) == 0 )
		return 1;
		
	else if ( (charsRead.at(index) & 0xE0) == 0xC0 )
		return 2;
	
	else if ( (charsRead.at(index) & 0xF0) == 0xE0 )
		return 3;

	else if ( (charsRead.at(index) & 0xF8) == 0xF0 )
		return 4;

	else {	// means byte is not valid UTF8 leading byte
		ostringstream oss;
		oss << "Not a valid UTF8 character. Bad leading byte: " << charsRead.at(index);
		throw oss.str();
	}
}

// Get chars from start to end
string U::get(int start, int end) const {
	int count = 0;
	int index = 0;
	string res = "";

	int size = this->size();
	
	if ( end > size || start < 0 || end < start ) {
		ostringstream oss;
		oss << "Indices start and end are an invalid range ( valid range: [0," << size << "] )";
		throw oss.str();
	}

	if (start == end)
		return res;

	while ( index != (end) ) {
		
		if (index == start) {
			while (index < end) {
				if (bytes(count) == 1) {
					res += charsRead.at(count);
					count++;
				}
				else if (bytes(count) == 2) {
					res += charsRead.at(count);
					count++;
					res += charsRead.at(count);
					count++;
				}
				else if (bytes(count) == 3) {
					res += charsRead.at(count);
					count++;
					res += charsRead.at(count);
					count++;
					res += charsRead.at(count);
					count++;
				}
				else if (bytes(count) == 4) {
					res += charsRead.at(count);
					count++;
					res += charsRead.at(count);
					count++;
					res += charsRead.at(count);
					count++;
					res += charsRead.at(count);
					count++;
				}
				index++;
			}
		} else {
			
			if (bytes(count) == 1)
				count++;
			
			else if (bytes(count) == 2)
				count += 2;
			
			else if (bytes(count) == 3)
				count += 3;

			else if (bytes(count) == 4)
				count += 4;

			index++;
		}
	}

	return res;
}
