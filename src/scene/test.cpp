#include <stdio.h>

#include <proto/bar.pb.h>

int main()
{
	bar_msg bmsg;
	bmsg.set_name("bbbb");
	printf("hello world!\n");
	getchar();
	return 0;
}