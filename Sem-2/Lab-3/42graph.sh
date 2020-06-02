#!/usr/bin/gnuplot
set terminal png size 1920,1080 enhanced font 'Arial, 16'
set output 'data/branch_result.png'

set style line 1 linecolor rgb 'red' linetype 1 linewidth 2
set style line 2 linecolor rgb 'blue' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics
set mxtics
set format y "%.6f"
set xlabel "Graph's vertices" font "Arial, 16"
set format x "%.0f"
set ylabel "Estimated time (sec)" font "Arial, 16"
set xtics font "Arial, 12"
set ytics font "Arial, 12"
set rmargin 4
set tmargin 2

plot "data/branch.dat" using 1:2 title "Branch and Bounds" with linespoints ls 1
     # "data/graph.dat" using 1:3 title "Branch and Bounds" with linespoints ls 2
