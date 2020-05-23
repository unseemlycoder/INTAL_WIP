/*
DAA MINIPROJECT - UE18CS251
A C library for Integers of Arbitrary length

NAME: Akash Murthy
SRN: PES2201800266

Intal - Arbitrary Length Positive Integers
Last Updated: 2155 HRS 23 May 2020 
Submission #2
*/
//This is my implementation of intal
//Immense care has been taken to cite author if a particular snippet was lifted. i.e. BinarySearch, QuickSort
//Karatsuba is work in progress
//Relevant debugging lines were removed

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int convert_to_int(char ch)
{
  return (int)(ch - 48);
}

char convert_to_char(int in)
{
  return (char)(in + 48);
}

static char *remove_leading_zeros(const char *arr)
{
  int in = 0;
  while (arr[in] == '0')
    in++;
  int len = strlen(arr);
  int ans_len = len - in + 1;
  if (len == in)
  {
    char *ans = malloc(sizeof(char) * 2);
    ans[0] = '0';
    ans[1] = '\0';
    return ans;
  }
  char *answer = malloc(sizeof(char) * ans_len);
  answer[--ans_len] = '\0';
  for (int z = in; z < len; z++)
  {
    answer[z - in] = arr[z];
  }
  return answer;
}

//Add each character starting from LSB using ‘0’ or 48 as reference as it is stored in characters and not numbers.
//The MSB of the resultant sum is carry and LSB is what is stored in that particular position of the array.
char *intal_add(char *intal1, char *intal2)
{
  int m = strlen(intal1);
  int n = strlen(intal2);

  int l = (m > n) ? m + 1 : n + 1;
  int s = (m > n) ? n : m;

  int z, d1, d2, rd, c = 0;
  char *answer = (char *)malloc((l + 1) * sizeof(char));

  for (z = 0; z < s; ++z)
  {
    d1 = convert_to_int(intal1[m - 1 - z]);
    d2 = convert_to_int(intal2[n - 1 - z]);

    rd = d1 + d2 + c;
    c = (rd > 9) ? 1 : 0;
    rd = (rd > 9) ? rd - 10 : rd;

    answer[l - 1 - z] = convert_to_char(rd);
  }

  while (z < l - 1)
  {
    if (m > n)
    {
      rd = convert_to_int(intal1[m - 1 - z]) + c;
      c = (rd > 9) ? 1 : 0;
      rd = (rd > 9) ? rd - 10 : rd;

      answer[l - 1 - z] = convert_to_char(rd);
    }
    else
    {
      rd = convert_to_int(intal2[n - 1 - z]) + c;
      c = (rd > 9) ? 1 : 0;
      rd = (rd > 9) ? rd - 10 : rd;

      answer[l - 1 - z] = convert_to_char(rd);
    }

    ++z;
  }

  if (c != 0)
  {
    answer[0] = convert_to_char(c);
    answer[l] = '\0';
  }
  else
  {
    for (z = 0; z < l - 1; ++z)
    {
      answer[z] = answer[z + 1];
    }

    answer = (char *)realloc(answer, l * sizeof(char));
    answer[l - 1] = '\0';
  }

  return answer;
}

//This functions is used to compare the greater of the given two intals , here we first compare the lengths of both the intals , and return +1, 0 or -1
//Accordingly, and if the lengths are equal, then we check linearly for each char , starting from LSB.
int intal_compare(char *intal1, char *intal2)
{
  int m = strlen(intal1);
  int n = strlen(intal2);

  if (m > n)
    return 1;
  else if (n > m)
    return -1;

  for (int z = 0; z < m; ++z)
  {
    if (intal1[z] > intal2[z])
      return 1;
    else if (intal2[z] > intal1[z])
      return -1;
  }

  return 0;
}

//Traverse from LSB to MSB identify the bigger intel, subtract the smaller one from it. take borrows by reducing the (n-z)th bit.
char *intal_diff(char *intal1, char *intal2)
{
  int cmp = intal_compare(intal1, intal2);
  if (cmp == 0)
  {
    char *answer = (char *)malloc(2 * sizeof(char));
    answer[0] = '0';
    answer[1] = '\0';
    return answer;
  }
  else if (cmp == -1)
  {
    char *tmp = intal1;
    intal1 = intal2;
    intal2 = tmp;
  }

  int m = strlen(intal1);
  int n = strlen(intal2);

  int l = (m > n) ? m : n;
  int s = m + n - l;

  int z, d1, d2, rd, c = 0;
  char *answer = (char *)malloc((l + 1) * sizeof(char));

  for (z = 0; z < s; ++z)
  {
    d1 = convert_to_int(intal1[m - 1 - z]) - c;
    d2 = convert_to_int(intal2[n - 1 - z]);

    c = (d2 > d1) ? 1 : 0;
    d1 = (d2 > d1) ? d1 + 10 : d1;
    rd = d1 - d2;

    answer[l - 1 - z] = convert_to_char(rd);
  }

  while (z < l)
  {
    rd = convert_to_int(intal1[m - 1 - z]) - c;
    c = (rd > -1) ? 0 : 1;
    rd = (rd > -1) ? rd : rd + 10;

    answer[l - 1 - z] = convert_to_char(rd);

    ++z;
  }

  for (z = 0; z < l; ++z)
  {
    if (answer[z] != '0')
      break;
  }

  answer[l] = '\0';
  int trim = z;
  if (trim != 0)
  {
    for (z = 0; z <= l - trim; ++z)
      answer[z] = answer[z + trim];

    answer = realloc(answer, (l - trim + 1) * sizeof(char));
  }

  return answer;
}

//Traverse from LSB to MSB of intal2 , multiply char to char and add carry to the next one instantly.
char *intal_multiply(char *intal1, char *intal2)
{
  char *num1 = remove_leading_zeros(intal1);
  char *num2 = remove_leading_zeros(intal2);
  int m = strlen(num1);
  int n = strlen(num2);
  int rlen = m + n + 1;
  char *answer = malloc(sizeof(char) * rlen);
  answer[rlen - 1] = '\0';
  for (int k = 0; k < rlen - 1; k++)
    answer[k] = '0';
  int i_n1 = 0;
  int i_n2 = 0;
  int i, j, carry, t1, t2, sum;
  for (i = m - 1; i >= 0; i--)
  {
    carry = 0;
    t1 = num1[i] - 48;
    i_n2 = 0;
    for (j = n - 1; j >= 0; j--)
    {
      t2 = num2[j] - 48;
      sum = t1 * t2 + answer[i_n1 + i_n2] - 48 + carry;
      carry = sum / 10;
      answer[i_n1 + i_n2] = sum % 10 + 48;
      i_n2++;
    }
    if (carry > 0)
      answer[i_n1 + i_n2] += carry;
    i_n1++;
  }
  char *reverse_result = malloc(sizeof(char) * rlen);
  reverse_result[rlen - 1] = '\0';
  for (i = 0; i < rlen - 1; i++)
    reverse_result[i] = answer[rlen - 2 - i];
  free(answer);
  char *holdn = remove_leading_zeros(reverse_result);
  free(reverse_result);
  return holdn;
}
/*
//Karatsuba Multiplication
char* karatsuba(char* intal1,char* intal2)
{
    int n1=strlen(intal1),n2=strlen(intal2),max=0,i=0;

    //{WIP - Need to add conversion snippet}

    if(intal1[i]-'0'<10 && intal2[i]-'0'<10)
        return ((intal1[i]-'0')*(intal2[i]-'0'))+'0';
    
    if(n1>n2)
    {
        max=n1;
        int1 = (char*)malloc(max+1);
        int2 = (char*)malloc(max+1);
        
        strcpy(int1,intal1);
        strcpy(int2+(n1-n2),intal2);
        for(i=(n1-n2);i>0;i--)
            int2[i]='0';
        
        int1[max]='\0';
        int2[max]='\0';
    }
    else if(n1<n2)
    {
        max=n2;
        int1 = (char*)malloc(max+1);
        int2 = (char*)malloc(max+1);
        
        strcpy(int2,intal2);
        strcpy(int1+(n2-n1),intal1);
        for(i=(n2-n1);i>0;i--)
            int1[i]='0';
        
        int1[max]='\0';
        int2[max]='\0';
    }
    free(intal1);
    free(intal2);
    else
        max=n2;
    int half = max/2;
    
    
    char* intal1_low = int1.substr(0,half);
    char* intal2_low = int2.substr(half,max-half);
    char* intal1_high = int1.substr(0,half);
    char* intal2_high = int2.substr(half,max-half);
    
    void* a1 = intal_create(intal1_low);
    void* a2 = intal_create(intal1_high);
    void* b1 = intal_create(intal2_low);
    void* b2 = intal_create(intal2_high);
    
    char* p0 = karatsuba(intal1_low,intal2_low);
    char* p1 = karatsuba(intal1_high,intal2_high);
    char* p2 = karatsuba(intal_add(a1,a2),intal_add(b1,b2));
    char* p3 = intal_diff(intal_create(p2),intal_add(intal_create(p0),intal_create(p1)));
    
}
*/

//This uses the long division method to compute where the quotient isn't stored and the answer
//remainder is the one that is cached from each computation.

char *intal_mod(char *intal1, char *intal2)
{
  int cmp = intal_compare(intal1, intal2);
  if (cmp == -1)
  {
    char *answer = (char *)malloc((strlen(intal1) + 1) * sizeof(char));
    strcpy(answer, intal1);
    return answer;
  }
  else if (cmp == 0)
  {
    char *answer = (char *)malloc((2) * sizeof(char));
    strcpy(answer, "0");
    return answer;
  }

  int p = strlen(intal2);
  int stop = strlen(intal1);
  char *current = (char *)malloc((strlen(intal2) + 2) * sizeof(char));
  for (int z = 0; z < p; ++z)
    current[z] = intal1[z];
  current[p] = '\0';

  do
  {
    if (intal_compare(current, intal2) == -1 && p < stop)
    {
      int len = strlen(current);
      if (strcmp(current, "0") == 0)
        len = 0;
      current[len] = intal1[p];
      current[len + 1] = '\0';
      ++p;
    }

    if (intal_compare(current, intal2) != -1)
    {
      char *answer = intal_diff(current, intal2);
      while (intal_compare(answer, intal2) > -1)
      {
        char *tmp = answer;
        answer = intal_diff(answer, intal2);
        free(tmp);
      }

      strcpy(current, answer);
      free(answer);
    }
  } while (p < stop);

  current = realloc(current, (strlen(current) + 1) * sizeof(char));
  current[strlen(current) + 1] = '\0';
  return current;
}

//Call INTAL_MULTIPLY multiple times till n and keep updating it in answer and return answer
char *intal_pow(char *intal1, unsigned int n)
{
  if (n == 0)
  {
    char *t;
    t = malloc(sizeof(char) * 2);
    t[0] = '1';
    t[1] = '\0';
    return t;
  }

  char *holdn = (char *)malloc((strlen(intal1) + 1) * sizeof(char));
  strcpy(holdn, intal1);

  for (int z = 1; z < n / 2; z++)
  {
    char *arr = holdn;
    holdn = intal_multiply(holdn, intal1);
    free(arr);
  }
  if (n % 2)
  {
    char *r = (char *)malloc((strlen(intal1) + 1) * sizeof(char));
    r = intal_multiply(holdn, intal1);
    holdn = intal_multiply(r, holdn);
  }
  else
  {
    holdn = intal_multiply(holdn, holdn);
  }

  return holdn;
}

//This function uses the iterative Euclid's algorithm to calculate GCD of two numbers
char *intal_gcd(char *intal1, char *intal2)
{
  char *a = (char *)malloc((strlen(intal1) + 1) * sizeof(char));
  strcpy(a, intal1);
  char *b = (char *)malloc((strlen(intal2) + 1) * sizeof(char));
  strcpy(b, intal2);

  int flag = strcmp(a, "0");
  while (flag != 0)
  {
    char *tmp = b;
    b = a;
    a = intal_mod(tmp, a);

    free(tmp);
    flag = strcmp(a, "0");
  }

  free(a);
  return b;
}

//Traverse starting from 0,1 and add the last two , make the answer as last and last as second last, using intal_add.
char *intal_fibonacci(unsigned int n)
{
  char *a = (char *)malloc(2 * sizeof(char));
  a[0] = '0';
  a[1] = '\0';

  if (n == 0)
    return a;

  char *b = (char *)malloc(2 * sizeof(char));
  b[0] = '1';
  b[1] = '\0';

  if (n == 1)
    return b;

  for (int z = 1; z < n; ++z)
  {
    char *c = intal_add(a, b);
    free(a);
    a = b;
    b = c;
  }

  free(a);
  return b;
}

//This function calculates factorial.
char *intal_factorial(unsigned int n)
{
  char *start = (char *)malloc(2 * sizeof(char));
  start[0] = '1';
  start[1] = '\0';

  if (n < 2)
    return start;

  char *answer = (char *)malloc(2 * sizeof(char));
  answer[0] = '1';
  answer[1] = '\0';

  char *step = (char *)malloc(2 * sizeof(char));
  step[0] = '1';
  step[1] = '\0';

  for (int z = 1; z < n; ++z)
  {
    char *tmp = start;
    start = intal_add(start, step);
    free(tmp);

    tmp = answer;
    answer = intal_multiply(start, answer);
    free(tmp);
  }

  free(start);
  free(step);

  return answer;
}

//Use DP to calculate the binomial coefficient of (n,k) using bottom up approach.
char *intal_bincoeff(unsigned int n, unsigned int k)
{
  if(k==0 || n==k)
    {
          char* place = malloc(sizeof(char) * 2);
          place[0] = '1';
          place[1] = '\0';
          return place;
      }
      if(k>= n-k)
      {
          k = n-k;
      }
  
      char **val = (char**) malloc((k+1) * sizeof(char*));
      for(int y = 0; y <= k; y++)
      {
          val[y] = (char*) malloc(1001 * sizeof(char));
          strcpy(val[y], "1");
      }
      
      for(int z = 1; z <= (n-k); z++)
      {
          for(int i = 1; i <= k; i++)
          {
              char* temp = intal_add(val[i], val[i-1]);
              free(val[i]);
              val[i] = temp;
          }
      }
      char *holdn = remove_leading_zeros(val[k]);
      for(int i = 0; i <= k; i++)
      {
          free(val[i]);
      }
      free(val);

      return holdn;
}
//Linear traversing, finding the max intal in a given array.
int intal_max(char **arr, int n)
{
  int p = 0;
  for (int z = 1; z < n; ++z)
  {
    if (intal_compare(arr[p], arr[z]) == -1)
      p = z;
  }

  return p;
}

//Linear traversing, finding the min intal in a given array.
int intal_min(char **arr, int n)
{
  int p = 0;
  for (int z = 1; z < n; ++z)
  {
    if (intal_compare(arr[p], arr[z]) == 1)
      p = z;
  }

  return p;
}

//Does a Linear Search to find the given key intal in a given array.
int intal_search(char **arr, int n, const char *key)
{
  int p = -1;
  for (int z = 0; z < n; ++z)
  {
    if (intal_compare(arr[z], (char *)key) == 0)
    {
      p = z;
      break;
    }
  }

  return p;
}

// The following binary_search code is lifted, without permission, from programiz.com
// Changes were made to fit this code snippet in my implementation of intal
//Does a Binary search to find the given key intal in a given array.
int intal_binsearch(char **arr, int n, const char *key)
{
  int start = 0;
  int end = n - 1;

  while (start <= end)
  {
    int mid = (start + end) / 2;
    int cmp = intal_compare(arr[mid], (char *)key);
    if (cmp == 0)
    {
      return mid;
    }
    else if (cmp > 0)
    {
      end = mid - 1;
    }
    else
    {
      start = mid + 1;
    }
  }

  return -1;
}

// The following partition & quicksort code is lifted, without permission, from hackr.io
// Changes were made to fit this code snippet in my implementation of intal
//Partition index function for quicksort
int partition(char **arr, int l, int h)
{
  int z = l - 1;
  char *pivot = arr[h];

  for (int j = l; j < h; ++j)
  {
    if (intal_compare(arr[j], pivot) < 0)
    {
      ++z;
      char *tmp = arr[z];
      arr[z] = arr[j];
      arr[j] = tmp;
    }
  }

  char *tmp = arr[z + 1];
  arr[z + 1] = arr[h];
  arr[h] = tmp;

  return (z + 1);
}

//Quick sort of a part
void quicksort(char **arr, int l, int h)
{
  if (l < h)
  {
    int pi = partition(arr, l, h);
    quicksort(arr, l, pi - 1);
    quicksort(arr, pi + 1, h);
  }
}

//This function uses QuickSort Algorithm to sort the array
void intal_sort(char **arr, int n)
{
  quicksort(arr, 0, n - 1);
}

//Uses a O(n) DP table to implement coin row problem
char *coin_row_problem(char **arr, int n)
{
  char **handle = (char **)malloc((n + 1) * sizeof(char *));
  handle[0] = (char *)malloc(2 * sizeof(char));
  handle[0][0] = '0';
  handle[0][1] = '\0';

  for (int z = 0; z < n; ++z)
  {
    handle[z + 1] = (char *)malloc((strlen(arr[z]) + 1) * sizeof(char));
    strcpy(handle[z + 1], arr[z]);
  }

  for (int z = 2; z < n + 1; ++z)
  {
    char *val = intal_add(arr[z - 1], handle[z - 2]);
    if (intal_compare(val, handle[z - 1]) > 0)
    {
      char *tmp = handle[z];
      handle[z] = val;
      free(tmp);
    }
    else
    {
      free(handle[z]);
      handle[z] = (char *)malloc((strlen(handle[z - 1]) + 1) * sizeof(char));
      strcpy(handle[z], handle[z - 1]);
      free(val);
    }
  }

  char *answer = (char *)malloc((strlen(handle[n]) + 1) * sizeof(char));
  strcpy(answer, handle[n]);

  for (int z = 0; z < n + 1; ++z)
  {
    free(handle[z]);
  }
  free(handle);

  return answer;
}
