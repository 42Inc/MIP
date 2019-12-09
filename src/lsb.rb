#!/usr/bin/env ruby

require 'rmagick'

$image = "cat.png"
$img = nil
$columns = 0
$rows = 0
$pix = 0

$mask = 0xFFFF

$percent = 0.25

def show_info(fname)
  $img = Magick::Image::read(fname).first
  fmt = $img.format
  $columns,$rows = $img.columns, $img.rows
  $pix = $columns*$rows
  dep = $img.depth
  nc = $img.number_colors
  nb = $img.filesize
  xr = $img.x_resolution
  yr = $img.y_resolution
  res = Magick::PixelsPerInchResolution ? "дюйм" : "см"
  puts <<-EOF
Файл: #{fname}
Формат: #{fmt}
Размеры: #{$columns}x#{$rows} пикселей (#{$pix})
Цветов: #{nc}
Длина файла: #{nb} байтов
Разрешение: #{xr}/#{yr} пикселей на #{res}
EOF
  puts
end

def save_red_channel(_image, prefix)
  img_tmp = _image.clone
  _image.rows.times do |y|
    pixels = img_tmp.get_pixels(0, y, _image.columns, 1)
    pixels.each_with_index do |p,x|
      if (p.red & 0x1 > 0)
        img_tmp.pixel_color(x, y, "black")
      else
        img_tmp.pixel_color(x, y, "white")
      end
    end
  end
  img_tmp.write(prefix+$image)
end

def save_green_channel(_image, prefix)
  img_tmp = _image.clone
  _image.rows.times do |y|
    pixels = img_tmp.get_pixels(0, y, _image.columns, 1)
    pixels.each_with_index do |p,x|
    if (p.green & 0x1 > 0)
      img_tmp.pixel_color(x, y, "black")
    else
      img_tmp.pixel_color(x, y, "white")
    end
  end
#    img_tmp.store_pixels(0, y, $columns, 1, pixels)
  end
  img_tmp.write(prefix+$image)
end

def save_blue_channel(_image, prefix)
  img_tmp = _image.clone
  _image.rows.times do |y|
    pixels = img_tmp.get_pixels(0, y, _image.columns, 1)
    pixels.each_with_index do |p,x|
       if (p.blue & 0x1 > 0)
         img_tmp.pixel_color(x, y, "black")
       else
         img_tmp.pixel_color(x, y, "white")
       end
    end
  #    img_tmp.store_pixels(0, y, $columns, 1, pixels)
  end
  img_tmp.write(prefix+$image)
end

def save_combine_channel(_image, prefix)
  img_tmp = _image.clone
  depth=img_tmp.depth
  max = depth == 8 ? 0xff : $mask
  max = $mask
  _image.rows.times do |y|
    pixels = img_tmp.get_pixels(0, y, _image.columns, 1)
    pixels.each_with_index do |p,x|
       if (p.red & 0x1 > 0)
         p.red=max
       else
         p.red=0
       end
       if (p.green & 0x1 > 0)
         p.green=max
       else
         p.green=0
       end
       if (p.blue & 0x1 > 0)
         p.blue=max
       else
         p.blue=0
       end
#       STDERR.print("Write #{p}\n")
       img_tmp.pixel_color(x, y, p)
    end
  #    img_tmp.store_pixels(0, y, $columns, 1, pixels)
  end
  img_tmp.write(prefix+$image)
end

def lsb_code(fname, prefix)
  return if $img.nil?
  print "LSB code for #{fname}\n"
  _write = $pix * $percent;
  cur = 0;
  1.times do |y|
    pixels = $img.get_pixels(0, y, 10, 1)
    pixels.each_with_index do |p,x|
       STDERR.print "#{p}[r][c] [#{y}][#{x}] = #{p.red.to_s(16)}#{p.green.to_s(16)}#{p.blue.to_s(16)}\n"
    end
  end
  save_red_channel($img, "lsb_coder_src_red_")
  save_green_channel($img, "lsb_coder_src_green_")
  save_blue_channel($img, "lsb_coder_src_blue_")
  save_combine_channel($img, "lsb_coder_src_comb_")
  img_tmp = $img.clone
  $img.rows.times do |y|
    pixels = img_tmp.get_pixels(0, y, $img.columns, 1)
    pixels.each_with_index do |p,x|
      if (cur < _write)
        rand_num_r = rand() > 0.5 ? 1 : 0
        rand_num_g = rand() > 0.5 ? 1 : 0
        rand_num_b = rand() > 0.5 ? 1 : 0
        p.red=(rand_num_r == 1 ? p.red | 0x1 : p.red & ($mask - 1))
        p.green=(rand_num_g == 1 ? p.green | 0x1 : p.green & ($mask - 1))
        p.blue=(rand_num_b == 1 ? p.blue | 0x1 : p.blue & ($mask - 1))
        img_tmp.pixel_color(x, y, p)
        cur = cur + 1
      end
    end
  #    img_tmp.store_pixels(0, y, $columns, 1, pixels)
  end
  img_tmp.write(prefix + "_" + $image)
  save_red_channel(img_tmp, "lsb_coder_#{prefix}_red_")
  save_green_channel(img_tmp, "lsb_coder_#{prefix}_green_")
  save_blue_channel(img_tmp, "lsb_coder_#{prefix}_blue_")
  save_combine_channel(img_tmp, "lsb_coder_#{prefix}_comb_")
end

def lsb_decode(fname)
  return if $img.nil?
  print "LSB decode for #{fname}\n"
end

def helper
  STDERR.print("Usage:\n")
  STDERR.print("\t./bin/lsb coder <image>\n")
  STDERR.print("\t./bin/lsb decoder <image>\n")
end

unless ARGV.empty? || ARGV.length < 2
  if ARGV[0] == 'coder'
    $image = ARGV[1]
    show_info($image)
    lsb_code($image, "code")
  elsif ARGV[0] == 'decoder'
    $image = ARGV[1]
    show_info($image)
    lsb_decode($image)
  else
    STDERR.print("Parameter not found! [LSB]\n")
    helper
  end
else
  STDERR.print("Parameter not found! [LSB]\n")
  helper
end
