#include "itoa.h"
#include "load_vitals.h"
#include "cumul_dis.h"
#include "prob_dis.h"
#include "binary_search.h"

#define seed_space_length 64
const unsigned long long int seed_space = pow(2, seed_space_length)-1;

unsigned long long int encode(char *plaintext) {
    time_t t;
    srand((unsigned) time(&t));

    unsigned long long int start;
    unsigned long long int start0 = cumulative_distribution(plaintext) * seed_space;
    // printf("%llu\n",start0);
    unsigned long long int end = (start0 + probability_distribution(plaintext)*seed_space) - 1;
    start = (unsigned long long int)(start0);
    double r = 0.595040256028; // rand();
    return(unsigned long long int)((double)(r*(end-start) + start));
}

unsigned long long int decode(unsigned long long int ciphertext) {
    double seed_location; //= ciphertext/seed_space;
    //printf("SL: %llu %llu\n",ciphertext,seed_space);
    //double seed_location = 0.0001590605715830;
    printf("Please compute %llu / %llu and Enter:\n",ciphertext,seed_space);
    scanf("%lf",&seed_location);

    int i = binary_search(0,INVERSE_TABLE_LENGTH,seed_location);
    double prev_value = inverse_table[i][0];
    unsigned long long int prev_msg = inverse_table[i][1];
    // printf("prev_value:%0.15f prev_msg:%llu\n",prev_value,prev_msg);
    unsigned long long int next_msg = next_message(prev_msg);

    char next_msg_str[30];
    ITOA(next_msg,next_msg_str);
    double next_value = cumulative_distribution(next_msg_str);

    if(next_msg==prev_msg) // at max message. checks to see if next==prev
        return prev_msg;
    // begin linear scan to find which range seed s falls in
    // printf("%0.15f %0.15f\n",seed_location,next_value);
    while(seed_location >= next_value) {
        //update prev and next
        prev_value = next_value;
        prev_msg = next_msg;
        next_msg = next_message(prev_msg);
        ITOA(next_msg,next_msg_str);
        next_value = cumulative_distribution(next_msg_str);
    }

    return prev_msg;
}

int main() { // http://prntscr.com/jbgo5o
  initialize();
  char *credit_card_example = "4117700001669792";
  unsigned long long int real_key = 2048101736616812280;
  unsigned long long int guess_key =  3496328831800304765;
  printf("Credit Card: %s\n",credit_card_example);
  printf("Real Key: %llu\n",real_key);
  printf("Guess Key: %llu\n",guess_key);

  unsigned long long int seed = encode(credit_card_example);
  unsigned long long int ciphertext = real_key ^ seed;
  printf("Real Seed: %llu\n",seed);
  printf("Ciphertext: %llu\n",ciphertext);

  unsigned long long int fake_seed = guess_key ^ ciphertext;
  printf("Guessed Seed: %llu\n",fake_seed);

  printf("\n");

  unsigned long long int fakeplaintext = decode(fake_seed);
  printf("Fake Plaintext: %llu\n",fakeplaintext);
  unsigned long long int plaintext = decode(seed);
  printf("Real Plaintext: %llu\n",plaintext);

  // unsigned long long int decipher_seed = guess_key ^ ciphertext;
  //
  // unsigned long long int plaintext = decode(decipher_seed);
  // printf("Plaintext: %llu\n",plaintext);

  return 0;
}
