/*
  Author: Tim Sternberg
  Class:  CS492
  Date:   4/30/2018

  Given a credit card number and a key to encrypt it with, honey encryption
  will encrypt it in such a way that even if an attacker attempting to brute
  force the key guesses an incorrect key, they will still get a perfectly
  possible, but false credit card as a result.

  An example run: http://prntscr.com/jbgo5o

  This implementation was created with the help of the following python code:
  https://github.com/danielzuot/honeyencryption
  and the following research paper:
  https://www.hindawi.com/journals/scn/2017/6760532/

  Note: In the decryption method in DTE.h, due to the size of two numbers
  being divided, overflow occured. As a result, it was necessary (due to time
  constraints) to simply prompt the user with the calculation and input the result.
*/

#include "itoa.h"           // We needed to use our own custom itoa for long ints
#include "load_vitals.h"    // Initializes prefixes/inverse_table, has getter methods
#include "cumul_dis.h"      // Cumulative_distribution function
#include "prob_dis.h"       // Probability distribution function
#include "binary_search.h"  // Searches through the inverse table for a value
#include "DTE.h"            // Contains the encode and decode methods

int main() {
  initialize();                                               // Loads prefixes and inverse_table from .txt files
  char *credit_card_example = "4117700001669792";
  unsigned long long int real_key = 2048101736616812280;      // The actual key used to get the ciphertext
  unsigned long long int guess_key =  3496328831800304765;    // An example of an attacker guessing a key
  printf("Credit Card: %s\n",credit_card_example);
  printf("Real Key: %llu\n",real_key);
  printf("Guess Key: %llu\n",guess_key);

  unsigned long long int seed = encode(credit_card_example);  // The SECRET seed obtained from encoding it
  unsigned long long int ciphertext = real_key ^ seed;        // Ciphertext as a result of XOR'ing seed with real key
  printf("Real Seed: %llu\n",seed);
  printf("Ciphertext: %llu\n",ciphertext);

  unsigned long long int fake_seed = guess_key ^ ciphertext;  // The attacker's attempt to getting the seed with their key
  printf("Guessed Seed: %llu\n",fake_seed);

  printf("\n");

  unsigned long long int fakeplaintext = decode(fake_seed);   // Decrypting the seed to get legibile plaintext
  printf("Fake Plaintext: %llu\n",fakeplaintext);
  unsigned long long int plaintext = decode(seed);            // Getting the actual plaintext from the real seed
  printf("Real Plaintext: %llu\n",plaintext);

  return 0;
}
