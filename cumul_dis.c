#include <load_vitals.h>
#include <math.h>

char *convertNumberIntoArray(unsigned int number) {
  int length = (int)floor(log10((float)number)) + 1;
  char * arr = new char[length];
  int i = 0;
  do {
      arr[i] = number % 10;
      number /= 10;
      i++;
  } while (number != 0);
  return arr;
}

*float get_prefix_cumul(int total_prob) {
  float cumul_prob = 0;
  int prefix_cumul[ORDERED_PREFIX_TABLE_LENGTH];
  int i;
  for(i=0; i<ORDERED_PREFIX_TABLE_LENGTH; i++) {
    prefix_cumul[i] = cumul_prob;
    cumul_prob += ((float)ordered_prefixes[i][2]) / total_prob
  }
  return prefix_cumul;
}

// Assume that ordered prefixes comes from a file
double cumulative_distribution(int plaintext) {
  char* plaintextArray = convertNumberIntoArray(plaintext);
  int *prefix_order;
  int *prefix_cumul;
  float totalCumul=0;
  int prefix[6];

  int i;
  for(i=0; i<6; i++) {
    prefix[i] = plaintextArray[i];
    // WHAT IS prefixStr???
  }
}

int main() {
  printf("%d\n",cumulative_distribution(117700001669792));
}
