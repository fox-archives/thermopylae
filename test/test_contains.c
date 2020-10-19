#include <stdio.h>
#include "../util.h"

void mustBeTrue(int ret, char* name) {
	if (ret == 1) {
		return;
	}

	printf("FAILED: %s\n", name);

}

void mustBeFalse(int ret, char* name) {
	if (ret == 0) {
		return;
	}

	printf("FAILED: %s\n", name);

}

int main() {
	{
	char *arr[3];
	arr[0] = "one";
	arr[1] = "two";
	arr[2] = "three";

	int ret = contains(arr, 3, "two");
	mustBeTrue(ret, "test1");
	}

	{
	char *arr[3];
	arr[0] = "one";
	arr[1] = "two";
	arr[2] = "three";

	int ret = contains(arr, 3, "four");
	mustBeFalse(ret, "test2");
	}

}
