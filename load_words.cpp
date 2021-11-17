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
//s[{"b"_s}]={1_s};
s[{std::string("A")}]={1685153};
s[{std::string("A-A")}]={9};
s[{std::string("A-A-D-C")}]={2};
s[{std::string("A-A-X")}]={1};
s[{std::string("A-ATC")}]={13};
s[{std::string("A-Add-Eth")}]={2};
s[{std::string("A-Albionic")}]={1};
s[{std::string("A-Americans")}]={1};
s[{std::string("A-B")}]={224};
s[{std::string("A-B-C")}]={6};
s[{std::string("A-B-C-D")}]={3};
s[{std::string("A-B-C-D-E-F")}]={2};
s[{std::string("A-B-C-G")}]={1};
s[{std::string("A-B-C-N-U-LATER")}]={1};
s[{std::string("A-B-C-N-YA")}]={6};
s[{std::string("A-BASIN")}]={3};
s[{std::string("A-BBS")}]={1};
s[{std::string("A-Basin")}]={8};
s[{std::string("A-Biological-Weapons-Factory")}]={1};
s[{std::string("A-Block")}]={1};
s[{std::string("A-Bmin-A-Bmin-A")}]={1};
s[{std::string("A-Bomb")}]={3};
s[{std::string("A-Bones")}]={1};
s[{std::string("A-By")}]={1};
s[{std::string("A-C")}]={8};
s[{std::string("A-C-D")}]={1};
s[{std::string("A-C-D-C-F-A-G")}]={1};
s[{std::string("A-C-E")}]={3};
s[{std::string("A-C-E-G")}]={1};
s[{std::string("A-C-I-D")}]={1};
s[{std::string("A-C-T-S")}]={1};
s[{std::string("A-Chains")}]={1};
s[{std::string("A-Changin")}]={1};
s[{std::string("A-Child")}]={1};
s[{std::string("A-Classic-Character-Only-To-Trash-Him")}]={1};
s[{std::string("A-Code")}]={1};
s[{std::string("A-Cons")}]={1};
s[{std::string("A-Courtin")}]={1};
s[{std::string("A-Cumba-Cumba-Cumba-Chera")}]={2};
s[{std::string("A-D")}]={12};
s[{std::string("A-D-A-A-D-D-A-E-D-A-E")}]={1};
s[{std::string("A-D-R-E-N-A-L-I-N-E")}]={4};
s[{std::string("A-DOS")}]={1};
s[{std::string("A-Day")}]={1};
s[{std::string("A-E")}]={7};
s[{std::string("A-ESC")}]={1};
s[{std::string("A-F")}]={12};
s[{std::string("A-FAMILY")}]={1};
s[{std::string("A-Fields")}]={1};
s[{std::string("A-Fleet")}]={1};
s[{std::string("A-Frame")}]={1};
s[{std::string("A-GkcLhT")}]={1};
s[{std::string("A-Go-Go")}]={4};
s[{std::string("A-Growin")}]={1};
s[{std::string("A-H")}]={3};
s[{std::string("A-HA")}]={15};
s[{std::string("A-HAH")}]={1};
s[{std::string("A-HEM")}]={3};
s[{std::string("A-Ha")}]={6};
s[{std::string("A-Haaa")}]={1};
s[{std::string("A-Haaah")}]={1};
s[{std::string("A-Harder-Code-Than-You")}]={1};
s[{std::string("A-Head")}]={2};
s[{std::string("A-Hem")}]={1};
s[{std::string("A-Hhem")}]={1};
s[{std::string("A-High")}]={1};
s[{std::string("A-I")}]={1};
s[{std::string("A-I-R")}]={1};
s[{std::string("A-INFOS")}]={1};
s[{std::string("A-If")}]={1};
s[{std::string("A-J")}]={14};
s[{std::string("A-JGA")}]={1};
s[{std::string("A-Jax")}]={3};
s[{std::string("A-Jeo-Ssi")}]={2};
s[{std::string("A-KO")}]={61};
}
