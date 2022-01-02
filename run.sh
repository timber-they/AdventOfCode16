#!/bin/bash

clang $1.c -Ofast -Werror -Wextra -Wall -fsanitize=address -g -lssl -lcrypto && ./a.out
