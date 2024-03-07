#include "main.h"
#include "dungeon.h"
#include "bsptree.h"

using namespace std;

void test(int arr[]) {
	if (sizeof(arr) > 0) {
		arr[0] = 3;
	}
}

int main(int argc, char* args[]) {
	int width, length, seed;
	cout << "Enter seed for generation: ";
	cin >> seed;
	cout << "Enter a width for the dungeon: ";
	cin >> width;
	cout << "Enter a length for the dungeon: ";
	cin >> length;

	srand(seed);
	Dungeon dungeon = Dungeon(width, length);

	BSPTree bspTreeGenerator = BSPTree();
	bspTreeGenerator.generate(&dungeon);
	dungeon.print();

	return 0;
}
