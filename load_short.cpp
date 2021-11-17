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
s[{"A"_s}]={1685153_s};
s[{"A-A"_s}]={9_s};
s[{"A-A-D-C"_s}]={2_s};
s[{"A-A-X"_s}]={1_s};
s[{"A-ATC"_s}]={13_s};
s[{"A-Add-Eth"_s}]={2_s};
s[{"A-Albionic"_s}]={1_s};
s[{"A-Americans"_s}]={1_s};
s[{"A-B"_s}]={224_s};
s[{"A-B-C"_s}]={6_s};
s[{"A-B-C-D"_s}]={3_s};
s[{"A-B-C-D-E-F"_s}]={2_s};
s[{"A-B-C-G"_s}]={1_s};
s[{"A-B-C-N-U-LATER"_s}]={1_s};
s[{"A-B-C-N-YA"_s}]={6_s};
s[{"A-BASIN"_s}]={3_s};
s[{"A-BBS"_s}]={1_s};
s[{"A-Basin"_s}]={8_s};
s[{"A-Biological-Weapons-Factory"_s}]={1_s};
s[{"A-Block"_s}]={1_s};
s[{"A-Bmin-A-Bmin-A"_s}]={1_s};
s[{"A-Bomb"_s}]={3_s};
s[{"A-Bones"_s}]={1_s};
s[{"A-By"_s}]={1_s};
s[{"A-C"_s}]={8_s};
s[{"A-C-D"_s}]={1_s};
s[{"A-C-D-C-F-A-G"_s}]={1_s};
s[{"A-C-E"_s}]={3_s};
s[{"A-C-E-G"_s}]={1_s};
s[{"A-C-I-D"_s}]={1_s};
s[{"A-C-T-S"_s}]={1_s};
s[{"A-Chains"_s}]={1_s};
s[{"A-Changin"_s}]={1_s};
s[{"A-Child"_s}]={1_s};
s[{"A-Classic-Character-Only-To-Trash-Him"_s}]={1_s};
s[{"A-Code"_s}]={1_s};
s[{"A-Cons"_s}]={1_s};
s[{"A-Courtin"_s}]={1_s};
s[{"A-Cumba-Cumba-Cumba-Chera"_s}]={2_s};
s[{"A-D"_s}]={12_s};
s[{"A-D-A-A-D-D-A-E-D-A-E"_s}]={1_s};
s[{"A-D-R-E-N-A-L-I-N-E"_s}]={4_s};
s[{"A-DOS"_s}]={1_s};
s[{"A-Day"_s}]={1_s};
s[{"A-E"_s}]={7_s};
s[{"A-ESC"_s}]={1_s};
s[{"A-F"_s}]={12_s};
s[{"A-FAMILY"_s}]={1_s};
s[{"A-Fields"_s}]={1_s};
s[{"A-Fleet"_s}]={1_s};
s[{"A-Frame"_s}]={1_s};
s[{"A-GkcLhT"_s}]={1_s};
s[{"A-Go-Go"_s}]={4_s};
s[{"A-Growin"_s}]={1_s};
s[{"A-H"_s}]={3_s};
s[{"A-HA"_s}]={15_s};
s[{"A-HAH"_s}]={1_s};
s[{"A-HEM"_s}]={3_s};
s[{"A-Ha"_s}]={6_s};
s[{"A-Haaa"_s}]={1_s};
s[{"A-Haaah"_s}]={1_s};
s[{"A-Harder-Code-Than-You"_s}]={1_s};
s[{"A-Head"_s}]={2_s};
s[{"A-Hem"_s}]={1_s};
s[{"A-Hhem"_s}]={1_s};
s[{"A-High"_s}]={1_s};
s[{"A-I"_s}]={1_s};
s[{"A-I-R"_s}]={1_s};
s[{"A-INFOS"_s}]={1_s};
s[{"A-If"_s}]={1_s};
s[{"A-J"_s}]={14_s};
s[{"A-JGA"_s}]={1_s};
s[{"A-Jax"_s}]={3_s};
s[{"A-Jeo-Ssi"_s}]={2_s};
s[{"A-KO"_s}]={61_s};
}
