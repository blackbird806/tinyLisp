#include <stdio.h>

int main()
{
	printf("#define FMT_CELL_CASES \\\n");
    for (int i = 1; i < 20; i++)
    {
        printf("case %d: printf(args[0].value.c_str()", i);
        for (int j = 1; j < i; j++)
        {
            printf(", to_string(args[%d]).c_str()", j);
        }
        puts("); break;\\");
    }
	puts("");
    return 0;
}