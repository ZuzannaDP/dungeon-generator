<<<<<<< HEAD
#include <cstdlib>
=======
#include <sstream>
>>>>>>> 45acbbd983da99c5f6a5e8f07e00b447b228857f

#include "random.h"

double randomDouble() {
	return ((double)std::rand() / (double)RAND_MAX);
}

int randomInt(int from, int to) {
	if (to <= from) { return from; }

	int range = to - from;
	return from + (rand() % range);
<<<<<<< HEAD
}
=======
}
>>>>>>> 45acbbd983da99c5f6a5e8f07e00b447b228857f
