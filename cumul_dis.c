#include "load_vitals.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// float *get_prefix_cumul() {
//   float cumul_prob = 0;
//   //float prefix_cumul[ORDERED_PREFIX_TABLE_LENGTH];
//   int i; for(i=0; i<ORDERED_PREFIX_TABLE_LENGTH; i++) {
//     prefix_cumul[i] = cumul_prob;
//     cumul_prob += ((float)ordered_prefixes[i][2]) / total_prob;
//   }
//   return prefix_cumul;
// }

int searchPrefixes(int prefix) {

  int o; for(o=0; o<PREFIX_TABLE_LENGTH; o++)
    if(prefixes[o][2] == prefix)
      return o;
  return -1;
}

void get_randomDigs(int *prefix,int start,char *m,int *randomDigs) {
  int size = 15 - 1 - start;
  int i,o=0; for(i=start; i<size; i++) {
    *randomDigs += m[i];
    o++;
  }
}

// Assume that ordered prefixes comes from a file
float cumulative_distribution(char *plaintext) {
  int i;

  // Getting get_prefix_cumul
  float cumul_prob = 0;
  float prefix_cumul[ORDERED_PREFIX_TABLE_LENGTH];
  for(i=0; i<ORDERED_PREFIX_TABLE_LENGTH; i++) {
    prefix_cumul[i] = cumul_prob;
    cumul_prob += ((float)ordered_prefixes[i][3]) / total_prob;
  }


  float totalCumul=0;
  int prefix[6];

  for(i=0; i<6; i++) {
    prefix[i] = (int)plaintext[i];
    int prefix_i,prefixInt = 0;
    int o; for(o=0; o<i; o++) prefixInt += prefix[o];
    if((prefix_i = searchPrefixes(prefixInt)) > 0) {
      // last digit is the check dig
      int randomDigs;
        get_randomDigs(prefix,6-prefixes[prefix_i][1],plaintext,&randomDigs);
      int numRandomDigs = prefixes[prefix_i][2] - 7;
      int intPrefixCumul = prefix_cumul[prefix_i];
      totalCumul = intPrefixCumul + (float)(randomDigs)*pow(10,-numRandomDigs) / total_prob;
      return totalCumul;
    }
  }
  printf("Invalid credit card\n");
  return -1;
}

int main() {
  printf("%s\n",cumulative_distribution("117700001669792"));
  return 0;
}
