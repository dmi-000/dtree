#include <chrono>
#include <random>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <string>

#include <boost/functional/hash.hpp>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <strstream>

#include <iostream> // std::cout
#ifndef TRACE
#define TRACE(x) x
#endif
#ifndef NOTRACE
#define NOTRACE(x)
#endif
namespace qtl{
#ifndef XSTR
# define XSTR(s) STR(s)
#endif
#ifndef STR
# define STR(s) #s
#endif
  
 #define BASE_T std::strstreambuf
 class FILEbuf:public BASE_T{
  using base_t=BASE_T;
#undef BASE_T
  using base_t::base_t;
   char buf[4096];   
    FILE *F;
    inline static const std::map<std::ios_base::openmode,const char*>modes={
      {std::ios_base::in,"r"},
      {std::ios_base::out,"w"},
      {std::ios_base::app,"a"},
      {std::ios_base::trunc,"w"},
      {std::ios_base::ate,"a+"},
    };
    inline static const std::map<int,const char*> flags={
      {O_RDONLY,"r"},
      {O_WRONLY,"w"},
      {O_WRONLY+O_EXCL,"wx"},
      {O_WRONLY+O_APPEND,"a"},

      {O_RDWR,"r+"},
      {O_RDWR+O_CREAT,"w+"},
      {O_RDWR+O_APPEND,"a+"},
      {O_RDWR+O_EXCL,"w+x"},
    };
    bool failbit=0;
    int O_FLAG;
    
  public:
    void clear(){ failbit=0; }
    FILEbuf(const char *path,  const char * mode):F(std::fopen(path,mode)){
      TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
      if( F ){
	O_FLAG=(strchr(mode,'r')?O_RDONLY:0)
	    |(strchr(mode,'w')?O_WRONLY:0)
  	    |(strchr(mode,'+')?O_RDWR:0)
	    |(strchr(mode,'x')?O_EXCL:0)
	    |(strchr(mode,'a')?O_APPEND:0)
	  ;
      }else{
	perror(path);
	failbit=1;
      }
      
    }

    FILEbuf(const char *path,  int oflag):O_FLAG(oflag){
        TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
     auto fd= open(path,oflag);
     if( fd < 0 ){
       perror("open failed");
       exit(1);
     }
     F=fdopen(fd,flags.at(oflag&(O_RDONLY|O_WRONLY|O_RDWR|O_APPEND|O_CREAT|O_EXCL)));
     if( !F ){
       perror("fdopen failed");
       exit(1);
     }
    }

    FILEbuf(const std::string &path,  const std::string &mode):FILEbuf(path.c_str(),mode.c_str()){
      TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
    }
    FILEbuf(const std::string &path,  std::ios_base::openmode mode = std::ios_base::in):FILEbuf(path.c_str(), modes.at(mode)){
        TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
	if( !modes.contains(mode) ){
	  std::cerr << "unknown mode " << mode << "\n";
	  exit(1);
	}
      }
    FILEbuf(const std::string &path, int oflag):FILEbuf(path.c_str(),oflag){
        TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
    }
    int fileno()const{ return ::fileno(F); }
    bool eof()const{ return feof(F); }
    bool bad()const{ return ferror(F); }
    bool fail()const{ return failbit||ferror(F); }
    bool good()const{ return !(eof()||fail()); }
    std::ios_base::iostate rdstate() const{
      return
	(bad() ?std::ios_base::badbit: (std::ios_base::iostate)0)|
        (failbit?std::ios_base::failbit: (std::ios_base::iostate)0)|
	(eof() ?std::ios_base::eofbit: (std::ios_base::iostate)0);
    };
    FILEbuf& flush(){
      NOTRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
      if( O_FLAG && base_t::pcount() ){
	auto n=::write(fileno(),base_t::pbase(),base_t::pcount());
	if( n != base_t::pcount()  ){
	  perror("write");
	  std::cout << n << " of " << base_t::pcount() << "\n" ;
	}
	base_t::setp(base_t::pbase()+n,base_t::epptr());
      }
      if( fflush(F) ){
      	perror("fflush");
	exit(1);
      }
      return *this;
    }
    std::string getline(){
        const int buflen=1024;
	char buf[buflen];
	std::string ret;
        while( !feof(F) ){
	  if( !std::fgets(buf,buflen,F) ){
	    if( ferror(F) ){
	      perror("fgets");
	      failbit=1;
	    }else if( feof(F) ){
	      // perror("fgets eof");
	      failbit=1;
	    }else{
	      perror("fgets");
	      failbit=1;
	    }
	    return ret;
	  }
	  if( buf[0] ){
	    ret.insert(ret.length(),buf);
	    if( ret.back()=='\n' ){
	      ret.pop_back();
	      return ret;
	    }
	  }else{
	    perror("fgets null buffer");
	  }
	}
	perror("getline: eol not found");
        failbit=1;
        return ret;
    };
    void clearerr(){
         TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
      ::clearerr(F);
      failbit=0;
    }
    void close(){
      TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
      fclose(F) && (perror("fclose"),0);
    }
    int capacity(){
     #ifdef  F_GETPIPE_SZ
      return fcntl(fileno(), F_GETPIPE_SZ);
      #else
      return 65535;
      #endif
    }
    long showmanyc(){
      int sz=-1;
      if( ioctl(fileno(), FIONREAD, &sz) < 0 ){
	perror("ioctl(FIONREAD)");
      }
#if 0
      auto pos=ftello(F);
      if( fseeko(F,0,SEEK_END) ){
	perror("fseeko(F,0,SEK_END)");
      }else{
	auto e=ftello(F);
	TRACE( std::cout << e << "-" << pos << "=" << (e-pos) << "\n"; )
      }
      if( fseeko(F,pos,SEEK_SET) ){
	  TRACE(std::cout << "fseeko(F,"<<pos<<",SEEK_SET)\n";)
	perror("fseeko(F,pos,SEEK_SET)");
     }
#endif

      TRACE( std::cout << __PRETTY_FUNCTION__ << "=" << sz << "\n" );
      return sz;
    }
    bool wouldblock(){
      auto en=errno;
      if( showmanyc() ){ return false; }
      
	auto fe=ferror(F);
        auto fl=fcntl(fileno(),F_GETFL);
        fcntl(fileno(), F_SETFL, O_NONBLOCK);
	errno=0;
	auto c=fgetc(F);
	auto b=  /* c == EOF && */ (errno == EAGAIN || errno == EWOULDBLOCK);
        if( errno ){ TRACE( perror("errno "); ) }
	if( !b ){
	  TRACE( std::cerr << "ungetc("<<c<<")\n"; )
	  ungetc(c,F);
	  /*****/ if( c==EOF ){ b=true; }
	}else{
	}
	fcntl(fileno(), F_SETFL, fl);
	errno=en;
	if( !fe ){ clearerr(); }
	return b;
    }
    bool waitforinput(long us){
      TRACE( std::cout << "waitforinput(" << us << ")\n"; )
	//TRACE( std::cout << "showmanyc=" << showmanyc() << "\n"; )
      if( us<0 ){
	std::cerr << "wait expired " << us << "\n";
	return false;
      }
      TRACE( if( ferror(F) ){ perror(XSTR(__LINE__)); } )

      if( !wouldblock() ){
	  return true;
      }

      fd_set readfds;
      FD_ZERO(&readfds);
      FD_SET(fileno(), &readfds);
      struct timeval timeout;
      timeout.tv_sec = us/1'000'000;
      timeout.tv_usec = us%1'000'000;
      auto activity=select( fileno() + 1 , &readfds,NULL,NULL,&timeout);
	if( activity < 0  ){  
            perror("select error");  
	    if( (errno!=EINTR) ){ exit(1); }
        }  
	if( activity==0 ){
	  std::cerr << "select timeout\n";
	  failbit=1;
	}else{
	  std::cerr << "select=" << activity << "\n";
	}

       TRACE( if( ferror(F) ){ perror(XSTR(__LINE__)); } )

       TRACE( std::cerr << "activity=" <<  activity << "\n"; )

	return activity>0;
    }
    bool waitforinput( std::chrono::time_point<std::chrono::system_clock> endtime){
      return waitforinput(std::chrono::duration_cast<std::chrono::microseconds>(endtime-std::chrono::system_clock::now()).count());
    }
    friend FILEbuf& getline(FILEbuf &f,std::string &s){
      s=f.getline();
      return f;
    }

protected:
    virtual int sync(){
      TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
      if( (O_FLAG&O_WRONLY) && base_t::pcount() ){
	auto w=fwrite(base_t::pbase(),1,base_t::pcount(),F);
	if( w != pcount() ){
	  perror("fwrite");
	  std::cout << w << " of " << base_t::pcount() << "\n" ;
	}
	base_t::setp(base_t::pbase()+w,base_t::epptr());
      }	
      if( fsync(fileno()) ){
	perror("fsync");
	return -1;
      }
      return base_t::pcount()?-1:0;
    }
    int underflow() {
      NOTRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
      NOTRACE(
      std::cout << "in_avail()=" << base_t::in_avail() << "\n";
      std::cout << "pcount()=" << base_t::pcount() << "\n";
      std::cout << "eback()=" << (void*)base_t::eback() << "\n";
      std::cout << "gptr()=" << (void*)base_t::gptr() << "\n";
      std::cout << "egptr()=" << (void*)base_t::egptr() << "\n";
	      )
      //      assert( base_t::pcount()==0 );
	if( eof() ){ return std::istream::traits_type::eof(); }
	int c=fgetc(F);
	NOTRACE( std::cout << "fgetc(F)=" << c << "\n"; )
	if( c==EOF ){ return std::istream::traits_type::eof(); }
	if( !eback() ){
	  base_t::setg(buf,buf+sizeof(buf),buf+sizeof(buf));
	  NOTRACE(
         std::cout << "in_avail()=" << base_t::in_avail() << "\n";
         std::cout << "pcount()=" << base_t::pcount() << "\n";
         std::cout << "eback()=" << (void*)base_t::eback() << "\n";
         std::cout << "gptr()=" << (void*)base_t::gptr() << "\n";
         std::cout << "egptr()=" << (void*)base_t::egptr() << "\n";
		  )
	}
auto r=base_t::sputbackc(c);
 NOTRACE(
      std::cout << "in_avail()=" << base_t::in_avail() << "\n";
      std::cout << "pcount()=" << base_t::pcount() << "\n";
      std::cout << "eback()=" << (void*)base_t::eback() << "\n";
      std::cout << "gptr()=" << (void*)base_t::gptr() << "\n";
      std::cout << "egptr()=" << (void*)base_t::egptr() << "\n";
      std::cout << "r=" << r << "\n";
	 )
      return r;

    }

 };// end class FILEbuf

 #define BASE_T  std::iostream
 class FILEstream:public BASE_T{
   public:
   using base_t=BASE_T;
#undef BASE_T
   using base_t::base_t;
   FILEbuf Fb;
   template<typename ...Ts>
   FILEstream(Ts... t):Fb(t...),base_t(&Fb){
     TRACE( std::cout << __PRETTY_FUNCTION__ << "\n"; );
   }
   auto capacity(){ return Fb.capacity(); }
   auto close(){ return Fb.close(); }
   auto clearerr(){ return Fb.clearerr(); }
   template  <typename T>
   auto waitforinput(T us){ return Fb.waitforinput(us); }
 };// end class FILEstream
}// end namespace qtl

#if __INCLUDE_LEVEL__ + !defined __INCLUDE_LEVEL__ < 1+defined TEST_H
//#include <fstream>
int main(){
  qtl::FILEstream in(__FILE__,"r");
  qtl::FILEstream out("in.test","w");
  while( in.good() ){
    std::string s;
    in >> s;
    out << s;
    std::cout << "s=" << s << "\n";
    //    out << "[" << in.getline() << "]\n";
    out.flush();
  }
}
#endif 
