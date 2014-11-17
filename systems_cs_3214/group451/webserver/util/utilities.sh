#!/bin/bash

gcc -Werror -Wall -o hex2dd hex2dd.c

gcc -Werror -Wall -o dd2hex dd2hex.c

gcc -Werror -Wall -o hostinfo hostinfo.c

gcc -Werror -Wall -o serveri echoserveri.c util.c

gcc -Werror -Wall -o client echoclient.c util.c
