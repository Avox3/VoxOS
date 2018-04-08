#include "utils.h"

// convert from int to it's ascii representation
void int_to_ascii(u8int val, char str[], u8int len)
{
    u8int i;
    for(i=1; i<=len; i++)
    {
      str[len-i] = (u8int) ((val % 10UL) + '0');
      val/=10;
    }
    str[i-1] = '\0';
}
