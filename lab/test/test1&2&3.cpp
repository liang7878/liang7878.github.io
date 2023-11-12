// #1. 0 , 5
// #2. 0

// #3
char *num2str(unsigned int num, char* buffer, int buf_size) {
    char *p = buffer;
    for (unsigned int tmp = num / 10; tmp != 0; tmp/=10) {
        p++;
    }

    p++;
    *p = '\0';

    do{
        *--p = '0' + num % 10;
        num /= 10;
    } while (num);

    return p;
}
