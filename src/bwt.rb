#!/usr/bin/env ruby
class BWT
  def initialize
    @array = Array.new
    @string = ""
    @blocksize = megabytes 8
    @c = ""
  end

  def megabytes num
    num * (2**20)
  end
  #TODO: make readbyte logic. make block-coder logic
  def run_coder
    _count = 0;
    while (_count < @blocksize)
      break if STDIN.eof?
      @c = STDIN.readbyte
      break if @c.nil?
      @string.concat(@c)
      _count = _count + 1
    end

    STDERR.print("Read - #{@string}\n")
    _length = @string.length
    _length.times do |v|
      @array.push v
    end
    STDERR.print("Rotate index array:\n")
    @array.each_with_index do |v, i|
      STDERR.print("#{i}\t#{v}\n")
    end

    @array.sort! do |a, b|
      _a = @string.split('').rotate(a)
      _b = @string.split('').rotate(b)
      _a[_a.length - 2] <=> _b[_b.length - 2]
    end
    STDERR.print("\nSorted Rotate index array: \n")
    @array.each_with_index do |v, i|
      STDERR.print("#{i}\t#{v}\n")
    end

    STDERR.print("\nResult: \n")
    _index = @array.index(1)
    _str = Array.new
    @array.each_with_index do |v, i|
      _tmp = @string.split('').rotate(v)
      _str.push(_tmp[_tmp.length - 1])
    end
    STDOUT.print("#{_str.join} #{_index} ")
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
