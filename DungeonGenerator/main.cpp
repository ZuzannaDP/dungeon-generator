#include "main.h"
#include "dungeon.h"

using namespace std;

void test(int arr[]) {
	if (sizeof(arr) > 0) {
		arr[0] = 3;
	}
}

int main(int argc, char* args[]) {
	int width, length;
	cout << "Enter a width for the dungeon: ";
	cin >> width;
	cout << "Enter a length for the dungeon: ";
	cin >> length;

	Dungeon dungeon = Dungeon(width, length);
	cout << dungeon.getWidth();

	return 0;
}
