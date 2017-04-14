#include "U.h"
#include "P.h"
#include <iostream>
using namespace std;
const string pub="/s/bach/a/class/cs253/pub/";   // ~ only works in shells
int main() {
	try {
		U u;
		u.append("a³+b³≠c³");
		P p(pub+"UnicodeData.txt");
		for (int i=0; i<u.size(); i++)
		p.count(u.codepoint(i));
		cout << "Should be 8: " << u.size() << '\n'
		 << "Should be 2: " << p.count("Sm") << '\n'
		 << "Should be b³: " << u.get(3,5) << '\n';
		try {
		u.readfile("/bogus");
		}
		catch (const string &msg) {
		cout << "I expected this: " << msg << '\n';
		}
		return 0;
	}
	catch (const string &msg) {
		cout << "Unexpected error: " << msg << '\n';
	}
}
