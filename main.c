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
    double start0 = cumulative_distribution(plaintext) * seed_space;
    unsigned long long int end = (int)(start0 + probability_distribution(plaintext) * seed_space);
    start = (unsigned long long int)(start0);
    return (unsigned long long int)(rand()*(end-start) + start);
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
    printf("%0.15f %0.15f\n",seed_location,next_value);
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

int main() {
  initialize();
  char *credit_card_example = "4117700001669792";
  unsigned long long int secret_key = 2048101736616812280;
  unsigned long long int guess_key =  3496328831800304765;

  unsigned long long int seed = encode(credit_card_example);
  unsigned long long int ciphertext = secret_key ^ seed;
  printf("Seed: %llu\n",seed);
  printf("Ciphertext: %llu\n",ciphertext);

  unsigned long long int decipher_seed = guess_key ^ ciphertext;

  unsigned long long int plaintext = decode(decipher_seed);
  printf("Plaintext: %llu\n",plaintext);

  return 0;
}

/*
  Alright!
  Kick back, relax, and grab yourself a beverage
  Or hop if you want to just bounce to my leverage
  The evidence, as they say is in the pudding
  So show me the money, like my name is Cuba Gooding
  It ain't fools gold, its gold, fool
  Can't stop won't stop keeping it old school, so cool
  Big Spoon is my nom de plume
  I keep it popping like a needle in a new balloon
  And if you don't know by now then you need more schoolin'
  Let loose kid, get stupid
  Got more flavor than I know what to do with
  So get your hands up if you're part of the movement
  And bump to the bump to the baseline
  I made it rock before your phone had Facetime
  Yessiree, you know my steeze
  I stay fresh to death like I'm drowning in Febreze
*/
