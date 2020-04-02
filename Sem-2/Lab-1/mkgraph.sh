#!/usr/bin/gnuplot

set terminal png size 800,480 enhanced font 'Arial, 10'

set style line 1 linecolor rgb 'red' linetype 2 linewidth 2
set style line 3 linecolor rgb 'blue' linetype 1 linewidth 2
set style line 2 linecolor rgb 'green' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics 10
set format x "%.2f"
set format y "%.4f"
set xlabel "a" font "Arial, 16"
set ylabel "n" font "Arial, 16"
#et xtics 8 font "Arial, 10"
#set ytics 1 font "Arial, 10"
set rmargin 4
set tmargin 2
set mxtics

set output 'res.png'
plot "log.dat" using 1:2 title "Count success iteration" with linespoints ls 1
