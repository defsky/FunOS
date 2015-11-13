#ifndef	_DEBUG_H_
#define	_DEBUG_H_

#define	DEBUG

#ifdef	DEBUG
void	assertion_failure(char* exp,char* file,char* base_file,int line);
#define	assert(exp)	if(exp);\
	else	assertion_failure(#exp,__FILE__,__BASE_FILE__,__LINE__)
#else
#define	assert(exp)	
#endif

void	spin(const char* function_name);

#endif

