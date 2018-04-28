int binary_search(int start, int end, double value) {
  int size = end - start;

  if(size == 1 || size == 0) {
    // printf("binary search: %i %i\n",start,end);
    return start;
  }

  int mid = start + (double)size/2;
  double mid_value = inverse_table[mid][0];
  int mid_msg = inverse_table[mid][1];

  // printf("binary search: %i %i %0.15f %i %0.15f\n",start,end,value,mid,mid_value);

  if(value >= mid_value)
    return binary_search(mid, end, value);
  else
    return binary_search(start,mid,value);
}

// int main() {
//   return 0;
// }
