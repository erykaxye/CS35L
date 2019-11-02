#include<stdio.h> 
#include<stdlib.h>
#include<string.h> 

void check_file_err(FILE *p);

int main(int argc, const char* argv[])
{

  /* declare all variables */
  const char *from, *to; 
  char c;
  char map[256] = {'\0'};
  size_t i;
  int end, flen, tlen; 

  /* check number of arguments */
  if(argc != 3)
    {
      fprintf(stderr, "invalid arguments\n");
      exit(1);
    }

  /* initialize variables */
  from = argv[1]; 
  to = argv[2];
  flen = strlen(from); 
  tlen = strlen(to);
  end = feof(stdin);

  /* compare to and from string lengths */
  if(flen != tlen)
    {
      fprintf(stderr, "to and from string lengths differ\n"); 
      exit(1); 
    }

  /* check if from has duplicates by populating map */
  for(i = 0; i < flen; i++)
    {
      if(map[from[i]] != '\0')
	{
	  fprintf(stderr, "from duplicates\n");
	  exit(1); 
	}
      map[from[i]] = to[i];
    }

  while(!end)
    {
      c = getchar();
      check_file_err(stdin);
      end = feof(stdin); 
      if(!end)
	{
	  /* compare char to map */
	  if(map[c] != '\0')
	    c = map[c];
	  /* print output */
	  putchar(c);
	  check_file_err(stdout);
	}
    }
}

/* file error check function */
void check_file_err(FILE *p)
{
  if (ferror(p))
    {
      fprintf(stderr, "IO Error\n");
      exit(1);
    }
}
