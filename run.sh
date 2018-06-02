clean="rm notebook"
echo $clean
$clean

compile="gcc -o notebook main.c file.c dynaArray.c btree.c"
echo $compile
$compile

run="./notebook notebook.nb"
echo $run
$run
