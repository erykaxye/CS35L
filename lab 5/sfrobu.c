#include<stdlib.h> 
#include<string.h>
#include<unistd.h> 
#include<sys/stat.h> 
#include<sys/types.h>  

int f = 0; 

void check_mem_err(void* p);
void check_file_err(ssize_t n);
int decrypt(char c);
int frobcmp(char const *a, char const *b);
int compare(const void *x, const void *y);

int main(int argc, const char* argv[])
{
  /* declare all variables */
  char c, *i, *str, *temp, **tmp, **arr, rbuf[1], wbuf[1], ebuf[20];
  size_t bufsize, filesize, linewidth, linenum, n, m;
  ssize_t input, output, fstatout;
  int size, end, err; 
  struct stat inputbuf;
  char argf[] = "-f";

  /* initialize variables */
  bufsize = 0;
  linewidth = 0;
  linenum = 0;
  size = 1;
  end = 0;
  err = 20; 
  m = 0;

  fstatout = fstat(0, &inputbuf);
  check_file_err(fstatout);
  filesize = inputbuf.st_size + 1; 

  /* check -f argument */
  if(argc == 2 || argc == 3)
    {
      if(!strcmp(argv[1], argf))
	f = 1;
      else
	{
	  char buf[] = "Invalid Arguments\n";
	  int err = write(2, buf, 17);
	  exit(err);
	}
    }

  if(S_ISREG(inputbuf.st_mode))
    {
      i = (char*)malloc(sizeof(char)*filesize);
      check_mem_err(i);

      while (!end)
	{
	  input = read(0, rbuf, size);
	  check_file_err(input);
	  c = rbuf[0];
	  end = !input; 
	  /* check if it we need more space */
	  if(bufsize == filesize)
	    {
	      filesize *= 2;
	      temp = (char*)realloc(i, sizeof(char)*filesize);
	      check_mem_err(temp); 
	      i = temp; 
	      temp = NULL; 
	    }
	  /* check if its a space at the beginning of the line */
	  if(!linewidth && c == ' ')
	    continue;
	  /* add character to line */
	  if(!end)
	    {
	      i[bufsize] = c;
	      bufsize++; 
	      linewidth++;
	      /* if space, increase number of lines, if not continue */
	      if(c == ' ')
		{
		  linenum++;
		  linewidth = 0;
		}
	    }
	  else
	    {
	      /* check if empty file */
	      if(!bufsize)
		{
		  free(i);
		  return 0;
		}
	      /* check if space at the end, add if not */
	      if(i[bufsize-1] != ' ')
		{
		  i[bufsize++] = ' ';
		  linenum++;
		  linewidth = 0;
		}
	      if(!linewidth)
		break; 
	    }
	}
    }
  /* if the input is not a regular file, read byte by byte */
  else
    {
      i = (char*)malloc(sizeof(char));
      check_mem_err(i);

      while (!end)
	{
          input = read(0, rbuf, size);
          check_file_err(input);
          c = rbuf[0];
          end = !input;
	  /* check if its a space at the beginning of the line */
	  if(!linewidth && c == ' ')
	    continue;
	  /* add character to line */
	  if(!end)
	    {
	      i[bufsize] = c;
	      bufsize++;
	      linewidth++;
	      /* increase line size */
	      temp = (char*)realloc(i, sizeof(char)*(bufsize+2));
	      check_mem_err(temp);
	      i = temp;
	      temp = NULL;
	      if(c == ' ')
		{
		  linenum++;
		  linewidth = 0;
		}
	    }
	  else
	    {
	      /* check if empty file */
	      if(!bufsize)
		{
		  free(i);
		  return 0;
		}
	      /* check if space at the end, add if not */
	      if(i[bufsize-1] != ' ')
		i[bufsize++] = ' ';
	      linenum++;
	      linewidth = 0;
	      /* if previous line ends with space, break out of loop */
	      if(!linewidth)
		break;
	    }
	}
    }

  /* split input into array */
  arr = (char**)malloc(sizeof(char*)*linenum);
  check_mem_err(arr);
  str = i;  
  for(n = 0; n < bufsize; n++)
    {
      if(i[n] == ' ')
	{
	  arr[m] = str;
	  m++; 
	  str = i + n + 1;
	}
    }

  /* sort using qsort */
  qsort(arr, linenum, sizeof(char*), compare);

  /* prints the array */
  for(n = 0; n < linenum; n++)
    {
      for(m = 0; ; m++)
        {
	  wbuf[0] = arr[n][m];
          output = write(1, wbuf, size);
          check_file_err(output);
          if(arr[n][m] == ' ')
            break;
        }
    }

  free(arr);
  free(i);
  return 0;
}

/* error check functions */
void check_mem_err(void *p)
{
  if (p == NULL)
    {
      char buf[] = "Memory Allocation Error\n";
      int err = write(2, buf, 24);
      exit(err);
    }
}

void check_file_err(ssize_t n)
{
  if (n < 0)
    {
      char buf[] = "IO Error\n";
      int err = write(2, buf, 9);
      exit(err);
    }
}

/* defrobs each character and checks the -f argument */
int decrypt(char c)
{
  c = c^42; 
  if(f) 
    c = toupper((unsigned char) c); 
  return c; 
}

/* compares each character */
int frobcmp(char const *a, char const *b)
{
  for(; *a == *b; a++, b++)
    {
      if(*a == ' ')
        return 0;
    }
  return(decrypt(*a) > decrypt(*b) ? 1 : -1);
}

/* puts input into correct format */
int compare(const void *x, const void *y)
{
  const char *a = *(const char**)x;
  const char *b = *(const char**)y;
  return frobcmp(a, b);
}
