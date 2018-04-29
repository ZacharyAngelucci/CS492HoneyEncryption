double cumulative_distribution(char *plaintext) {
  int i;

  // Getting prefix_cumul
  double cumul_prob = 0;
  double prefix_cumul[PREFIX_TABLE_LENGTH];
  // printf("%i\n",total_prob);
  for(i=0; i<PREFIX_TABLE_LENGTH; i++) {
    // printf("%0.15g\t%i\n",cumul_prob,ordered_prefixes[i][3]);
    // sleep(1);
    prefix_cumul[i] = cumul_prob;
    int found=0; while(!found) {
      int o; for(o=0; o<PREFIX_TABLE_LENGTH && !found; o++) {
        if(prefixes[o][0] == ordered_prefixes[i][0]) {
          prefixes[o][4] = i;
          found = 1;
        }
      }
    }
    cumul_prob += (double)ordered_prefixes[i][3] / (double)total_prob;
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
      int64_t randomDigs=0;
        get_randomDigs(prefix,6-prefixes[prefix_i][1],plaintext,&randomDigs);
      // printf("RandomDigs: %" PRId64 "\n",randomDigs);
      int numRandomDigs = prefixes[prefix_i][2] - 7;
      // printf("numRandomDigs: %i\n",numRandomDigs);
      double prefixCumul = prefix_cumul[ prefixes[prefix_i][4] ];
      // printf("PrefixCumul: %0.12f\n",prefixCumul);
      double totalCumul = prefixCumul + (double)(randomDigs)*pow(10,-numRandomDigs) / total_prob;
      // printf("TotalCumul: %0.12f\n",totalCumul);
      return totalCumul;
    }
  }
  printf("Invalid credit card\n");
  return -1;
}

// int main() {
//   printf("%g\n",cumulative_distribution("4117700001669792"));
//   return 0;
// }
