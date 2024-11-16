#ifndef Z_STRING_HPP_
#define Z_STRING_HPP_
#include <string>
#include <vector>

/* Return vector of z_indexes for string s */
std::vector<size_t> z_string(std::string &s);

/* 
 * Return vector of indexes from where string sub in the string s
 *
 * sep is uniquie character not existing in s or sub
 */
std::vector<size_t> z_string_subs(std::string &s, std::string &sub, char sep = '3');


#endif // !Z_STRING_HPP_
