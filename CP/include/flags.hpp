#ifndef FLAGS_HPP_
#define FLAGS_HPP_

namespace flags {
  inline bool ignore_case = false; // -i
  inline bool ignore_tab_expansion = false; // -E
  inline bool ignore_trailing_space = false; // -Z
  inline bool ignore_space_change = false; // -b
  inline bool ignore_all_space = false; // -w
  inline bool ignore_black_lines = false; // -B
  inline bool expand_tabs_to_spaces_in_output = false; // -t
  
} // !flags;

#endif // !FLAGS_HPP_
