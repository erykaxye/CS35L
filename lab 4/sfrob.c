#include <stdio.h>
#include <stdlib.h> 

void check_mem_err(void* p);
void check_file_err(FILE *p); 
int frobcmp(char const *a, char const *b);
int compare(const void *x, const void *y);

int main(void)
{
  /* declare all variables */
  char c, *i, *temp, **tmp, **arr;
  size_t linewidth, linenum, n, m; 
  int end; 

  /* initialize variables */
  linewidth = 0;
  linenum = 0; 
  end = feof(stdin); 

  i = (char*)malloc(sizeof(char));
  check_mem_err(i);
  arr = (char**)malloc(sizeof(char*));
  check_mem_err(arr);
    
  while (!end)
    { 
      c = getchar(); 
      check_file_err(stdin);
      end = feof(stdin); 
      /* check if its a space at the beginning of the line */
      if(!linewidth && c == ' ')
	continue;
      /* add character to line */  
      if(!end)
	{
	  i[linewidth] = c;
	  linewidth++;
	  /* increase line size */
	  temp = (char*)realloc(i, sizeof(char) * (linewidth+2));
	  check_mem_err(temp);
	  i = temp;
	  if(c != ' ')
	      continue; 
	  else
	    {
	      /* if space, add new line and store input into array */
	      arr[linenum] = i;
	      linenum++; 
	      /* increase line numbers of array and reset input */
	      tmp = (char**)realloc(arr, sizeof(char*) * (linenum+2));
	      check_mem_err(tmp);
	      arr = tmp;
	      i = NULL; 
	      i = (char*)malloc(sizeof(char));
	      linewidth = 0;
	    }
	}
      else
	{
	  /* check if empty file */   
	  if(!linenum)  
	    {
	      free(i);
	      return 0;
	    } 
	  /* check if input not empty not 0 */
	  if(linewidth)
	    {
	      /* check if space at the end, add if not */
	      if(i[linewidth-1] != ' ')
		i[linewidth++] = ' '; 
	      /* then add to array */
	      arr[linenum] = i;
              linenum++; 
              tmp = (char**)realloc(arr, sizeof(char*) * (linenum+2));
              check_mem_err(tmp);
              arr = tmp;
              i = NULL; 
              linewidth = 0;
	    }
	  /* if previous line ends with space, break out of loop */
	  if(!linewidth)
	      break; 
	}
    }   

  /* sort using qsort */ 
  qsort(arr, linenum, sizeof(char*), compare);

  /* prints the array */
  for(n = 0; n < linenum; n++)
    {
      for(m = 0; ; m++)
	{
	  putchar(arr[n][m]);  
	  check_file_err(stdout);
	  if(arr[n][m] == ' ')
	    break; 
	}
    }
  
  /* deletes the array */
  for(n = 0; n < linenum; n++)
      free(arr[n]);

  free(arr);
  exit(0);
}

/* error check functions */
void check_mem_err(void *p)
{
  if (p == NULL) 
    {
      fprintf(stderr, "Memory Allocation Error\n"); 
      exit(1);
    }
} 

void check_file_err(FILE *p)
{
  if (ferror(p))
    {
      fprintf(stderr, "IO Error\n"); 
      exit(1);
    }
}

/* compares each character */
int frobcmp(char const *a, char const *b) 
{
  for(; *a == *b; a++, b++)
    {
      if(*a == ' ')
	return 0;
    }
  return(((*a^42) > (*b^42)) ? 1 : -1);
}

/* puts input into correct format */ 
int compare(const void *x, const void *y)
{
  const char *a = *(const char**)x; 
  const char *b = *(const char**)y; 
  return frobcmp(a, b);
}
