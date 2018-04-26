#include <stdio.h>
#include <stdlib.h>

#define PREFIX_TABLE_LENGTH 2081
#define ORDERED_PREFIX_TABLE_LENGTH 2081
#define INVERSE_TABLE_LENGTH 150881

int prefixes[PREFIX_TABLE_LENGTH][4];
int ordered_prefixes[ORDERED_PREFIX_TABLE_LENGTH][4];
float inverse_table[INVERSE_TABLE_LENGTH][2];
int total_prob;

void load_prefixes(int t[PREFIX_TABLE_LENGTH][4]) {
  // Assume max 20 chars per line. Multiply that by #lines = buffersize
  char buf[20 * PREFIX_TABLE_LENGTH];
  FILE *f;
  f = fopen("bin.txt","r");
  if(f) {
    char c;
    while( !feof(f) ) {
      c = fgetc(f);
      printf("%c",c);
    }
    free(buf);
  }
  else {
    printf("Could not open file");
  }
}

// This function has to go through the prefixes and sort them
// If the prefix has a **, treat it as a 9
void load_ordered_prefixes(int t[ORDERED_PREFIX_TABLE_LENGTH][4]) {
  // Assume max 20 chars per line. Multiply that by #lines = buffersize
  char buf[20 * ORDERED_PREFIX_TABLE_LENGTH];

  // stuff

  free(buf);
}

void load_inverse_table(float t[INVERSE_TABLE_LENGTH][2]) {
  // On average let's say 48 characters per line just to be safe
  // We then multiply 48 * #lines and that will be our buffer size
  char buf[48 * INVERSE_TABLE_LENGTH];

  // stuff somewhat similar to load_prefixes

  free(buf);
}

void initialize() {
  load_prefixes(prefixes);
  load_ordered_prefixes(ordered_prefixes);
  load_inverse_table(inverse_table);
  total_prob = 1;
}

// For testing purposes only. I'd recommend only trying to load one function at a time.
int main() {
  printf("RUNNING:\n");
  load_prefixes(prefixes);
}
