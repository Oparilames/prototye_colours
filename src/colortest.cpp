// Copyright (c) 2022 Oparilames under MIT License
// Exact license terms can be found in the LICENSE file
// found in the root-level directory of this distribution.

// Resources if I want to implement this better
/*
 * https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
 * https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
 */

// To read for code improvements: https://discord.com/channels/216010419213434882/427569677427015680/942055355138056232
#include <fstream>
#include <utility> // pair
std::ofstream changeLog;


#include <iostream>

#include "../config.hpp"

//LANG=en g++ -Wfatal-errors -Wall -Wextra -std=c++20 -O2 -I/home/oparilames/Programming/Code/2021/2021-11-04_UnicodeCharacterSequenceAnalyser/src -D __DEBUG_ON main.cpp -o ./bin/main 

#if defined (LATEXRIPPER_OS_WINDOWS)
#include <windows.h>
#endif
#include <concepts>
template <typename T>
constexpr int toUnderlyingType(T a){return static_cast<std::underlying_type_t<T>>(a);};

template<typename T, typename ... U>
concept anyOfType = (std::same_as<T, U> || ...);

template<typename T, typename ... U>
concept anyOfDecayedType = (std::same_as<typename std::decay<T>::type, typename std::decay<U>::type> || ...);

// Output operator into osstream available?
// from https://quuxplusone.github.io/blog/2018/09/08/problems-concepts-should-solve/
// and https://stackoverflow.com/a/49026811
template<typename S, typename T, typename = void>
struct is_to_stream_writable: std::false_type {};

template<typename S, typename T>
struct is_to_stream_writable<S, T,
        std::void_t<  decltype( std::declval<S&>()<<std::declval<T>() )  > >
  : std::true_type {};

template<class T>
inline constexpr bool canPrintedByConsole_v = is_to_stream_writable<std::ostream,T>::value;


template<bool ... T>
inline constexpr bool anyTruthIn ( ){return (... || T);};

namespace dye {
    namespace detail {
		enum class colourID
		{
			reset,
			black,
			red,
			green,
			yellow,
			blue,
			magenta,
			cyan,
			white,
			
			INVALID
		};
		const int firstColourIndex=static_cast<int>(dye::detail::colourID::black);
		
        static constexpr std::string_view colourNames[]={
			"Reset",
            "Black",
            "Red",
            "Green",
            "Yellow",
            "Blue",
            "Magenta",
            "Cyan",
            "White",
            
            "INVALID"
        };

#if defined (LATEXRIPPER_OS_WINDOWS)
            static constexpr int colourCodes[]={
				07,// reset in windows??
                0,
                4,
                2,
                14,
                1,
                5,
                11,
                7,

                -1
            };
    #else
            static constexpr int colourCodes[]={
				0,
                30,
                31,
                32,
                33,
                34,
                35,
                36,
                37,

                -1
            };
    #endif
    constexpr int getForegroundCode(dye::detail::colourID c) {return dye::detail::colourCodes[toUnderlyingType(c)];};
    constexpr int getBackgroundCode(dye::detail::colourID c) {
		#if defined (LATEXRIPPER_OS_WINDOWS)
		return dye::detail::colourCodes[toUnderlyingType(c)]*16;
		#else
		return dye::detail::colourCodes[toUnderlyingType(c)]+10;
		#endif
	}
	class colouriser {
	public:
		inline static std::pair<dye::detail::colourID, int> foreground{dye::detail::colourID::reset, getForegroundCode(dye::detail::colourID::reset)};
		inline static std::pair<dye::detail::colourID, int> background{dye::detail::colourID::reset, getBackgroundCode(dye::detail::colourID::reset)};
	
		void setForeground(dye::detail::colourID c) {
			foreground=std::pair(c, getForegroundCode(c));//(c,getCode(c));
			changeLog << "Foreground: " << dye::detail::colourNames[toUnderlyingType(foreground.first)] << "\n";
		}
		void setBackground(dye::detail::colourID c) {
			/*
			#if defined (LATEXRIPPER_OS_WINDOWS)
			background=(c,getCode(c)*16);
			#else
			background=(c,getCode(c)+10);
			#endif 
			*/
			background=std::pair(c, getBackgroundCode(c));
			changeLog << "Background: " << dye::detail::colourNames[toUnderlyingType(background.first)] << "\n";
		}
		friend std::ostream& operator<<(std::ostream& os, dye::detail::colourID targetColour);
		void reset() {setBackground(dye::detail::colourID::black); setForeground(dye::detail::colourID::reset); //std::cout << background.first;
			};
	};
	

	std::ostream& operator<<(std::ostream& os, dye::detail::colourID targetColour) {
		#if defined (LATEXRIPPER_OS_WINDOWS)
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole,getForegroundCode(targetColour)+getBackgroundCode(colouriser::background.first));
		os << 
		#else
		os <<"\033[" << getForegroundCode(targetColour) << ";" << /*std::to_string(getBackgroundLinux(bg))*/ getBackgroundCode(colouriser::background.first) << "m";
		#endif
		return os;
	}

    static_assert(sizeof(dye::detail::colourCodes)/sizeof(int) == toUnderlyingType(dye::detail::colourID::INVALID)+1,
                "incorrect number of colour codes provided");

    static_assert(sizeof(dye::detail::colourNames)/sizeof(std::string_view) == toUnderlyingType(dye::detail::colourID::INVALID)+1,
                "incorrect number of colour names provided");
    
template<int T>
inline constexpr bool isConvertibleToColourType_v =
    anyTruthIn<
        (T==static_cast<int>(dye::detail::colourID::reset)),
        (T==static_cast<int>(dye::detail::colourID::black)),
        (T==static_cast<int>(dye::detail::colourID::red)),
        (T==static_cast<int>(dye::detail::colourID::green)),
        (T==static_cast<int>(dye::detail::colourID::yellow)),
        (T==static_cast<int>(dye::detail::colourID::blue)),
        (T==static_cast<int>(dye::detail::colourID::magenta)),
        (T==static_cast<int>(dye::detail::colourID::cyan)),
        (T==static_cast<int>(dye::detail::colourID::white))
        >();
	} // namespace detail

	template<int T>
	requires(dye::detail::isConvertibleToColourType_v<T>)
	struct colour {
		constexpr inline static const dye::detail::colourID value{static_cast<dye::detail::colourID>(T)};
		//constexpr colour(dye::detail::colourID toSet):value(toSet){};
	};
	
    namespace experimental {
			constexpr colour<static_cast<int>(dye::detail::colourID::reset)> reset;
			constexpr colour<static_cast<int>(dye::detail::colourID::black)> black;
			constexpr colour<static_cast<int>(dye::detail::colourID::red)> red;
			constexpr colour<static_cast<int>(dye::detail::colourID::green)> green;
			constexpr colour<static_cast<int>(dye::detail::colourID::yellow)> yellow;
			constexpr colour<static_cast<int>(dye::detail::colourID::blue)> blue;
			constexpr colour<static_cast<int>(dye::detail::colourID::magenta)> magenta;
			constexpr colour<static_cast<int>(dye::detail::colourID::cyan)> cyan;
			constexpr colour<static_cast<int>(dye::detail::colourID::white)> white;
			
			template<typename T>
			concept colourType = (anyOfDecayedType<T,
				decltype(reset),
				decltype(black),
				decltype(red),
				decltype(green),
				decltype(yellow),
				decltype(blue),
				decltype(magenta),
				decltype(cyan),
				decltype(white)
			>);
			
		const int colourAsInt(colourType auto& c){return static_cast<const int>(std::decay<decltype(c)>::type::value);}
		
		
		dye::detail::colourID intAsColourID(int value){
			switch(value)
			{
				case static_cast<int>(dye::detail::colourID::reset):	return colour<static_cast<int>(dye::detail::colourID::reset)>::value;
				case static_cast<int>(dye::detail::colourID::black):	return colour<static_cast<int>(dye::detail::colourID::black)>::value;
				case static_cast<int>(dye::detail::colourID::red):		return colour<static_cast<int>(dye::detail::colourID::red)>::value;
				case static_cast<int>(dye::detail::colourID::green):	return colour<static_cast<int>(dye::detail::colourID::green)>::value;
				case static_cast<int>(dye::detail::colourID::yellow):	return colour<static_cast<int>(dye::detail::colourID::yellow)>::value;
				case static_cast<int>(dye::detail::colourID::blue):		return colour<static_cast<int>(dye::detail::colourID::blue)>::value;
				case static_cast<int>(dye::detail::colourID::magenta):	return colour<static_cast<int>(dye::detail::colourID::magenta)>::value;
				case static_cast<int>(dye::detail::colourID::cyan):		return colour<static_cast<int>(dye::detail::colourID::cyan)>::value;
				case static_cast<int>(dye::detail::colourID::white):	return colour<static_cast<int>(dye::detail::colourID::white)>::value;
			};
			std::cout << "THIS IS AN ILLEGAL NUMBER" << std::endl;
			return colour<0>::value; // bad decission
		};
			
			template<typename T, typename T2=decltype(std::cout)>
			struct paint {
				std::pair<dye::detail::colourID,dye::detail::colourID> backup{dye::detail::colouriser::foreground.first, dye::detail::colouriser::background.first};
				int foregroundIndex{static_cast<int>(backup.first)}, backgroundIndex{static_cast<int>(backup.second)}; // gute Idee?
				auto foreground() {return intAsColourID(foregroundIndex); }
				auto background() {return intAsColourID(backgroundIndex); }
				T content;
				T2& os;

				paint(const dye::experimental::colourType auto& textcol, T&& toColourise, T2& osstream=std::cout)
				requires (canPrintedByConsole_v<T>)
				: foregroundIndex(colourAsInt(textcol)), content(toColourise), os(osstream)
				{
				}
				auto withBackground(const dye::experimental::colourType auto& bgcol) {
					backgroundIndex=colourAsInt(bgcol);
					return *this;
				}
				
				inline void print() {
					dye::detail::colouriser x;
					x.setBackground(background());
					std::cout << foreground() << content;
					x.setBackground(backup.second);
					std::cout << backup.first;
				}
				friend std::ostream& operator<<(std::ostream& os, paint<T>&& targetColour) {
					targetColour.print();
					return os;
				}
			};
			/**
			template<typename T, typename T2=decltype(std::cout)>
			inline static auto&& onlyBackground(const dye::experimental::colourType auto& bgcol, T&& toColourise, T2& osstream=std::cout)
			requires (canPrintedByConsole_v<T>)
			{
				return paint(dye::detail::colouriser::foreground.first, toColourise).withBackground(bgcol);
				//backgroundIndex(colourAsInt(bgcol))
			};
			**/
		template<typename T, typename T2>
		std::ostream& use(T& toColourise, const dye::experimental::colourType auto& foreground, const dye::experimental::colourType auto& background=dye::detail::colouriser::background.first, T2& os=std::cout) {
			std::pair<dye::detail::colourID,dye::detail::colourID> backup{dye::detail::colouriser::foreground.first, dye::detail::colouriser::background.first};
			dye::detail::colouriser x;
			if(background!=backup.second) x.setBackground(background);
			os << foreground << toColourise << backup.first, backup.second;
			return os;
		}
		template<typename T, typename T2>
		std::ostream& useBg(T& toColourise, const dye::experimental::colourType auto& background=dye::detail::colouriser::background.first, T2& os=std::cout) {
			return os << use(dye::detail::colouriser::foreground.first,background);
		}
			
			template<typename T>
			concept colourType_old = (anyOfType<typename std::decay<T>::type, 
			typename std::decay<decltype(reset)>::type, typename std::decay<decltype(black)>::type, typename std::decay<decltype(red)>::type, typename std::decay<decltype(green)>::type, typename std::decay<decltype(yellow)>::type, typename std::decay<decltype(blue)>::type, typename std::decay<decltype(magenta)>::type, typename std::decay<decltype(cyan)>::type, typename std::decay<decltype(white)>::type>);
	};
}
	
	
	auto setBackgroundTo(const dye::experimental::colourType auto& col, decltype(std::cout)& os=std::cout) {
		using colType = std::decay<decltype(col)>::type;
		dye::detail::colouriser x;
		x.setBackground(colType::value);
		os << dye::detail::colouriser::foreground.first;
		/// os << col.value;
		return "";
	}
	auto resetBackground() {
		dye::detail::colouriser x;
		x.setBackground(dye::detail::colouriser::background.first);
		std::cout << dye::detail::colouriser::foreground.first;
		return "";
		}
	
	
	template<typename T, typename T2>
	std::ostream& yellow(T& toColourise, T2& os=std::cout)
	///requires(std::is_convertible_v<T2,std::basic_ostream>)
	{
		//auto backupText = detail::colouriser::foreground;
		//auto backupBackground = detail::colouriser::background;
		auto tempColour=std::pair(dye::detail::colourID::yellow, dye::detail::getForegroundCode(dye::detail::colourID::yellow));
		os << tempColour.first << toColourise;
		os << dye::detail::colouriser::foreground;
		return os;
	}
	template<typename T, typename T2>
	std::ostream& yellow(T&& toColourise, T2& os=std::cout)
	///requires(std::is_convertible_v<T2,std::ostream>)
	{
		//auto backupText = detail::colouriser::foreground;
		//auto backupBackground = detail::colouriser::background;
		auto tempColour=std::pair(dye::detail::colourID::yellow, dye::detail::getForegroundCode(dye::detail::colourID::yellow));
		os << tempColour.first << toColourise;
		os << dye::detail::colouriser::foreground.first;
		return os;
	}
	
	template<typename T>
	auto& testfarbe(T& toColourise, decltype(std::cout)& os=std::cout) {
		auto tempColour=std::pair(dye::detail::colourID::yellow, dye::detail::getForegroundCode(dye::detail::colourID::yellow));
		os << tempColour.first << toColourise;
		os << dye::detail::colouriser::foreground.first;
		return "";
	}
	template<typename T>
	auto&& testfarbe(T&& toColourise, decltype(std::cout)& os=std::cout) {
		auto tempColour=std::pair(dye::detail::colourID::yellow, dye::detail::getForegroundCode(dye::detail::colourID::yellow));
		os << tempColour.first << toColourise;
		os << dye::detail::colouriser::foreground.first;
		return "";
	}
	
	template<typename T>
	auto& testfarbe2(T& toColourise, decltype(std::cout)& os=std::cout) {
		auto tempColour=std::pair(dye::detail::colourID::blue, dye::detail::getForegroundCode(dye::detail::colourID::blue));
		os << tempColour.first << toColourise;
		os << dye::detail::colouriser::foreground.first;
		return "";
	}
	template<typename T>
	auto&& testfarbe2(T&& toColourise, decltype(std::cout)& os=std::cout) {
		auto tempColour=std::pair(dye::detail::colourID::blue, dye::detail::getForegroundCode(dye::detail::colourID::blue));
		os << tempColour.first << toColourise;
		os << dye::detail::colouriser::foreground.first;
		return "";
	}
	
	
	template<typename T>
	auto& test_AonB(T& toColourise, dye::detail::colourID A, dye::detail::colourID B,decltype(std::cout)& os=std::cout) {
		auto backup=std::pair(dye::detail::colouriser::foreground.first, dye::detail::colouriser::background.first);
		dye::detail::colouriser x; 
		x.setBackground(B);
		os << A << toColourise<<"\n";
		x.setBackground(backup.second);
		os << backup.first;
		return "";
	}
	
void test_oldMethod() {
	std::cout << "    Wir schreiben in " <<  testfarbe("gelb") << " und in " << testfarbe2("blau") << "\n";
	std::cout << "    Und jetzt " << test_AonB("schwarz auf weiß",dye::detail::colourID::black, dye::detail::colourID::white);
	std::cout << "    Hier steht wieder normal gefäbter Text.\n";
	std::cout << "    Der Hintergrund " << test_AonB(" ist jetzt blau.",dye::detail::colouriser::foreground.first,dye::detail::colourID::blue) << "    Und wieder wie zuvor." << std::endl;
	std::cout  <<  std::endl;
}

void test_newMethod() {
	std::cout << "    Wir schreiben in " << dye::experimental::paint(dye::experimental::yellow,std::string("gelb")) << " und in " << dye::experimental::paint(dye::experimental::blue,std::string("blau")) <<  "\n";
	std::cout << "    Und jetzt "        << dye::experimental::paint(dye::experimental::black, std::string("schwarz auf weiß")).withBackground(dye::experimental::white) << "\n    Hier steht wieder normal gefäbter Text.\n";
	/// std::cout << "    Der Hintergrund "  << dye::experimental::onlyBackground(dye::experimental::blue,std::string(" ist jetzt blau.")) << "Und wieder wie zuvor." << std::endl;
	std::cout  <<  std::endl;
}

int main() {
	std::cout << "Alte Methode: \n";
	test_oldMethod();
	std::cout << "-------------\nNeue Methode: \n";
	test_newMethod();
	
	//std::cout << dye::experimental::use("Rot auf blau.",dye::experimental::red, dye::experimental::blue) << " Und normal. " << use("Gelbe Schrift.",dye::experimental::yellow) << use("Gelbe Schrift.",dye::experimental::yellow)
	//std::cout << "Der Hintergrund " << setBackgroundTo(dye::experimental::yellow) << " ist jetzt blau." << resetBackground() << "Trallalalallala." << std::endl;
}
