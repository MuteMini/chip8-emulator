/*
    Author: Min Kang
    Creation Date: January 30th, 2024

    Declares and defines a logger that will be used only if a compile definition is set.
    File system design from https://stackoverflow.com/questions/17595957/operator-overloading-in-c-for-logging-purposes

    endl overload from https://stackoverflow.com/questions/1134388/stdendl-is-of-unknown-type-when-overloading-operator
*/

#ifndef LOGGER_H
#define LOGGER_H

#ifndef DEBUG_OFF

#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>

class Logger
{
    private:
        std::ostream& stream;

    public:
        Logger(std::ostream& stream = std::cout) : stream(stream) {};

        template<typename T> 
        Logger& operator<< (T object) 
        {
            (*this).stream << object;
            return *this;
        };

        Logger& operator<< (std::ostream& (*os)(std::ostream&))
        {
            (*this).stream << os;
            return *this;
        };
};

#else

class Logger
{
    public:
        Logger() {};

        template<typename T> 
        Logger& operator<< (T object)                           { return *this; };
        Logger& operator<< (std::ostream& (*os)(std::ostream&)) { return *this; };
};

#endif

#endif