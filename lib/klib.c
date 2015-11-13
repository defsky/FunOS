#include	"type.h"
#include 	"global.h"

PUBLIC	void	dec2a(char * buf,int n)
{
	int k = n;
	int m = 1;

	while(k >= 10)
	{
		k = k / 10;
		m = m * 10;
	}
	
	while(m > 0)
	{
		k = n / m;
		*buf++ = k + '0';
		n = n % m;
		m = m / 10;
	}
}

PUBLIC	void	itoa(unsigned char * p,u32 n)
{
	int i;
	unsigned char temp;
	int flag=0;
	
	*p++ = '0';
	*p++ = 'x';
	if(n == 0)
	{
		*p++ = '0';
	}
	else
	{
		for(i=7;i>=0;i--)
		{
			temp = (u8)((n >> (i*4)) & 0x0000000f);
			if(flag || temp > 0)
			{
				flag = 1;
				if(temp > 9)
				{
					temp = temp - 0xa;
					temp += 'A';
				}
				else
				{
					temp += '0';
				}
				*p++ = temp;
			}
		}
	}
	*p = '\0';
}

PUBLIC	void	disp_int(u32 n)
{
	unsigned char temp[16];
	itoa(temp,n);
	disp_str(temp);
}

