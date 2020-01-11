
#include <iostream> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h> s
#define MAX_ITERS 1000000

using namespace std;

double Rand(double L, double R)
{
	return L + (R - L) * rand() * 1.0 / RAND_MAX;
}

int main()
{
	srand(time(NULL));
	int cnt = 0;
	for (int i = 0; i < MAX_ITERS; i++)
	{
		double x = Rand(-1, 1);
		double y = Rand(-1, 1);
		if (x * x + y * y <= 1)
			cnt++;
	}
	cout << "pi:" << cnt * 4.0 / MAX_ITERS;
	return 0;
}