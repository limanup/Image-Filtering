from PIL import Image, ImageFilter

before = Image.open("images/yard.bmp")
after = before.filter(ImageFilter.BoxBlur(1))
after.save("pythonblurout.bmp")