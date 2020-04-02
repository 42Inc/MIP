#!/usr/bin/gnuplot
set terminal png size 800,480 enhanced font 'Arial, 16'
set output 'data/v4_prog_result.png'

set style line 1 linecolor rgb 'red' linetype 1 linewidth 2
set style line 2 linecolor rgb 'blue' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics 5
set mxtics 10
set format y "%.0f"
set xlabel "Number of experiments" font "Arial, 16"
set format x "%.0f"
set ylabel "Number of symbols" font "Arial, 16"
set xtics 50 font "Arial, 12"
set ytics 30 font "Arial, 12"
set rmargin 4
set tmargin 2

plot "data/fr.dat" using 1:2 title "First Reader" with linespoints ls 1,\
     "data/sr.dat" using 1:2 title "Seconde Reader" with linespoints ls 2
