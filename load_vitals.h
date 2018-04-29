#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#define PREFIX_TABLE_LENGTH 2081
#define INVERSE_TABLE_LENGTH 150887

int prefixes[PREFIX_TABLE_LENGTH][5];
int ordered_prefixes[PREFIX_TABLE_LENGTH][4];
double inverse_table[INVERSE_TABLE_LENGTH][2];
int total_prob;

int searchPrefixes(int prefix) {
  int o; for(o=0; o<PREFIX_TABLE_LENGTH; o++)
    if(prefixes[o][0] == prefix)
      return o;
  return -1;
}

void get_randomDigs(int *prefix,int start,char *m,int64_t *randomDigs) {
  int i=0,o=0,size=0;

  // The following should be size=cardlength-1
  while(m[size+1] != '\0') {
    size++;
  }

  for(i=start; i<size; i++) {
    *randomDigs += ( (m[i]-'0') * pow(10,size-i-1) );
    o++;
  }
}

unsigned long long int next_message(unsigned long long int prev_msg) {
  //printf("Intitial: %llu\n",prev_msg);
  //sleep(1);
  int lastDigit = prev_msg%10;
  prev_msg = prev_msg - lastDigit;
  prev_msg = prev_msg / 10;
  prev_msg++;

  char strBaseNumber[30];
  ITOA(prev_msg,strBaseNumber);
  // printf("\t%s\n",strBaseNumber);

  int sum = 0;
  int i=0; while(strBaseNumber[i] != '\0') {
    sum += (strBaseNumber[i] - '0');
    i++;
  }
  int last = (9 * sum) % 10;

  unsigned long long int next_msg = (prev_msg*10) + last;
  //printf("Final: %llu\n",next_msg);
  return next_msg;
}

void load_prefixes(int t[PREFIX_TABLE_LENGTH][5]) {
  char buf[6]; // Longest we're gonna need is for a prefix which is of length 6
  FILE *f;
  f = fopen("bin.txt","r");
  if(f) {
    char c;
    int numElementsOnLine=0,lineNumber=0,objectLength=0;
    while( !feof(f) ) {
      c = fgetc(f);
      if( c==' ' || c=='\n') {
        int value=0;
        int i; for(i=0; i<objectLength; i++)
          value += ( (buf[i]-'0') * pow(10,objectLength-i-1) );
        t[lineNumber][numElementsOnLine] = value;
        objectLength=0;

        numElementsOnLine++;
        if(numElementsOnLine == 4) {
          // printf("%i %i %i %i\n",t[lineNumber][0],t[lineNumber][1],t[lineNumber][2],t[lineNumber][3]);
          // sleep(1);
          numElementsOnLine=0;
          lineNumber++;
        }
      }
      else if(c!='*'){ // Otherwise add it to our buffer if it's not a *
        buf[objectLength] = c;
        objectLength++;
      }
    }
  }
  else {
    printf("Could not open file");
  }
  printf("Successfully loaded prefixes into memory\n");
}

// This function has to go through the prefixes and sort them according to VALUES
void load_ordered_prefixes(int t[PREFIX_TABLE_LENGTH][4]) {
  int i,o,p,n=PREFIX_TABLE_LENGTH;
  for(i=0; i<PREFIX_TABLE_LENGTH; i++) {
    for(o=0; o<4; o++) {
      t[i][o] = prefixes[i][o];
    }
    // sleep(1);
    // printf("%i %i %i %i\n",t[i][0],t[i][1],t[i][2],t[i][3]);
  }

  // Good old bubble sort
  int swaps;
  for (i=0; i < (n-1); i++) {
    swaps=0;
    for (o=0; o < (n-i-1); o++) {
      int greater=0,normal=1;
      if( (t[o][0]<100000) || (t[o+1][0]<100000) ) {
        normal = 0;
        if(t[o+1][0]>100000) {
          greater = 1;
        }
        else if(t[o][0]<100000) { // If they're both <100,000
          double val1=t[o][0], val2=t[o+1][0];
          int z; for(z=0; z<6 && val1>0 && val2>0; z++) {
            val1 = floor(val1/10);
            val2 = floor(val2/10);
          }

          if(val1 < val2)
            greater = 1;
          else if(val1 == val2)
            normal = 1;
        }
      }

      if(normal) {
        if(t[o][1] > t[o+1][1])
          greater = 1;
        else if(t[o][2] > t[o+1][2])
          greater = 1;
        else if(t[o][3] > t[o+1][3])
          greater = 1;
      }

      if (greater) {
        swaps=1;
        int p,temp[4];
        for(p=0; p<4; p++)
          temp[p] = t[o][p];

        for(p=0; p<4; p++)
          t[o][p] = t[o+1][p];

        for(p=0; p<4; p++)
          t[o+1][p] = temp[p];

        // prefixes[o][4] = o+1;
        // prefixes[o+1][4] = o;
      }
    }
    if(!swaps) break;
  }

  printf("Successfully loaded ordered prefixes into memory\n");
}

void load_inverse_table(double t[INVERSE_TABLE_LENGTH][2]) {
  char buf[30];
  FILE *f;
  f = fopen("inverse_table.txt","r");
  if(f) {
    char c;
    int numElementsOnLine=0,lineNumber=0,objectLength=0;
    while( !feof(f) ) {
      c = fgetc(f);
      if( c==' ' || c=='\n') {
        double value=0;
        int isDecimal=0; if(buf[1]=='.') isDecimal=2;
        int i; for(i=isDecimal; i<objectLength; i++) {
          if(isDecimal) {
            value += ( (buf[i]-'0') * pow(10,-i+1) );
            // if(lineNumber==75443) {
            //   printf("%0.15lf %c\n",value,buf[i]);
            //   sleep(1);
            // }
          }
          else {
            value += ( (buf[i]-'0') * pow(10,objectLength-i-1) );
          }
        }
        // if(lineNumber==75443) {
        //   printf("%0.15lf %s\n",value,buf);
        // }

        t[lineNumber][numElementsOnLine] = value;
        objectLength=0;

        numElementsOnLine++;
        if(numElementsOnLine == 2) {
          // printf("%0.18g %g\n",t[lineNumber][0],t[lineNumber][1]);
          // sleep(1);
          numElementsOnLine=0;
          lineNumber++;
        }
      }
      else { // Otherwise add it to our buffer
        buf[objectLength] = c;
        objectLength++;
      }
    }
  }
  else {
    printf("Could not open file");
  }
}

int getTotalProbability() {
  int sum = 0;
  int i; for(i=0; i<PREFIX_TABLE_LENGTH; i++) {
    sum += prefixes[i][3];
  }
  return sum;
}

void initialize() {
  load_prefixes(prefixes);
  load_ordered_prefixes(ordered_prefixes);
  load_inverse_table(inverse_table);
  total_prob = getTotalProbability();
}

// For testing purposes only. I'd recommend only trying to load one function at a time.
// int main() {
//   printf("RUNNING:\n");
//   load_prefixes(prefixes);
//   load_ordered_prefixes(ordered_prefixes);
// }
