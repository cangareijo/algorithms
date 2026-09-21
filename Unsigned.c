#include <stdio.h>

void print_permutations(const unsigned n) {
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

int main() {
  print_permutations(4);
  return 0;
}
