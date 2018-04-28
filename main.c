#include "load_vitals.h"
#include "cumul_dis.h"
#include "prob_dis.h"

// #include "binary_search.h"

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

int decode(int ciphertext) {
    int *inverse_table_length;
    int **inverse_table = get_inverse_table(inverse_table_length);
    double seed_location = (double)ciphertext/seed_space;
    double prev_value;
    int prev_msg;
    binary_search(&inverse_table,0,*inverse_table_length,seed_location,&prev_value,&prev_msg);

    int next_msg = next_message(prev_msg);
    double next_value = cumulative_distribution(next_msg);

    if(next_msg==prev_msg) // at max message. checks to see if next==prev
        return prev_msg;
    // begin linear scan to find which range seed s falls in
    while(seed_location >= next_value) {
        //update prev and next
        prev_value = next_value;
        prev_msg = next_msg;
        next_msg = next_message(prev_msg);
        next_value = cumulative_distribution(next_msg);
    }

    return prev_msg;
}

void binary_search(int ***inverse_table, int start, int end, double seed_location, double *prev_value, int *prev_msg) {

}

int main() {
  initialize();
  printf("Ciphertext: %llu\n",encode("4117700001669792"));

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
