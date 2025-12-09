# Auto-generated gnuplot script
set terminal pngcairo size 1000,800 enhanced font 'Arial,12'
set output 'result.png'
set title 'Root-Finding Iterations'
set xlabel 'x'
set ylabel 'f(x)'
set grid
plot 'result.dat' using 1:2 with linespoints lt rgb 'blue' pt 7 lw 2 title 'Iteration Path'
