#include "qtl/store.h"
#include "qtl/container.h"
#include "qtl/tree.h"

/*
{"a"}
{"b", 1}
{"cd", 2, 3}
{"ce", "de", 4, 5}

queries would be

col[0] = "a"
col[1] = 1
col[2] > 3
col[0] < col[1]
*/

int main(){
  qtl:: store s;
using namespace std::string_literals;
using namespace lex::literals;
using namespace qtl::literals;

 s[std::vector<lex::scalar>{}]={"a"_s};
 // s[{"a"_s}]={};
 s[{"b"_s}]={1_s};
 s[{"cd"_s}]={ 2_s, 3_s};
 s[std::vector<lex::scalar>{"ce"_s, "de"_s}] = {4_s, 5_s};

 std::vector<qtl::expr>queries={
    qtl::expr(0_col)==qtl::expr("a"_s),
    qtl::expr(1_col)==qtl::expr(1_s),
    qtl::expr(2_col)>qtl::expr(3_s),
    qtl::expr(0_col)<qtl::expr(1_col),
 };
 for( auto q:queries ){
   std::cout << q.stringify() << "\n";
   for( auto x:s[q] ){
      std::cout << x << "\n";
   }
 }
}


