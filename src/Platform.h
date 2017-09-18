#ifndef PLATFORM_H
# define PLATFORM_H

# ifdef _WIN32
#  define PLATFORM_WINDOWS
# elif defined __linux__
#  define PLATFORM_LINUX
# else
#  error Platform not supported
# endif

# if defined __LP64__ || defined _LP64 || __SIZEOF_POINTER__ == 8
#  define PLATFORM_64
# else
#  define PLATFORM_32
# endif

#endif
