#include "load_vitals2.h"

int searchPrefixes(int prefix) {
  int o; for(o=0; o<PREFIX_TABLE_LENGTH; o++)
    if(prefixes[o][0] == prefix)
      return o;
  return -1;
}

void get_randomDigs(int *prefix,int start,char *m,unsigned long long int *randomDigs) {
  int size = 15; // Credit card # size -1
  int i,o=0; for(i=start; i<size; i++) {
    *randomDigs += ( (m[i]-'0') * pow(10,size-i-1) );
    o++;
  }
}

// Assume that ordered prefixes comes from a file
double cumulative_distribution(char *plaintext) {
  initialize();
  int i;

  // Getting prefix_cumul
  double cumul_prob = 0;
  double prefix_cumul[PREFIX_TABLE_LENGTH];
  for(i=0; i<PREFIX_TABLE_LENGTH; i++) {
    prefix_cumul[i] = cumul_prob;
    cumul_prob += (double)prefixes[i][3] / total_prob;
    // printf("%0.15g\n",cumul_prob);
  }

  int prefix[6];

  for(i=0; i<6; i++) {
    prefix[i] = plaintext[i] - '0'; // converts char to int
    // printf("%i %c\n",prefix[i],plaintext[i]);
    int prefix_i,prefixInt = 0;
    int o; for(o=0; o<=i; o++) {
      prefixInt = prefixInt*10;
      prefixInt += prefix[o];
    }
    // printf("%i\n",prefixInt);
    if((prefix_i = searchPrefixes(prefixInt)) > 0) {
      // last digit is the check dig
      unsigned long long int randomDigs;
        get_randomDigs(prefix,6-prefixes[prefix_i][1],plaintext,&randomDigs);
      // printf("RandomDigs: %llu\n",randomDigs);
      int numRandomDigs = prefixes[prefix_i][2] - 7;
      // printf("numRandomDigs: %i\n",numRandomDigs);
      double prefixCumul = prefix_cumul[prefix_i];
      // printf("PrefixCumul: %g %i\n",prefixCumul,prefixInt);
      double totalCumul = prefixCumul + (double)(randomDigs)*pow(10,-numRandomDigs) / total_prob;
      // printf("TotalCumul: %g\n",prefixCumul);
      return totalCumul;
    }
  }
  printf("Invalid credit card\n");
  return -1;
}

int main() {
  printf("%g\n",cumulative_distribution("4117700001669792"));
  return 0;
}
