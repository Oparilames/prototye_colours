/// Based on the operating system differentiation of SFML 2.5
#ifndef TERMINALCOLOURISATOR_OSCONFIG_HPP
#define TERMINALCOLOURISATOR_OSCONFIG_HPP

#if defined(_WIN32)

    // Windows
    #define TERMINALCOLOURISATOR_OS_WINDOWS
#elif defined(__unix__)
	#define TERMINALCOLOURISATOR_OS_UNIX
	#if defined(__linux__)

         // Linux
        #define TERMINALCOLOURISATOR_OS_UNIX_LINUX

    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

        // FreeBSD
        #define TERMINALCOLOURISATOR_OS_UNIX_FREEBSD

    #elif defined(__OpenBSD__)

        // OpenBSD
        #define TERMINALCOLOURISATOR_OS_UNIX_OPENBSD

    #else

        // Unsupported UNIX system
        #error This UNIX operating system is not supported by terminal colourisator.

    #endif

#else

    // Unsupported system
    #error This operating system is not supported by terminal colourisator.

#endif

#endif
