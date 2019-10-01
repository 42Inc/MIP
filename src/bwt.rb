#!/usr/bin/env ruby
class BWT
  def initialize
    @array = Array.new
    @string = ""
  end

  def run_coder
    @string = STDIN.read().chomp!.split ''
    STDERR.print("Read - #{@string}\n")
    _length = @string.length
    (1.._length).each do |v|
      @array.push @string.clone
      @string = @string.rotate 1
    end
    STDERR.print("Array - #{@array}\n")
  end

  def run_decoder

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
