#!/usr/bin/env ruby
class MTF
  def initialize
    @alphabet = Array.new
    @c = '\0'
    @n = 0
    STDERR.print("Generating alphabet...\n")
    (0..255).each_with_index do |v, i|
      @alphabet.push(i.chr)
    end
  end

  def run_compress
    STDERR.print("#{@alphabet}\n")
    while true
      @c = STDIN.read().chomp!
      break if @c.nil?
      _array = @c.bytes
      _array.each_with_index do |v, i|
        STDERR.print("Read code - #{v}\n")
        STDERR.print("Found with index - #{@alphabet.index(v.chr)}\n")
        STDOUT.print("#{@alphabet.index(v.chr)} ")
        @alphabet.unshift(@alphabet.delete(v.chr))
        STDERR.print("#{@alphabet}\n")
      end
    end
  end

  def run_decompress
    STDERR.print("#{@alphabet}\n")
    while true
      break if STDIN.eof?
      @n = STDIN.read()
      break if @n.nil?
      _array = @n.split(' ')
      _array.each_with_index do |v, i|
        _index = v.to_i
        STDERR.print("Found by index [#{v} | #{_index}] - #{@alphabet[_index]}\n")
        STDOUT.print("#{@alphabet[_index]}")
        @alphabet.unshift(@alphabet.delete(@alphabet[_index]))
        STDERR.print("#{@alphabet}\n")
      end
    end
  end
end

if (ARGV.length > 0)
  if (ARGV[0] == "compress")
    MTF_Obj = MTF.new
    MTF_Obj.run_compress
  elsif (ARGV[0] == "decompress")
    MTF_Obj = MTF.new
    MTF_Obj.run_decompress
  else
    STDERR.print("Parameter not found! [MTF]\n")
  end
else
  STDERR.print("Parameter not found! [MTF]\n")
end
