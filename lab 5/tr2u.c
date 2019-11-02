#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, const char* argv[])
{

  /* declare all variables */
  const char *from, *to;
  char buf[1];
  char map[256] = {'\0'};
  size_t i, n;
  ssize_t input, output; 
  int end, flen, tlen;

  /* check number of arguments */
  if(argc != 3)
    {
      fprintf(stderr, "invalid arguments\n");
      exit(1);
    }

  /* initialize variables */
  n = 1;
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

  input = read(0, buf, n); 
  while(input > 0)
    {
      /* compare buffer to map */
      if(map[buf[0]] != '\0')
	buf[0] = map[buf[0]]; 
      /* write output and read next input */
      output = write(1, buf, n);
      input = read(0, buf, n);
    }
}
