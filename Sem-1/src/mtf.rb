#!/usr/bin/env ruby
# frozen_string_literal: true

# MTF
class MTF
  def initialize
    @alphabet = []
    @c = '\0'
    @p = '\0'
    @n = 0
    STDERR.print("Generating alphabet...\n")
    (0..255).each_with_index do |_v, i|
      @alphabet.push(i)
    end
  end

  def run_compress
    STDERR.print("#{@alphabet}\n")
    loop do
      break if STDIN.eof?

      @c = STDIN.readbyte
      break if @c.nil?

      STDOUT.print("#{@alphabet.index(@c)} ")
      STDERR.print("Read - #{@c} [#{@alphabet.index(@c)}]\n")
      STDERR.print("#{@alphabet}\n")
      @alphabet.unshift(@alphabet.delete(@c))
    end
  end

  def num?(param)
    param.chr >= '0' && param.chr <= '9'
  end

  def run_decompress
    STDERR.print("#{@alphabet}\n")
    loop do
      @n = 0
      break if STDIN.eof?

      loop do
        @c = STDIN.readbyte
        break if @c.nil?
        break unless num? @c

        @n = @n * 10 + @c.to_i - '0'.bytes[0]
      end
      #      _array.each_with_index do |v, i|
      index = @n
      STDERR.print("Read - #{@n} [#{index} #{@alphabet[index]}]\n")
      STDOUT.print(@alphabet[index].chr.to_s)
      @alphabet.unshift(@alphabet.delete(@alphabet[index]))
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

if !ARGV.empty?
  if ARGV[0] == 'compress'
    MTF_Obj = MTF.new
    MTF_Obj.run_compress
  elsif ARGV[0] == 'decompress'
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
