#include <stdio.h>
#include <string.h>
#include <math.h>
#define seed_space_length 64

double cumulative_distribution(char[] m);
double probability_distribution(char[] m);
int[][] get_inverse_table(char *inverse_table_length);
void binary_search(int[][] *inverse_table, int start, int end, double seed_location, double *prev_value, int *prev_message);
int[] next_message(int[] prev_msg);

seed_space = pow(2, seed_space_length) - 1;

int encode(char[] m) {
    double start0 = cumulative_distribution(m) * seed_space;
    int end = (int)(start + probability_distribution(m) * speed_space);
    int start = (int)(start);
    return randomSeed(start,end);
}

int decode(char[] c) {
    int inverse_table_length;
    int[][] inverse_table = get_inverse_table(&inverse_table_length);
    double seed_location = c/seed_space;
    double prev_value;
    int prev_msg;
    binary_search(&inverse_table,0,inverse_table_length,seed_location,&prev_value,&prev_msg);

    int next_msg = next_message(prev_msg);
    double next_value = cumulative_distribution(next_msg);

    if(!strcmp(next_msg,prev_msg)) // at max message. checks to see if next==prev
        return prev_msg;
    // begin linear scan to find which range seed s falls in
    while(seed_loc >= next_value) {
        //update prev and next
        prev_value = next_value;
        prev_msg = next_msg;
        next_msg = next_message(prev_msg);
        next_value = cumulative_distribution(next_msg);
    }

    return prev_msg;
}

/*
    next_msg = pfxns.next_message(prev_msg)
    next_value = pfxns.cumul_distr(next_msg)
    if next_msg == prev_msg: # at max message
        return prev_msg
    # begin linear scan to find which range seed s falls in
    while seed_loc >= next_value:
        # update prev and next
        (prev_value, prev_msg) = (next_value, next_msg)
        next_msg = pfxns.next_message(prev_msg)
        next_value = pfxns.cumul_distr(next_msg)

    return prev_msg
*/