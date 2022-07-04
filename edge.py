from PIL import Image, ImageFilter

before = Image.open("images/yard.bmp")
after = before.filter(ImageFilter.FIND_EDGES)
after.save("pythonedgeout.bmp")