#include	"debug.h"
#include	"syscall.h"

void	assertion_failure(char* exp,char* file,char* base_file,int line)
{
	printl("\nassert(%s) failed,file:%s,base_file:%s,ln%d\n",exp,file,base_file,line);
	spin("assertion_failure()");
}

void	spin(const char* function_name)
{
	printl("spin in %s\n",function_name);

	while(1){}
}
