double cumulative_distribution(char *plaintext) {
  int i;

  // Getting prefix_cumul
  double cumul_prob = 0;
  double prefix_cumul[PREFIX_TABLE_LENGTH];
  for(i=0; i<PREFIX_TABLE_LENGTH; i++) {
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
  }

  int prefix[6]; // Our prefix in char[] form

  for(i=0; i<6; i++) {
    prefix[i] = plaintext[i] - '0'; // converts the char to int by subtracting the int value of '0'
    int prefix_i; // The index of the prefix in the prefixes array
    int prefixInt = 0; // Simply the prefix in number form
    int o; for(o=0; o<=i; o++) {
      prefixInt = prefixInt*10;
      prefixInt += prefix[o];
    }

    if((prefix_i = searchPrefixes(prefixInt)) > 0) { // Setting prefix_i while searching through prefixes for our prefix
      // Note: last digit is the check dig
      int64_t randomDigs=0;
        // Essentially, we're saying randomDigs = the plaintext, excluding digits 0-5, and the last digit
        get_randomDigs(prefix,6-prefixes[prefix_i][1],plaintext,&randomDigs);
      int numRandomDigs = prefixes[prefix_i][2] - 7; // Length of our credit card - 7
      // Get the prefix_cumul of our prefix. The index of which is stored in the fourth element of the prefix in prefixes[]
      double prefixCumul = prefix_cumul[ prefixes[prefix_i][4] ];
      double totalCumul = prefixCumul + (double)(randomDigs)*pow(10,-numRandomDigs) / total_prob;
      return totalCumul; // The result of running cumulative_distribution
    }
  }
  printf("Invalid credit card\n");
  return -1;
}
