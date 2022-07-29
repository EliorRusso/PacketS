#include <linux/types.h>
#include <stdio.h>
struct {
	unsigned int widthValidated : 1;
}status1;
int main(){
    printf("%d", sizeof(status1));
}