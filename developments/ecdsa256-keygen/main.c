#include <main.h>

static void usage(void)
{
	printf("usage:\r\n");
	printf("    ecdsa256-keygen [-r] [-s] [-c]\r\n");
}

int main(int argc, char * argv[])
{
	uint8_t public_key[33] = { 0 };
	uint8_t private_key[32] = { 0 };
	int r = 0, s = 0, c = 0;
	unsigned int seed;
	int fd, i;

	if(argc == 1)
	{
		r = 1;
		s = 1;
		c = 1;
	}
	else
	{
		for(i = 1; i < argc; i++)
		{
			if(!strcasecmp(argv[i], "-r"))
			{
				r = 1;
			}
			else if(!strcasecmp(argv[i], "-s"))
			{
				s = 1;
			}
			else if(!strcasecmp(argv[i], "-c"))
			{
				c = 1;
			}
			else
			{
				usage();
				return -1;
			}
		}
	}
	if(r)
	{
		fd = open("/dev/random", O_RDONLY);
		if(fd)
		{
			if(read(fd, &seed, sizeof(seed)) != sizeof(seed))
				seed = (unsigned int)time(NULL);
			close(fd);
		}
	}
	else
	{
		seed = (unsigned int)time(NULL);
	}
	srand(seed);
	if(!ecdsa256_keygen(public_key, private_key))
		return -1;
	if(s)
	{
		printf("PUBLIC_KEY\t:= \"");
		for(i = 0; i < sizeof(public_key); i++)
			printf("%02x", public_key[i]);
		printf("\"\r\n");
		printf("PRIVATE_KEY\t:= \"");
		for(i = 0; i < sizeof(private_key); i++)
			printf("%02x", private_key[i]);
		printf("\"\r\n");
	}
	if(c)
	{
		printf("static const uint8_t public_key[] = {");
		for(i = 0; i < sizeof(public_key); i++)
		{
			if(i % 8 == 0)
				printf("\r\n\t");
			printf("0x%02x, ", public_key[i]);
		}
		printf("\r\n}\r\n");
		printf("static const uint8_t private_key[] = {");
		for(i = 0; i < sizeof(private_key); i++)
		{
			if(i % 8 == 0)
				printf("\r\n\t");
			printf("0x%02x, ", private_key[i]);
		}
		printf("\r\n}\r\n");
	}
	return 0;
}
