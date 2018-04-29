#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#define PREFIX_TABLE_LENGTH 2081
#define INVERSE_TABLE_LENGTH 150887

int prefixes[PREFIX_TABLE_LENGTH][5]; // The last element is the prefix's location in cumul_dist
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
  *randomDigs=0; // making sure it's = 0 first
  int i=0,o=0,size=0;

  // The following should be size=cardlength-1
  while(m[size+1] != '\0')
    size++;

  for(i=start; i<size; i++) {
    *randomDigs += ( (m[i]-'0') * pow(10,size-i-1) );
    o++;
  }
}

unsigned long long int next_message(unsigned long long int prev_msg) {
  int lastDigit = prev_msg%10;
  prev_msg = prev_msg - lastDigit;
  prev_msg = prev_msg / 10;
  prev_msg++;

  char strBaseNumber[30];
  ITOA(prev_msg,strBaseNumber);

  int sum = 0;
  int i=0; while(strBaseNumber[i] != '\0') {
    sum += (strBaseNumber[i] - '0');
    i++;
  }
  int last = (9 * sum) % 10;

  unsigned long long int next_msg = (prev_msg*10) + last;
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
      if( c==' ' || c=='\n') { // Then we've reached a new value, and need to save the previous one
        int value=0;
        int i; for(i=0; i<objectLength; i++)
          value += ( (buf[i]-'0') * pow(10,objectLength-i-1) );
        t[lineNumber][numElementsOnLine] = value; // numElementsOnLine tells us where to put the value
        objectLength=0;

        numElementsOnLine++; // So we know when there's a new line and we can update numElementsOnLine to 0
        if(numElementsOnLine == 4) {
          numElementsOnLine=0;
          lineNumber++;
        }
      }
      else if(c!='*'){ // Otherwise add it to our buffer if it's not a *
        buf[objectLength] = c; // Add the character to the buffer
        objectLength++; // Add to the length of the object
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
  // Initialize ordered_prefixes to be what prefixes currently is
  int i,o,p,n=PREFIX_TABLE_LENGTH;
  for(i=0; i<PREFIX_TABLE_LENGTH; i++) {
    for(o=0; o<4; o++) {
      t[i][o] = prefixes[i][o];
    }
  }

  // Good old bubble sort
  int swaps;
  for (i=0; i < (n-1); i++) {
    swaps=0;
    for (o=0; o < (n-i-1); o++) {
      int greater=0,normal=1;
      if( (t[o][0]<100000) || (t[o+1][0]<100000) ) { // If the prefix is shorter than 6 digits
        normal = 0; // Then it's not a normal case
        if(t[o+1][0]>100000) { // We want to put the prefixes with <6 digits LAST. So prioritize the one with greatest amount of digits
          greater = 1;
        }
        else if(t[o][0]<100000) { // If they're both <100,000, we gotta see which one has more digits
          double val1=t[o][0], val2=t[o+1][0];
          int z; for(z=0; z<6 && val1>0 && val2>0; z++) {
            val1 = floor(val1/10);
            val2 = floor(val2/10);
          }

          if(val1 < val2)
            greater = 1;
          else if(val1 == val2) // Then they're both of the same digit length <6, so we compare as normal
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

      if (greater) { // If we need a swap then do so
        swaps=1;
        int p,temp[4];
        for(p=0; p<4; p++)
          temp[p] = t[o][p];

        for(p=0; p<4; p++)
          t[o][p] = t[o+1][p];

        for(p=0; p<4; p++)
          t[o+1][p] = temp[p];
      }
    }
    if(!swaps) break;
  }

  printf("Successfully loaded ordered prefixes into memory\n");
}

void load_inverse_table(double t[INVERSE_TABLE_LENGTH][2]) { // works similar to how we loaded in prefixes
  char buf[30]; // max buffer length we'll need, just to be safe
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
          }
          else {
            value += ( (buf[i]-'0') * pow(10,objectLength-i-1) );
          }
        }

        t[lineNumber][numElementsOnLine] = value;
        objectLength=0;

        numElementsOnLine++;
        if(numElementsOnLine == 2) {
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

int getTotalProbability() { // Gets the total probability
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
