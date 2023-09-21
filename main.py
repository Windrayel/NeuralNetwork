from PIL import Image
import random
import os

black = (0, 0, 0)
white = (256, 256, 256)


def invert(pixel):
    return white if pixel == black else black


def base_image():
    img = Image.new('RGB', (7, 7), "white")  # create a new white image
    pixels = img.load()  # create the pixel map
    return pixels, img


def circle():
    pixels, img = base_image()
    for i in range(2, 5):
        pixels[i, 1] = black
        pixels[1, i] = black
        pixels[i, 5] = black
        pixels[5, i] = black
    return pixels, img


def square():
    pixels, img = base_image()
    for i in range(1, 6):
        pixels[i, 1] = black
        pixels[1, i] = black
        pixels[i, 5] = black
        pixels[5, i] = black
    return pixels, img


def triangle():
    pixels, img = base_image()
    for i in range(7):
        pixels[i, 5] = black
        j = 4-i if i < 4 else i-2
        pixels[i, j] = black
    return pixels, img


# for i in range(140, 150):
#     pixels, img = circle()
#
#     for j in range((i // 10) % 5):
#         col = random.randint(0, 6)
#         row = random.randint(0, 6)
#         pixels[col, row] = invert(pixels[col, row])
#
#     directory = os.path.dirname(os.path.realpath(__file__)) + "/Images/"
#     img.save(directory + str(i) + ".png", "PNG")

# pixels, img = triangle()
# directory = os.path.dirname(os.path.realpath(__file__)) + "/Images/"
# img.save(directory + str(0) + ".png", "PNG")
for i in range(10):
    print(round(random.uniform(0.85, 0.95), 2))
    # print(round(random.uniform(0.1, 0.17), 2))
