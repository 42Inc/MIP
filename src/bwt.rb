#!/usr/bin/env ruby
# frozen_string_literal: false

# BWT
class BWT
  def initialize
    @array = []
    @string = ''
    @blocksize = 10 # megabytes 8
    @block_read = false
    @c = ''
  end

  def megabytes(num)
    num * (2**20)
  end

  # TODO: make readbyte logic. make block-coder logic
  def run_coder
    loop do
      @string = ''
      @array = []
      break if STDIN.eof?

      count = 0
      while count < @blocksize
        break if STDIN.eof?

        @c = STDIN.readbyte
        break if @c.nil?

        @string.concat(@c)
        count += 1 if @block_read
      end

      STDERR.print("Read - #{@string}\n")
      length = @string.length
      length.times do |v|
        @array.push v
      end
      STDERR.print("Rotate index array:\n")
      @array.each_with_index do |v, i|
        STDERR.print("#{i}\t#{v}\n")
      end

      @array.sort! do |a, b|
        a1 = @string.split('').rotate(a)
        b1 = @string.split('').rotate(b)
        a1[a1.length - 2] <=> b1[b1.length - 2]
      end
      STDERR.print("\nSorted Rotate index array: \n")
      @array.each_with_index do |v, i|
        STDERR.print("#{i}\t#{v}\n")
      end

      STDERR.print("\nResult: \n")
      index = length == 1 ? 0 : @array.index(1)
      str = []
      @array.each_with_index do |v, _i|
        tmp = @string.split('').rotate(v)
        str.push(tmp[tmp.length - 1])
      end
      STDERR.print("#{str.join} #{index} ")
      STDOUT.print("#{str.join} #{index} ")
      break unless @block_read
    end
  end

  # TODO: make readbyte logic. make block-decoder logic
  def run_decoder
    @string = STDIN.read.split ' '
    return if @string.length < 2

    index = @string[1].clone.to_i
    str = @string[0]
    array_orig = []
    array1 = []
    array2 = []
    array_res = []
    STDERR.print("Read - #{index} #{str}\n")
    (1..str.length).each_with_index do |_v, i|
      array1.push [str[i], i]
      array_orig.push [str[i], i]
    end
    STDERR.print("Array: \n")
    array_orig.each_with_index do |v, _i|
      STDERR.print("\t#{v}\n")
    end
    array1.sort! do |a, b|
      a[0].bytes[0] <=> b[0].bytes[0]
    end
    STDERR.print("\nSorted array 1: \n")
    array1.each_with_index do |v, _i|
      STDERR.print("\t#{v}\n")
    end
    (1..str.length).each_with_index do |v, _i|
      array2.push(array1[array1.index(array_orig[v - 1])].clone)
      array2.last[1] = array1.index(array_orig[v - 1])
    end
    STDERR.print("\nArray 2: \n")
    array2.each_with_index do |v, _i|
      STDERR.print("\t#{v}\n")
    end
    STDERR.print("\nResult: \n")
    (1..str.length).each do |_v|
      array_res.push array2[index].clone.first
      index = array2[index].clone.last
    end
    STDOUT.print(array_res.join.to_s)
  end
end

def helper
  STDERR.print("Usage:\n")
  STDERR.print("\t./bin/bwt coder\n")
  STDERR.print("\t./bin/bwt decoder\n")
end

if !ARGV.empty?
  if ARGV[0] == 'coder'
    BWT_Obj = BWT.new
    BWT_Obj.run_coder
  elsif ARGV[0] == 'decoder'
    BWT_Obj = BWT.new
    BWT_Obj.run_decoder
  else
    STDERR.print("Parameter not found! [BWT]\n")
    helper
  end
else
  STDERR.print("Parameter not found! [BWT]\n")
  helper
end
