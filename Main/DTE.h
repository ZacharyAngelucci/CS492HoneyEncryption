#define seed_space_length 64
const unsigned long long int seed_space = pow(2, seed_space_length)-1;

unsigned long long int encode(char *plaintext) {
    time_t t;
    srand((unsigned) time(&t)); // Initialize rand()

    unsigned long long int start;
    unsigned long long int start0 = cumulative_distribution(plaintext) * seed_space;
    unsigned long long int end = (start0 + probability_distribution(plaintext)*seed_space) - 1;
    start = (unsigned long long int)(start0); // Make it an int after we use it in the end calculation
    double r = (double)rand() / (double)RAND_MAX; // Any decimal value between 0 and 1
    return(unsigned long long int)((double)(r*(end-start) + start));
}

unsigned long long int decode(unsigned long long int ciphertext) {
    double seed_location; // = ciphertext / seed_space. Example seed_location = 0.0001590605715830
    // Need to do the following due to overflow
    printf("Please compute %llu / %llu and Enter:\n",ciphertext,seed_space);
    scanf("%lf",&seed_location);

    int i = binary_search(0,INVERSE_TABLE_LENGTH,seed_location);
    double prev_value = inverse_table[i][0];
    unsigned long long int prev_msg = inverse_table[i][1];
    unsigned long long int next_msg = next_message(prev_msg);

    char next_msg_str[30];
    ITOA(next_msg,next_msg_str); // Convert next_msg into a string using our custom ITOA
    double next_value = cumulative_distribution(next_msg_str); // Using next_msg_str get the cumul_dist

    if(next_msg==prev_msg) // at max message. checks to see if next==prev
        return prev_msg;

    // begin linear scan to find which range seed s falls in
    while(seed_location >= next_value) {
        // update prev and next
        prev_value = next_value;
        prev_msg = next_msg;
        next_msg = next_message(prev_msg);
        ITOA(next_msg,next_msg_str);
        next_value = cumulative_distribution(next_msg_str);
    }

    return prev_msg;
}
