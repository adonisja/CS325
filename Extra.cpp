#include <string>
#include <iostream>
#include <sstream>

class Message
{
    private:
        int id;
        std::string msg;

    public:
    /* I keep forgetting how to define these */
        Message() : id(0), msg("") {};  // Default constructor

        Message(int num, std::string txt) : id(num), msg(txt) {}; // Overloaded Constructor

        Message(const Message& other) : id(other.id), msg(other.msg) {}; // Copy Constructor

        Message& operator=(const Message& other)    // Overloaded Assignment Operator
        {
            if (this != &other)
            {
                id = other.id;
                msg = other.msg;
            }
            return *this;
        }

        ~Message() {} // Empty Destructor

        int code() const { return id; }

        std::string message() const { return msg; }

        void code(int num) { id = num; };

        void message(std::string txt) { msg = txt; }

        std::string toString() const
        {
            std::stringstream out;
            out << "\"" << id << " : " << msg << "\"";
            return out.str();
        }
};
