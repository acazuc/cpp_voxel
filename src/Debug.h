#ifndef DEBUG_H
# define DEBUG_H

# include <iostream>
# include <unistd.h>
# include <typeinfo>

# ifdef ERROR
#  undef ERROR
# endif
# define LOG(s) {std::cout << "[LOG] " << s << std::endl;}
# define WARN(s) {std::cerr << "[WARNING] " << s << " (" << __FILE__ << ':' << __LINE__ << ')' << std::endl;}
# define ERROR(s) {std::cerr << "[ERROR] " << s << " (" << __FILE__ << ':' << __LINE__ << ')' << std::endl;exit(EXIT_FAILURE);}
# define EXCEPTION(e) {std::cerr << "[EXCEPTION] " << typeid(e).name << ": " << e.what() << " (" << __FILE__ << ':' << __LINE__ << ')' << std::endl;}

#endif
