#!/bin/sh

echo "Start to create tags file..."
rm cscope.out
rm cscope.files
rm tags

find . \( -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' \) -print > cscope.files
cscope -i cscope.files

ctags -R -L cscope.files
echo "Completed to create tags file..."

