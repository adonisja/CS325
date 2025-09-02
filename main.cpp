#include "Helper.h"

#include <string>
#include <iostream>

bool IsBinaryString(const std::string& idx)
{
    for (char c : idx)
    /* // Modern C++ syntax for range-based looping, this is similar to Python's
        item based access for string/list/dicts, basically an iterator with a built-in
        .next(), update your system if this doesn't work for you >.>
    */ 
    {
        if ((c != '0') && (c != '1'))
        {
            return false;
        }
    }
    return true;
}

std::string Invert(const std::string& idx)
{
    if (IsBinaryString(idx)) // Check if the inputted string is binary or not
    {
        std::string res;  // use a new variable to store the result of the previous check
        for (char c : idx) // See above
        {
            if (c == '0') { res += '1'; } // if its a 0 flip it and add to our result string
            else { res += '0'; } // Otherwise we know its a 1 (or we wouldn't be here), flip that then add to the result string
        }
        return res; 
    } else { return ""; } // If its not a binary string then return an empty string
}

int main()
{
    std::cout << "Lets test this program" << std::endl;
    std::string testcases[] = // My test-cases, feel free to add your own 
    {
        "101010",
        "101",
        "0201",
        "dfgjh",
        "1",
        "0"
    };
    for (int i = 0; i < std::size(testcases); i++) // regular index-based looping
    {
        std::string res = Invert(testcases[i]); // variable to store the result of inverting the test string
        std::cout << "Test " << (i+1) << " '" << testcases[i] << "' : " 
        << (res.empty() ? "This is not a binary string" : res) << std::endl;
        /* Using a ternary operator I crafted a custom test that spits out the inverted string of digits
         IF it was a valid binary digit, or a custom error message if the result was an empty string.
        */
    }
}