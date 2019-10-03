#!/usr/bin/env ruby
class MTF
  def initialize
    @alphabet = Array.new
    @c = '\0'
    @p = '\0'
    @n = 0
    STDERR.print("Generating alphabet...\n")
    (0..255).each_with_index do |v, i|
      @alphabet.push(i)
    end
  end

  def run_compress
    STDERR.print("#{@alphabet}\n")
    _first_run = true
    while true
      break if STDIN.eof?
      @c = STDIN.readbyte
      break if @c.nil?
      STDOUT.print("#{@alphabet.index(@c)} ")
      STDERR.print("Read - #{@c} [#{@alphabet.index(@c)}]\n")
      STDERR.print("#{@alphabet}\n")
#      _array.each_with_index do |v, i|
#        STDOUT.print("#{@alphabet.index(v.chr)} ")
      @alphabet.unshift(@alphabet.delete(@c))
    end
  end

  def is_num?(param)
    '0' <= param.chr && param.chr <= '9'
  end

  def run_decompress
    STDERR.print("#{@alphabet}\n")
    _first_run = true
    while true
      @n = 0
      break if STDIN.eof?
      loop do
        @c = STDIN.readbyte
        break if @c.nil?
        break unless is_num? @c
        @n = @n*10 + @c.to_i - '0'.bytes[0]
      end
#      _array.each_with_index do |v, i|
      _index = @n
      STDERR.print("Read - #{@n} [#{_index} #{@alphabet[_index]}]\n")
      STDOUT.print("#{@alphabet[_index].chr}")
      @alphabet.unshift(@alphabet.delete(@alphabet[_index]))
      STDERR.print("#{@alphabet}\n")
#      end
    end
  end
end

def helper
  STDERR.print("Usage:\n")
  STDERR.print("\t./bin/mtf compress\n")
  STDERR.print("\t./bin/mtf decompress\n")
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
    helper
  end
else
  STDERR.print("Parameter not found! [MTF]\n")
  helper
end
