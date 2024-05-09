#ifndef PREFIX_FUNCTION_HPP_
#define PREFIX_FUNCTION_HPP_
#include <string>
#include <vector>

/* Return vector where on i'nth index stored max suffix of string's prefix s[0..i] that equal all string's s prefix */
std::vector<size_t> prefix_function(std::string s);

/* Return result same as prefix_function but realization is a classical KMP algo */
std::vector<size_t> kmp(std::string s);

/* Return vector of indexes corresponds all occurences of string sub in string s*/
std::vector<size_t> kmp_subs(std::string s, std::string sub, char sep = '$');



#endif // !PREFIX_FUNCTION_HPP_
