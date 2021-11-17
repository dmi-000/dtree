#include <regex>
#include <map>
#define TRACE(x)

//#include "qtl/container.h"
//#include "qtl/tree.h"



//#include <ranges>
//#define TEST_IOTA
#ifndef TEST_IOTA
#include "qtl/sql.h"
#include "qtl/store.h"
#include "qtl/expr.h"
  qtl::store file;
#else
#include <iostream>
#include <fstream>
#include <boost/spirit/home/x3.hpp>
#define XSTR(s) STR(s)
#define STR(s) #s

namespace x3 = boost::spirit::x3;
namespace qtl{
 class expr:public std::string{
 public:
using base_t=std::string;
using base_t::base_t;
   int o=0;
   std::string stringify(){ return *this; }
    expr operator=(const std::string &s){
      *(base_t *)this=s;
      return *this;
     }
 };
static const auto to_string=[](auto& ctx){ _val(ctx)=std::string(_attr(ctx).begin(),_attr(ctx).end()); };
  //using expr_rule_t = x3::rule<struct expr_rule, qtl::expr>;

 static auto const expr_rule=(
	      x3::rule<struct as_string, std::string>{}= ( x3::raw[
		    +x3::char_(" -~") 
		 ] [ qtl::to_string ] )
	      );


};
  class dummy_store{
    std::vector<int> data;
  public:
    dummy_store(int n=20){
      for( int i=1;i<=n;++i ){
	data.push_back(i);
      }
    }

    const std::vector<int> & operator[](qtl::expr &dummy){
      return data;
    }

    void get(const std::string & dummy){}

    friend std::ostream & operator<<(std::ostream &o,const dummy_store &d){
      o << "{";
      bool first=true;
      for( auto x:d.data ){
	if( !first ){ o << ", "; }
	o << x;
	first=false;
      }
      o << "}";
      return o;
    }
  } file(50);



#endif

#undef TRACE
#define TRACE(x) x


#if 0
struct error_handler
        {
            template <typename Iterator, typename Exception, typename Context>
            x3::error_handler_result on_error(
                Iterator& first, Iterator const& last
              , Exception const& x, Context const& context)
            {
                auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
                std::string message = "Error! Expecting: " + x.which() + " here:";
                error_handler(x.where(), message);
                return x3::error_handler_result::fail;
            }
        };
#endif

int main( int argc, char *argv[] ){ 
using namespace std::string_literals;  
//using namespace lex::literals;
//using namespace qtl::literals;
// std::cout << file << "\n";
 std::cout << __LINE__ << "\n";
 // file.get("stuff.dat");
  file.get("stuff.0.dat");
 std::cout << __LINE__ << "\n";
 std::cout << file << "\n";
 std::cout << __LINE__ << "\n";
 { std::ofstream pf("pipes/.pid");
    pf << getpid() << "\n";
    pf.close();
 }
 while(1){
 std::cout << __LINE__ << "\n";
 std::ifstream in ("pipes/.qd",std::ofstream::in);
 std::cout << __LINE__ << "\n";
 if( !in.is_open() ){
   perror("open pipes/.qd failed");
   exit(1);
 }
 while( in.good() ){
   std::string query;
   std::getline(in, query);
 std::cout << "in.rdstate()" << in.rdstate() << "\n"
  << in.good() << " "
  << in.fail() << " "
  << in.eof() << " "
  << in.bad() << "\n";

 std::cout << "["<< query << "]\n";


 namespace x3 = boost::spirit::x3;
      //  using qtl::start_rule;
 static auto const name_rule=(
                 x3::rule<struct as_string, std::string>{}= ( x3::raw[
      //		    x3::char_("!#$%+0-9=A-Z[]a-z_~") >> *x3::char_("-!#$%+.0-9=A-Z[]a-z_~")
		    x3::char_("!--0-~") >> *x3::char_(" -.0-~")
		 ] [ qtl::to_string ] )
	      );

      static auto const dispatch_rule= x3::rule<struct dispatch, std::pair<std::string,qtl::expr>>{}=
	(name_rule >> x3::lit("/") >> qtl::expr_rule )
       [ ([](auto& ctx){
	 _val(ctx).first  = at_c<0>( _attr(ctx) );
	 _val(ctx).second = at_c<1>( _attr(ctx) );
       })]
	;
    std::pair<std::string,qtl::expr> result;
    //       static auto const start_rule =qtl::expr_rule_t{}=/*qtl::expr_rule=*/  qtl::not_rule;
	  try{
	    auto b=query.begin();
	    auto e=query.end();
	    //	    std::cout <<  qtl::type_name<decltype(b)>() << "\n"; 
	    while( b!=e ){
	     boost::spirit::x3::ascii::space_type space;
             auto p=phrase_parse( b,e, dispatch_rule,space/**/,result/**/ );
  	      if( !p ){ break; }
	      std::cout << "parsed " << std::string(query.begin(),b) << "\n";
	    }
	    if( b!=e ){
	      std::cout << "parse ended at " << std::string(b,e) << '\n';
            }
	  }catch (x3::expectation_failure<std::string::const_iterator> const& e) { 
	      std::cout << "expectation_failure: " << e.what() << " : " << e.which() << " : " << *e.where() << /*std::endl*/ '\n';
	      //	            std::cout << "Expected: " << e.which() << " at '" << std::string(e.where(), query.end()) << "'\n";
	      //	      std::cout << "expected: "; print_info(x.what_);
	      //	      std::cout << "got: \"" << std::string(x.first, x.last) << '"' << std::endl;
	      continue;
	  } catch (const std::exception& ex) {
	    std::cout << "EXCEPTION: " << ex.what() << "\n";
	      continue;
         }
          auto name=result.first;
	  auto search=result.second;
	  if( name.empty() ){
	    std::cerr << "(" << query << ") no pipename\n";
	    continue;
	  }
	  if( (int)search.o < 0 ){
	    std::cerr << "(" << query << ") bad expression\n";
	    continue;
	  }
	  std::cout << name << '/' << search << '\n';
	  std::cout << search.stringify() << '\n';
	  //	  char tempname[]="pipes/XXXXXX";
	  //	  strcpy(tempname,"pipes/XXXXXX");
	  //	  std::cout << tempname << "\n";
	  //	  mkdtemp(tempname);
	  //	  std::cout << tempname << "\n";
	  std::string path="pipes/" + name + "/pipe";
	  std::cout << path << "\n";
	  std::fstream pp(path,std::fstream::out|std::fstream::in /* keeping an extra open in handle keeps the wwrites from exiting when the real read closes */ );
	    if( !pp.is_open() ){
	      std::cerr << "can't open pipe\n";
	      continue;
	    }
	    //	    std::cerr << "pp good" <<  pp.good() << " eof" << pp.eof() << " fail" << pp.fail() << " bad" << pp.bad() << "\n";
	    //          pp <<  search.stringify() << "\n";
	    //	    std::cerr << "pp good" <<  pp.good() << " eof" << pp.eof() << " fail" << pp.fail() << " bad" << pp.bad() << "\n";

	    if( auto pid=fork() ){
	      std::ofstream pf("pipes/"+name+"/pid");
	      pf << pid << "\n";
	      pf.close();
	      pp.close();
	      {
		system("lsof | egrep 'pipes|qd|FIFO' &");
	      }
  	      std::cout << "forked " << pid << "\n";
	    }else{
	     in.close();
	     fclose(stdin);
	    
	    //	    fclose(stdout);
	    std::cerr << "started " << getpid() << " pp.rdstate= " << pp.rdstate() << "\n";
	    std::cerr << getpid() << " " << __LINE__
		      << " g" <<  pp.good() 
		      << " f" << pp.fail()
                      << " e" << pp.eof()
		      << " b" << pp.bad()
		      << "\n";
	    pp << "[start " << getpid() << " " <<  search.stringify() << "]\n";
	    //pp.flush();
	    std::cerr << getpid() << " " << __LINE__
		      << " g" <<  pp.good()
		      << " f" << pp.fail()
		      << " e" << pp.eof()
		      << " b" << pp.bad()
		      << "\n";

	    TRACE( std::cerr << getpid() << "file[" << search.stringify() << "]" << "\n"; )

	    for( auto x:
		   file[search]
		 ){

	      if( !pp.good() ){
		perror(STR(__LINE__));
	      }

	      std::cerr << getpid() << " " << __LINE__
		      << " g" <<  pp.good()
		      << " f" << pp.fail()
		      << " e" << pp.eof()
		      << " b" << pp.bad()
		      << "\n";

	      pp << x << "\n";
  	      pp.flush();

#define TEST_DELAY 200'000
#ifdef TEST_DELAY
	      usleep(TEST_DELAY);
#endif
	    }
	    pp << "[done]\n";
  	    pp.flush();
	    std::cerr << getpid() << " " << __LINE__
	    << " g" <<  pp.good()
    	    << " f" << pp.fail()
            << " e" << pp.eof()
	    << " b" << pp.bad()
	    << "\n";
    	   std::cerr << "sleep(10-" << sleep(10) << ")";
	  	    std::fstream rr(path,std::fstream::in);
	    pp.close(); 
	    std::cerr << "fork " << getpid() << " done\n";
	    std::cerr << "p.is_open()" << pp.is_open() << "\n";
	    std::cerr << getpid() << " " << __LINE__
		      << "r.is_open()" << rr.is_open()
	    << " g" <<  rr.good()
    	    << " f" << rr.fail()
            << " e" << rr.eof()
	    << " b" << rr.bad()
	    << "\n";
	    while( auto s=sleep(3600) ){
	      std::cerr << getpid() << " interupted sleep(" << s << ")\n";
	    }
	    {
	       namespace fs = std::filesystem;
	       std::string dir="pipes/" + name ;
	       auto n=fs::remove_all(fs::path(dir.c_str()));
	       TRACE( std::cout << "Deleted " << n << " " << dir << "\n"; )
	    }
	    std::cerr << "fork " << getpid() << " exiting\n";
	    exit(EXIT_SUCCESS);
	  }
 }
 std::cerr << "in.rdstate() " << in.rdstate() << "\n"
  << in.good() << " "
  << in.fail() << " "
  << in.eof() << " "
  << in.bad() << "\n";
 }
}
