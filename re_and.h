namespace qtl{
     bool re_and(const std::string &r0,const std::string &r1){
       auto p=fork();
       if( p>0 ){
	 int status;
	 waitpid(p,&status,0);
	 return WEXITSTATUS(status);
       }else if( p==0 ){
	 TRACE( std::cout << "re_and.py " << r0 << " " << r1 << "\n"; )
	   auto e=execl("re_and.py","re_and.py",r0.c_str(),r1.c_str(),0);
	 perror("exec re_and.py");
	 exit(1);
       }else{
	 perror("fork");
       }
       return 1;
     }
}



