#!/usr/bin/env python3
"""
Apply C++17 refactoring patterns to upstream/coro branch store.h
"""

import re

def apply_cpp17_patterns():
    with open('qtl/store.h', 'r') as f:
        content = f.read()
    
    print("Applying C++17 patterns to coro branch store.h...")
    
    # Pattern 1: sample class - vector inheritance to composition
    print("  Fixing sample class...")
    old = r'''#define BASE_T std::vector<T>
class sample: public BASE_T\{ // sample O\( log\^2\(n\) \) out of n 
   using base_t=BASE_T;
 #undef BASE_T
   int n=0;
  int lg=0; // log2\(count\)
   int nl=1;
 public:
   int count=0;

   using base_t::base_t;
   void push_back\( const T& v\)\{
     if\( \+\+count >= n \){ // ∫ lg\(x\)/x dx ∝ lg\^2\(x\)
       base_t::push_back\(v\); // v\.size\(\) = O\(lg\^2\(count\)\)'''
    
    new = r'''#define BASE_T std::vector<T>
class sample /*: public BASE_T*/ {
  // C++17: Changed from vector inheritance to composition
  BASE_T base_vec_;
  using base_t=BASE_T;
#undef BASE_T
  int n=0;
  int lg=0; // log2(count)
  int nl=1;
public:
  int count=0;
  
  sample() = default;
  sample(const sample&) = default;
  
  // Vector compatibility methods
  auto begin() const { return base_vec_.begin(); }
  auto end() const { return base_vec_.end(); }
  auto size() const { return base_vec_.size(); }
  auto empty() const { return base_vec_.empty(); }
  
  void push_back( const T& v){
    if( ++count >= n ){ // ∫ lg(x)/x dx ∝ lg^2(x)
      base_vec_.push_back(v); // v.size() = O(lg^2(count))'''
    
    content = re.sub(old, new, content)
    
    # Fix sample::clear()
    content = re.sub(
        r'void clear\(\)\{\s*base_t::clear\(\);',
        'void clear(){ base_vec_.clear();',
        content
    )
    
    # Fix sample::write()
    content = re.sub(
        r'os << " size\(\)="<< base_t::size\(\);',
        'os << " size()="<< base_vec_.size();',
        content
    )
    content = re.sub(
        r'os << " base_t="<< \*reinterpret_cast<const base_t\*>\(this\);',
        'os << " base_vec_="<< base_vec_;',
        content
    )
    
    with open('qtl/store.h', 'w') as f:
        f.write(content)
    
    print("Done applying C++17 patterns to sample class")

if __name__ == '__main__':
    apply_cpp17_patterns()
