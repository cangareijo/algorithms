#include <stdio.h>

void print_permutations_heaps(const unsigned n) {
  if (n == 0) return;
  unsigned state[n] = {};
  unsigned permutation[n];
  for (unsigned i = 0u; i < n; i++) permutation[i] = i;
  for (unsigned i = 0u; i < n; i++) printf("%u ", permutation[i]);
  printf("\n");
  unsigned current = 1u;
  while (current < n)
    if (state[current] < current) {
      const unsigned target = current % 2 == 0 ? 0u : state[current];
      const unsigned temporary = permutation[target];
      permutation[target] = permutation[current];
      permutation[current] = temporary;
      for (unsigned i = 0u; i < n; i++) printf("%u ", permutation[i]);
      printf("\n");
      state[current]++;
      current = 1u;
    } else {
      state[current] = 0u;
      current++;
    }
}

void print_permutations_lexicographical(const unsigned n) {
  if (n == 0) return;
  unsigned permutation[n];
  for (unsigned i = 0u; i < n; i++) permutation[i] = i;
  while (true) {
    for (unsigned i = 0u; i < n; i++) printf("%u ", permutation[i]);
    printf("\n");
    unsigned pivot = n - 1u;
    while (pivot > 0u && permutation[pivot - 1u] >= permutation[pivot]) pivot--;
    if (pivot == 0u) break;
    pivot--;
    unsigned successor = n - 1u;
    while (permutation[pivot] >= permutation[successor]) successor--;
    const unsigned temporary = permutation[pivot];
    permutation[pivot] = permutation[successor];
    permutation[successor] = temporary;
    unsigned left = pivot + 1u;
    unsigned right = n - 1u;
    while (left < right) {
      const unsigned reverse = permutation[left];
      permutation[left] = permutation[right];
      permutation[right] = reverse;
      left++;
      right--;
    }
  }
}

int main() {
  print_permutations_lexicographical(4);
  return 0;
}
