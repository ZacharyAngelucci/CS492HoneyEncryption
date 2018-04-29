/*
  A simple binary search algorithm to go through the inverse table
  Looking for 'value'
*/

int binary_search(int start, int end, double value) {
  int size = end - start;

  if(size == 1 || size == 0) {
    return start;
  }

  int mid = start + (double)size/2;
  double mid_value = inverse_table[mid][0];
  int mid_msg = inverse_table[mid][1];

  if(value >= mid_value)
    return binary_search(mid, end, value);
  else
    return binary_search(start,mid,value);
}
