/*$@utils.c*/

long from_octdec(int which, char* str, int len)
{
  long raw = 0;
  int i;

  for (i = 0; i < len && str[i] >= '0' && str[i] <= (which ? '7' : '9'); i++) {
    raw = raw * (which ? 8 : 10) + (str[i] - '0');
  }

  return raw;
}

/* convert hexadecimal digits string to number */
long from_hex(char* hex, int len)
{
  long  raw = 0;
  int   i;

  for (i = 0; i < len && ((hex[i] >= '0' && hex[i] <= '9') || (hex[i] >= 'A' && hex[i] <= 'F') || (hex[i] >= 'a' && hex[i] <= 'f')); i++) {
    if (hex[i] >= '0' && hex[i] <= '9') raw = raw * 16 + (hex[i] - '0');
    else if (hex[i] >= 'a' && hex[i] <= 'f') raw = raw * 16 + (hex[i] - 'a' + 10);
    else if (hex[i] >= 'A' && hex[i] <= 'F') raw = raw * 16 + (hex[i] - 'A' + 10);
  }
  return raw;
}

long from_dec(char* dec, int len)
{
  return from_octdec(0, dec, len);
}

/*convert octal string to a number*/
long from_oct(char* oct, int len)
{
  return from_octdec(1, oct, len);
}
