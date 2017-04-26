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
U::U(const U & rhs) : charsRead(rhs.charsRead), charsReadVect(rhs.charsReadVect) { } 	// copy all rhs's class members (private vars) to

// Filename ctor
U::U(string filename) {
	charsRead = "";			// initialize string to empty string
	readfile(filename);		// call readfile to do the work
}

// Assignment operator=
const U & U::operator=(const U & rhs) {
	if (this != &rhs) {						// rhs by itself is the actual object, but with the & (& rhs), it get the pointer to rhs, which is also what this is (a pointer to the object)
		charsRead = rhs.charsRead;			// assign contents of rhs's charsRead to this object
		charsReadVect = rhs.charsReadVect; 	// assign contents of rhs's charsReadVect to this obj
	}
	return *this; 							// this is just a pointer, *this is the actual object
}

// Dtor
U::~U( ) { }								// default destructor

// Error check ifstream for failure
void U::streamFail(ifstream & in, int byteNum, string filename) {
	if (in.fail()) {
		ostringstream oss;
		oss << "Byte " << byteNum << " was unable to be retrieved (doesn't exist) in file: " << filename;
		throw oss.str();
	}
}

// Error check for invalid continuation byte
void U::contByteFail(int byte, string filename) {
	if ( (byte & 0xC0) != 0x80 ) {
		ostringstream oss;
		oss << "0x" << hex << byte << " was not a valid continuation byte in file: " << filename;
		throw oss.str();
	}
}

// Read and check for valid UTF8 characters
void U::readUTF(int byte1, ifstream & in, string filename) {
	char fbyte = byte1;	// used to concatenate first byte to the charsRead string
	char c; // to get the additional UTF bytes

	// 1 byte
	if ( (byte1 & 0x80) == 0 )
		charsRead += fbyte; // add first byte of char to charsRead

	// 2 bytes
	else if ( (byte1 & 0xE0) == 0xC0 ) {
		charsRead += fbyte;	// add first byte of char to charsRead
		
		in.get(c);
		streamFail(in, 2, filename);	// check if the stream failed to get a character
		int byte2 = c;
		contByteFail(byte2, filename);	// check if this is a valid continuation byte
		charsRead += c;
	}

	// 3 bytes
	else if ( (byte1 & 0xF0) == 0xE0) {
		charsRead += fbyte;	// add first byte of char to charsRead
		
		in.get(c);
		streamFail(in, 2, filename);	
		int byte2 = c;
		contByteFail(byte2, filename);
		charsRead += c;

		in.get(c);
		streamFail(in, 3, filename);
		int byte3 = c;
		contByteFail(byte3, filename);
		charsRead += c;
	}

	// 4 bytes
	else if ( (byte1 & 0xF8) == 0xF0 ) {
		charsRead += fbyte;	// add first byte of char to charsRead

		in.get(c);
		streamFail(in, 2, filename);
		int byte2 = c;
		contByteFail(byte2, filename);
		charsRead += c;

		in.get(c);
		streamFail(in, 3, filename);
		int byte3 = c;
		contByteFail(byte3, filename);
		charsRead += c;

		in.get(c);
		streamFail(in, 4, filename);
		int byte4 = c;
		contByteFail(byte4, filename);
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
		if (in.fail()) {
			ostringstream oss;
			oss << "Byte 1 was unable to be retrieved (doesn't exist) in file: " << filename;
			throw oss.str();
		}
		
		readUTF(c, in, filename);
	}

	in.close();		// close the file stream

	// Create vector from this accumulated string
	for (uint i = 0; i < charsRead.length(); /* leave incrementing work for body of loop */) {
		if (bytes(i) == 1) {
			charsReadVect.push_back(charsRead.substr(i, 1));
			++i;	
		} else if (bytes(i) == 2) {
			charsReadVect.push_back(charsRead.substr(i, 2));
			i += 2;
		} else if (bytes(i) == 3) {
			charsReadVect.push_back(charsRead.substr(i, 3));
			i += 3;
		} else if (bytes(i) == 4) {
			charsReadVect.push_back(charsRead.substr(i, 4));
			i += 4;
		}
	}
}

void U::append(string extra) {
	/*for (int i = 0; i < extra.length(); i++) {
		if (bytes(i) == 1) {
			
		}
	}*/
	charsRead+=extra;
}

// State how many characters read thus far
int U::size() const {
	//char byte;
	/* int size = 0;

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
	} */
	
	return charsReadVect.size();;		// return length of the vector
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
	return charsReadVect.at(index);
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
	string res = "";
	for (int i = start; i < end; ++i) {
		res += charsReadVect.at(i);
	}

	return res;
}

int U::convUTF(int byte1, string charac) const {	// index is passed by reference because it will change the actual value of index
																			// in the loop this function will be called in
	char c; // to get the additional UTF bytes

	if ( (byte1 & 0x80) == 0 ) 
		return (byte1 & 0x7F);

	if ( (byte1 & 0xE0) == 0xC0 ) {
		c = charac.at(1);
		int byte2 = c;

		byte1 = (byte1 << 6) & 0x7C0;
		byte2 = byte2 & 0x3F;

		return (byte1 | byte2);
	}

	if ( (byte1 & 0xF0) == 0xE0) {
		c = charac.at(1);
		int byte2 = c;

		c = charac.at(2);
		int byte3 = c;

		byte1 = (byte1 << 12) & 0xF000; // shift to bits 15-12, mask upper 4 bits
		byte2 = (byte2 << 6) & 0xFC0;   // shift to bits 11-6, mask bits 12-7
		byte3 = byte3 & 0x3F;			// shift to bits 5-0, mask bits 5-0

		return (byte1 | byte2 | byte3); // combine the 3 bytes to get unicode
	}

	if ( (byte1 & 0xF8) == 0xF0 ) {
		c = charac.at(1);
		int byte2 = c;

		c = charac.at(2);
		int byte3 = c;

		c = charac.at(3);
		int byte4 = c;

		byte1 = (byte1 & 0x07) << 18; // get lower 3 bits of 1st byte, shift to bits 20-18
		byte2 = (byte2 & 0x3F) << 12; // get lower 6 bits of 2nd byte, shift to bits 17-12
		byte3 = (byte3 & 0x3F) << 6;  // get lower 6 bits of 3rd byte, shift to bits 11-6
		byte4 = byte4 & 0x3F;		  // get lower 6 bits of 4th byte, shift to bits 5-0
		
		return (byte1 | byte2 | byte3 | byte4); // combine the 4 bytes to get unicode
	}

	return -1; // return -1 if not a valid unicode character
}

// Return the codepoint at charsRead[index]
int U::codepoint(int index) const {
	string charac = get(index);
	return convUTF(charac.at(0), charac);
}

// Return true if charsRead is empty
bool U::empty() const {
	return (charsRead.length() == 0);
}

// Clear charsRead
void U::clear() {
	charsRead = "";
}
