#!/usr/bin/env ruby
class BWT
  def initialize
    @array = Array.new
    @string = ""
  end
  #TODO: make readbyte logic. make block-coder logic
  def run_coder
    @string = STDIN.read().chomp!.split ''
    STDERR.print("Read - #{@string}\n")
    _length = @string.length
    _length.times do |v|
      @array.push @string.clone
      @string.rotate! 1
    end
    STDERR.print("Array:\n")
    @array.each_with_index do |v, i|
      STDERR.print("#{i}\t#{v.join}\n")
    end
    _original_string = @array[0]
    @array.sort! do |a, b|
      _a = a.clone
      _b = b.clone
      _a.pop
      _b.pop
      _a.reverse!
      _b.reverse!
      _a <=> _b
    end
    STDERR.print("\nSorted array: \n")
    @array.each_with_index do |v, i|
      STDERR.print("#{i}\t#{v.join}\n")
    end
    STDERR.print("\nResult: \n")
    _index = @array.index(_original_string.rotate(1))
    _str = Array.new
    @array.each_with_index do |v, i|
      _str.push v[v.length - 1]
    end
    STDOUT.print("#{_str.join} #{_index}")
  end

  def run_decoder
      @string = STDIN.read().split ' '
      return if @string.length < 2
      _index = @string[1].clone.to_i
      _str = @string[0]
      _array_orig = Array.new
      _array_1 = Array.new
      _array_2 = Array.new
      _array_res = Array.new
      STDERR.print("Read - #{_index} #{_str}\n")
      (1.._str.length).each_with_index do |v, i|
        _array_1.push ([_str[i], i])
        _array_orig.push ([_str[i], i])
      end
      STDERR.print("Array: \n")
      _array_orig.each_with_index do |v, i|
        STDERR.print("\t#{v}\n")
      end
      _array_1.sort! do |a, b|
        a[0].bytes[0] <=> b[0].bytes[0]
      end
      STDERR.print("\nSorted array 1: \n")
      _array_1.each_with_index do |v, i|
        STDERR.print("\t#{v}\n")
      end
      (1.._str.length).each_with_index do |v, i|
        _array_2.push (_array_1[_array_1.index(_array_orig[v - 1])]).clone
        _array_2.last[1] = _array_1.index(_array_orig[v - 1])
      end
      STDERR.print("\nArray 2: \n")
      _array_2.each_with_index do |v, i|
        STDERR.print("\t#{v}\n")
      end
      STDERR.print("\nResult: \n")
      (1.._str.length).each do |v|
        _array_res.push _array_2[_index].clone.first
        _index = _array_2[_index].clone.last
      end
      STDOUT.print("#{_array_res.join}")
  end
end

def helper
  STDERR.print("Usage:\n")
  STDERR.print("\t./bin/bwt coder\n")
  STDERR.print("\t./bin/bwt decoder\n")
end

if (ARGV.length > 0)
  if (ARGV[0] == "coder")
    BWT_Obj = BWT.new
    BWT_Obj.run_coder
  elsif (ARGV[0] == "decoder")
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
