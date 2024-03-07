#include <cstdlib>

#include "random.h"

double randomDouble() {
	return ((double)std::rand() / (double)RAND_MAX);
}

int randomInt(int from, int to) {
	if (to <= from) { return from; }

	int range = to - from;
	return from + (rand() % range);
}
