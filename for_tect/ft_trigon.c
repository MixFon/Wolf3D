
#include "stdio.h"
#include "math.h"
#include "unistd.h"

int main(void)
{
	double angle;
	double rad;
	int i;

	i = -1;
	while (++i <= 360)
	{
		rad = angle * M_PI / 180;
		printf("angle = [%f] , rad = {%f}\n", angle, rad);
		printf("cos a = [%f] , cos r = {%f}\n", cos(angle * M_PI /180), cos(rad));
		angle++;
		printf("{%d}\n", -390 % 360);
		return (0);
	}
	return (0);
}
