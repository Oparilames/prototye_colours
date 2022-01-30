// Copyright (c) 2022 Oparilames under MIT License
// Exact license terms can be found in the LICENSE file
// found in the root-level directory of this distribution.

#include <ostream>
#include <array>
#include <string_view>
#include <iostream>
#if defined(_WIN32)
    #define OS_WINDOWS
#elif defined(__unix__) && (__linux__)
	#define OS_UNIX
#else
	#error "OS not supported."
#endif

// OS specific stuff
#if defined (OS_WINDOWS) 
#include <windows.h>
#endif


constexpr int INVALIDCOLOR = 8;
constexpr int COLOURARRAY_SIZE = INVALIDCOLOR+1;
constexpr int MAX_INDEX = COLOURARRAY_SIZE-1;

// typesafe enum to avoid going beyond array boundaries.
// not enum class because one can't use it as indices for arrays. 
struct index {
enum {BLACK = 0,
			RED,
			GREEN,
			YELLOW,
			BLUE,
			MAGENTA,
			CYAN,
			WHITE,
			INVALID} value;
			using TYPE = decltype(value);
	constexpr index(TYPE v):value(v){};
	TYPE operator()() {return value;}
	TYPE constexpr get() {return value;}
};

// global constants to allow easier access
constexpr index
      idx_black(index::BLACK),
	  idx_red(index::RED),
	  idx_green(index::GREEN),
	  idx_yellow(index::YELLOW),
	  idx_blue(index::BLUE),
	  idx_magenta(index::MAGENTA),
	  idx_cyan(index::CYAN),
	  idx_white(index::WHITE),
	  idx_invalid(index::INVALID);




struct colorisation;

// represents a colour with a code (for display function) and a name (debug purposes only?) to use in colorisation class 
struct colour {
private:
// Array with all colours that are available for both OS, can be instanziated with normal int and size_t values.
// Direct access avoided by including it in struct.
	inline static constexpr std::array<const char*,COLOURARRAY_SIZE> colourNames={{
			{"black"},
			{"red"},
			{"green"},
			{"yellow"},
			{"blue"},
			{"magenta"},// {"purple"},
			{"cyan"}, // {"aqua"},
			{"white"}/*,
			{"grey"},
			{"light blue"},
			{"light green"},
			{"light aqua"},
			{"light red"},
			{"light purple"},
			{"light yellow"},
			{"bright white"}*/,
			{"INVALID"}}
	};
	const int code;
	const std::string_view name;
public:
	colour(const int codeToPut, index toName);
	
	constexpr inline const auto getName() const {return name;}
	
	constexpr inline const int getCode() const {return code;}
	
	static const size_t ifExistingGetIndex(const char* toFind);
	
	friend struct colorisation;
	friend void test(index::TYPE bg);
};

colour::colour(const int codeToPut, index toName):code(codeToPut),name(colourNames[toName()]){ }
const size_t colour::ifExistingGetIndex(const char* toFind) {
		for(int i=0; i<INVALIDCOLOR; ++i)
			if(colourNames[i] == toFind) return i;
		return MAX_INDEX;
	}


struct colorisation {
	inline static colour hue[COLOURARRAY_SIZE]={
	#if defined (OS_WINDOWS)
		{colour(0,  idx_black)},
		{colour(4,  idx_red)},
		{colour(2,  idx_green)},
		{colour(14, idx_yellow)},
		{colour(1,  idx_blue)},
		{colour(5,  idx_magenta)},
		{colour(3, idx_cyan)},
		{colour(15,  idx_white)},
	#else
		{colour(30,  idx_black)},
		{colour(31,  idx_red)},
		{colour(32,  idx_green)},
		{colour(33, idx_yellow)},
		{colour(34,  idx_blue)},
		{colour(35,  idx_magenta)},
		{colour(36, idx_cyan)},
		{colour(37,  idx_white)},
	#endif
		{colour(-1, idx_invalid)}
	};
	std::ostream& stream;
	colorisation(std::ostream& s):stream(s){}
	static std::ostream& reset() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole,colorisation::hue[idx_black.value].getCode()*16+colorisation::hue[idx_white.value].getCode());
		return std::cout;
	}
	
	~colorisation(){reset();}
	
	std::ostream& setTo(index textcolor, index backgroundcolor) {
		auto& colorObjectForText{colorisation::hue[textcolor()]};
		auto& colorObjectForBackground{colorisation::hue[backgroundcolor()]};
		
	#if defined (OS_WINDOWS)
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole,colorObjectForBackground.getCode()*16+colorObjectForText.getCode());
	#endif
	return stream;
	}
};



void windowsCalculator(index textcolour, index backgroundcolour) {
	auto& textColourObj{colorisation::hue[textcolour()]};
	auto& backgroundColourObj{colorisation::hue[backgroundcolour()]};
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	colorisation::reset();
	
	std::cout << "[COLOUR CALCULATION: " << textColourObj.getName() << " on " << backgroundColourObj.getName() <<  "]\n";
		// First word is text colour coloured
		SetConsoleTextAttribute(hConsole,textColourObj.getCode());
		std::cout << "         Text colour: ";
		colorisation::reset();
		std::cout << textColourObj.getName() << ", value: " << textColourObj.getCode() <<  "\n";
		
		
	// First word is background colour coloured
	SetConsoleTextAttribute(hConsole,backgroundColourObj.getCode());
		std::cout << "   Background colour: ";
		colorisation::reset();
		std::cout << backgroundColourObj.getName() << ", value: " << backgroundColourObj.getCode() <<  "\n";
	
	// Both together
	SetConsoleTextAttribute(hConsole,backgroundColourObj.getCode()*16+textColourObj.getCode());
	std::cout << "    Should look like this.";
	
	colorisation::reset();
	std::cout << "\n\n";
}
int main() {
	std::cout << "This program is a prototype of a coloring API for Linux and Windows based on\nhttps://github.com/imfl/color-console\n\n";

	windowsCalculator(idx_magenta.value, idx_green.value);
    windowsCalculator(idx_blue.value, idx_black);
	windowsCalculator(idx_blue.value, idx_red);
	windowsCalculator(idx_blue.value, idx_green);
	windowsCalculator(idx_blue.value, idx_yellow);
	windowsCalculator(idx_blue.value, idx_blue);
	windowsCalculator(idx_blue.value, idx_magenta);
	windowsCalculator(idx_blue.value, idx_cyan);
	windowsCalculator(idx_blue.value, idx_white);
	windowsCalculator(idx_blue.value, idx_invalid);
	
	auto checker=[](const char* input){
		auto tmp{colour::ifExistingGetIndex(input)};
		if(tmp!=MAX_INDEX) std::cout << input << " does exist at index: " << tmp << "\n";
		else std::cout << input << " does NOT exist.\n";
		};
	
	checker("grey");
	checker("red");
	checker("over 9000");
	checker("green");
	checker("cyan");
	
	colorisation peter(std::cout);
	peter.setTo(idx_blue.value, idx_black.value) << "Hello Peter!\n";
	peter.setTo(idx_black.value, idx_white.value) << "Goodbye!\n";
	
	return 0;
};