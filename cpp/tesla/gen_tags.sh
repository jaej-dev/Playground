#!/bin/sh

echo "Start to create tags file..."
rm cscope.files
rm tags

find . \( -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' \) -print > cscope.files

ctags -R -L cscope.files
echo "Completed to create tags file..."

