#ifndef NBT_EXCEPTION_H
# define NBT_EXCEPTION_H

# include <exception>
# include <string>

namespace voxel
{

	class NBTException : public std::exception
	{

	private:
		std::string err;

	public:
		NBTException(std::string str): err(str) {};
		~NBTException() throw() {};
		const char *what() const throw() {return (this->err.c_str());};

	};

}

#endif
