#ifndef HASHBERG_HPP_
#define HASHBERG_HPP_
/*
 * Realization of Hashberg's algorithm 
 * for finding Longest Common Sequence(LCP for futher) of 2 sequence
 * with time complexety O(n1*n2)
 * with memory complexety O(min{n1, n2});
 * */


#include "VectorView.hpp"

using LCPMatrix = std::vector<std::vector<int>>;

/**
 * @brief From Bill Smith algo on strings realization finding LCP length of 2 sequence
 * 
 * @param x1 -- sequence 1
 * @param x2 -- sequence 2
 *
 * @return Last line of LCP matrix corresponds to LCP(x1[0..i], x2[0..n2]), n2 -- |x2|
 */
std::vector<int> H2(const view::vector<int> x1, const view::vector<int> x2);
std::vector<int> H2_prim(std::vector<int> x1, std::vector<int> x2);


/**
 * @brief From Bill Smith algo on strings realization of finding LCP of 2 sequence
 *
 * @param x1 -- sequence 1
 * @param x2 -- sequence 2
 *
 * @return -- LCP of x1 and x2
 */
std::vector<int> H3(const view::vector<int> x1, const view::vector<int> x2);
std::vector<int> H3_prim(std::vector<int> x1, std::vector<int> x2);




#endif // !HASHBERG_HPP_
