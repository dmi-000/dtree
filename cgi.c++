#define TRACE(x)
//#include "qtl/sql.h"

//#include "qtl/expr.h"
#include <iostream>
#include <chrono>
#include <random>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <charconv>
#include <filesystem>
#include <fstream>

#undef TRACE
#define TRACE(x) x

#include <boost/functional/hash.hpp>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#if __has_include(<crypt.h>)
# include <crypt.h>
#endif
#include <string.h>
#include <cstring>

#include "qtl/FILEstream.h"
//namespace x3 = boost::spirit::x3;
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

int timeout=400'000;
int main( int argc, char *argv[] ){ 
using namespace std::string_literals;  
//using namespace lex::literals;
//using namespace qtl::literals;
 if( argc < 2 || argv[1][0]=='+' && argc < 3  ){
   std::cout << "usage: " << argv[0] << " [+microsecondtimeout] <query|-cursorname>\n";
   exit(1);
 }
 using namespace std::literals;
  if( argv[1][0]=='+' && '0' <= argv[1][1] && argv[1][1] <= '9' ){
    timeout=atoi(argv[1]+1);
    argv++;
  }
 auto endtime = std::chrono::system_clock::now()+ std::chrono::microseconds(timeout); // 400ms;
 chdir("pipes");
  auto const size_len=(std::numeric_limits<std::size_t>::digits+4)/5;
  char X[]="_XXXXXX";
  char pipedir[size_len+sizeof(X)+1];
  std::string pipename;
 if( *argv[1]=='-' ){
   ++argv[1];
   auto l=std::strspn(argv[1],"#.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~");
   if( l==0 || l > sizeof(pipedir)-1 ){
     std::cerr << "invalid name\n";
     exit(1);
   }
   *(argv[1]+l)='\0';
   std::strcpy(pipedir,argv[1]);
    pipename=std::string(pipedir)+"/pipe";
 }else{
   std::ofstream qd (".qd",std::ios_base::out);
   if( !qd.is_open() ){
     perror("ofstream(\".qd\") failed");
     exit(1);
   } 

     std::cout << argv[1] << "\n";
#if 0
    std::size_t seed = 0;
    boost::hash_combine( seed, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() );
    boost::hash_combine( seed, getpid() );
    if( auto [ptr, ec]=std::to_chars(pipedir,pipedir+size_len,seed,36); ec == std::errc() ){
      strcpy(ptr,X);
    }else{
      std::cerr << "to_chars(pipedir,pipedir+"<<size_len<<","<<seed<<",36) failed\n";
      exit(1);
    }
#else
    const char digits[65]="0123456789ABCDEFGHIJKLMNOPQRSTUVWZYZabcdefghijkjmnopqrstuvwxyz/.";
    char ps[size_len+1];
#if 0
    if( auto [ptr, ec]=std::to_chars(ps,pipedir+size_len,getpid()%(36*36)+36*36,36); ec == std::errc() ){
      *ptr='\0';
      std::cout << getpid() << " " << getpid()%(36*36) << " " << ps << "\n";
    }else{
        std::cerr << "to_chars(ps,ps+"<<size_len<<","<<getpid()%(36*36)+36*36<<",36) failed\n";
        exit(1);
      }        
#else
    auto pid=getpid();
    ps[0]='1';
    ps[1]=digits[(pid/64)%64];
    ps[2]=digits[pid%64];
    ps[3]='\0';
    std::cout << "pid " << pid << " " << ps+1 << "\n";
#endif
      char ts[size_len+1];
#if 0
      auto t=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#else
     std::uniform_int_distribution<long> d(0, 36l*36*36*36*36*36*36*36-1);
     std::random_device rd1; // uses RDRND or /dev/urandom
   auto t=d(rd1);
#endif
      if( auto [ptr, ec]=std::to_chars(ts,ts+size_len,t,36); ec == std::errc() ){
         *ptr='\0';
	 std::cout << "seed " << t << " " << ts << "\n";
      }else{
        std::cerr << "to_chars(ts,ts+"<<size_len<<","<< t <<",36) failed\n";
        exit(1);
}        
      auto cryp=crypt(ts,ps+1);
      std::cout << cryp << "\n";
      auto p=stpcpy(pipedir,cryp+3);
      std::replace(pipedir,p,'/','~');
      std::replace(pipedir,pipedir+1,'.','#');
      stpcpy(p-1,X);
#endif
    std::cout << pipedir << "\n";
   if( !mkdtemp(pipedir) ){
     perror("mkdtemp failed");
      exit(1);
   }
    std::cout << pipedir << "\n";
    pipename=std::string(pipedir)+"/pipe";
    std::cout << pipename << "\n";
    if(  mkfifo(pipename.c_str(), 0666) ){
	    perror("mkfifo failed");
	    exit(1);
     }
     qd <<  pipedir << "/" << argv[1] << "\n";
 }
 TRACE( std::cout << __LINE__ << "\n"; );
 qtl::FILEstream results(pipename, /* O_RDWR*/ O_RDONLY|O_NONBLOCK| O_CLOEXEC );
 TRACE( std::cout << __LINE__ << "\n"; );
	  {
	    int cap = results.capacity();
             std::cout << "Pipe capacity: " << cap << "\n";
	  }
	  while( results.good() && results.waitforinput( endtime ) ){
	    std::cerr << "results " << results.rdstate()
		      << " g" << results.good()
		      << " f" << results.fail()
		      << " e" << results.eof()
		      << " b" << results.bad()
		      << "\n";
	    std::string r;
	    getline(results,r);
	    std::cout << "result:[" << r << "]\n";
	  }
	  std::cout << "results.rdstate() " << results.rdstate() << "\n"
		      << " g" << results.good()
		      << " f" << results.fail()
		      << " e" << results.eof()
		      << " b" << results.bad()
		    << "\n";
	   if( results.eof() ){
	     {
	     std::string s="ls -lA "s+pipedir;
	     system(s.c_str());
	     }
	     {
	     std::string s="ps -wl -p `cat "s+pipedir+"/pid` &";
	     //std::cout << s << "\n";
	     system(s.c_str());
	     }
	     {
	     std::string s="lsof -p `cat "s+pipedir+"/pid`&";
	     //std::cout << s << "\n";
	     system(s.c_str());
	     }
	       namespace fs = std::filesystem;
	       sleep(5);
	       auto n=fs::remove_all(fs::path(pipedir));
	       std::cout << "Deleted " << n << " " << pipedir << "\n";
	   }else{
	     std::cout << "-" << pipedir << "\n";
             results.clearerr();
	     results.close();
	   }
}
