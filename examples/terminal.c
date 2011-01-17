#include <stdio.h>
#include <string.h>
#include "clipboard.h"
#include <stdlib.h>

void interactive()
{
	printf("Interactive mode!\n");
	char in[256];
	int i = 0;
	int size = 0;
	while(fgets(in, 256, stdin))
	{
		if(strncmp(in, "q", 1) == 0)
			break;
		if(strncmp(in, "set", 3) == 0)
		{
			int l = strlen(in);
			size = l - 5;
			if(l>5)
			{
			   // strndup not implemented in MinGW 3.4.5, workaround
			   char* copy = (char*)malloc(size + 1);
			   if (copy) {
			      for (i = 0 ; i < size ; ++i) {copy[i] = in[4 + i];}
			      copy[size] = '\0';
   //				char *copy = strndup(in+4, l-5);
               if(Set_clipboard_text(copy, l-5))
               {
                  printf("Clipboard set: %s\n", copy);
               }
                   else
                   {
                       fprintf(
                               stderr,
                               "Set failed: %s\n",
                               Get_clipboard_errmsg());
                   }
               free(copy);
			   }
			}
		}
		if(strncmp(in, "get", 3) == 0)
		{
			const char* get = Get_clipboard_text();

            if(get)
            {
			    printf("Clipboard got: %s\n", get);
            }
            else
            {
                fprintf(
                        stderr,
                        "Get failed: %s\n",
                        Get_clipboard_errmsg());
            }
		}
	}
}

int main(int argc, char *argv[])
{
	printf("Hello clipboard!\n");
	if(argc<2) //usage
	{
		return 0;
	}
	if(strncmp(argv[1], "-i", 2) == 0)
	{
		interactive();
	}	

	return 0;
}
