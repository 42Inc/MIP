#!/usr/bin/gnuplot
set terminal png size 800,480 enhanced font 'Arial, 16'
set output 'data/v1_result.png'

set style line 1 linecolor rgb 'red' linetype 1 linewidth 2
set style line 2 linecolor rgb 'blue' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics
set mxtics
set format y "%.0f"
set xlabel "Amount of graph's vertexes" font "Arial, 16"
set format x "%.0f"
set ylabel "Estimated time" font "Arial, 16"
set xtics font "Arial, 12"
set ytics font "Arial, 12"
set rmargin 4
set tmargin 2

plot "data/v2.dat" using 1:2 title "Full enumeration" with linespoints ls 1,\
     "data/v2.dat" using 1:3 title "Optimized Method" with linespoints ls 2
