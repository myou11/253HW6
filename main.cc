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
		
		/* -------------- U testing ----------------*/
		
		cout << "\n\n------------- U testing ----------------\n\n";

		// size and empty
		cout << "Should be 8: " << u.size() << '\n'
			<< "Should be false: " << u.empty() << '\n';
		
		// get
		cout << "Should be a³+b³≠c³: " << u.get() << '\n'
			<< "Should be ≠: " << u.get(5) << '\n'
			<< "Should be b³: " << u.get(3,5) << '\n';
		try {
			u.get(-1);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.get(9);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.get(-1,9);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		
		// append TODO: WHAT ARE THE WAYS THIS CAN FAIL?? SEE DESKTOP NOTE
		cout << "Before: " << u.get() << '\n';
		u.append("marvεlΩus");
		cout << "After: " << u.get() << '\n';

		// readfile
		cout << "Before: " << u.get() << '\n';
		u.readfile("story.txt");
		cout << "After: " << u.get() << '\n';
		try {
			u.readfile("nothere.txt");
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.readfile("/bogus");
		} catch (const string &msg) {
			cout << "I expected this: " << msg << '\n';
		}

		// codepoint
		cout << "Should be ≠: " << u.get(5) << '\n';
		cout << "Should be 2260 converted to dec (8800): " << u.codepoint(5) << '\n';
		cout << "Should be 97: " << u.codepoint(0) << '\n';
		cout << "Should be 179: " << u.codepoint(1) << '\n';
		cout << "Should be 99: " << u.codepoint(6) << '\n';
		try {
			u.codepoint(-1);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.codepoint(8);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}

		// clear
		cout << "Clearing u...\n";
		u.clear();
		cout << "Size should be 0: " << u.size() << '\n';
		cout << "Should be empty (true): " << u.empty() << '\n';

		/* -------------- P testing ----------------*/
		
		cout << "\n\n------------- P testing ----------------\n\n";
		
		// readfile (should be error because already read a propfile)
		try {
			p.readfile("UnicodeDataV2.txt");
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}

		// count(string)
		cout << "Should be 2: " << p.count("Sm") << '\n'
			<< "Should be 3: " << p.count("Ll") << '\n';

		// count(int)
		cout << "# Ll's before: " << p.count("Ll") << '\n';
		p.count(122);
		cout << "# Ll's after: " << p.count("Ll") << '\n';
		
		// props
		set<string> props = p.props();
		for (auto &s : props)
			cout << s << ' ';
		cout << '\n';

		// size
		cout << "Should be 29: " << p.size() << '\n';

		// empty
		cout << "Should be false: " << p.empty() << '\n';

		// clear
		cout << "Clearing p...\n";
		p.clear();
		cout << "Size should be 0: " << p.size() << '\n';
		cout << "Should be empty (true): " << p.empty() << '\n';

		// testing hamlet
		cout << "Testing hamlet\n";
		U ham = U(pub + "hamlet.txt");
		cout << "Should be 182568: " << ham.size() << '\n';

		return 0;
	}
	catch (const string &msg) {
		cout << "Unexpected error: " << msg << '\n';
	}
}
