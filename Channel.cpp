#include "Channel.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{
	
}

Channel::Channel(std::string name)
{
	_name = name;
	_iskey = 0;
}

Channel::Channel(std::string name, std::string key)
{
	_name = name;
	_key = key;
	_iskey = 1;
}