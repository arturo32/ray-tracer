#include "error.hpp"

#include <cstdlib>  // std::exit
#include <sstream>

inline std::ostream& operator<<(std::ostream& os,
																const rt3::SourceContext& sc) {
	os << sc.file << ":" << sc.line;
	return os;
}

namespace rt3 {

/// Prints out the warning, but the program keeps running.
void Warning(const std::string& msg, const SourceContext& sc) {
	std::cerr << std::setw(80) << std::setfill('=') << " " << std::endl
						<< "[RT3 Communication System] Warning: \"" << msg << "\"\n"
						<< "     REPORTED AT: < " << sc << " > \n"
						<< std::setw(80) << std::setfill('=') << " " << std::endl;
}

/// Prints out the error message and exits the program.
void Error(const std::string& msg, const SourceContext& sc) {
	std::cerr << std::setw(80) << std::setfill('=') << " " << std::endl
						<< "[RT3 Communication System] Severe error: \"" << msg << "\""
						<< std::endl
						<< "     REPORTED AT: < " << sc << " > " << std::endl
						<< "     exiting...\n"
						<< std::setw(80) << std::setfill('=') << " " << std::endl;

	std::exit(EXIT_FAILURE);
}

void Message(const std::string& str) { std::cout << str << std::endl; }
}  // namespace rt3
