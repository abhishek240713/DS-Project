#!/bin/sh
set -e
cd "$(dirname "$0")"

echo "Building Library Management System..."
gcc -std=c99 -Wall -Wextra -Wpedantic -O2 main.c books.c members.c transactions.c reservations.c stack.c queue.c bst.c sorting.c search.c file_manager.c reports.c utils.c -o library
echo "Build successful: library"
