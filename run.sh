clean="rm Bash"
#echo $clean
$clean

compile="gcc -o Bash main.c file.c struct.c"
#echo $compile
$compile

run="./Bash ../notebook.nb"
#echo $run
$run