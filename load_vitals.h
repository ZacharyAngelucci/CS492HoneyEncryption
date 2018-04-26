int **prefixes;
int PREFIX_TABLE_LENGTH;
int *ordered_prefixes;
int ORDERED_PREFIX_TABLE_LENGTH;
float **inverse_table;
int INVERSE_TABLE_LENGTH;

int **load_prefixes() {

}

int *load_ordered_prefixes() {

}

int **load_inverse_table() {

}

void initialize() {
  prefixes = load_prefixes();
  load_inverse_table = load_ordered_prefixes();
  inverse_table = load_inverse_table();
}
