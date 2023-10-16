from PIL import Image
import io

# 从第一个文本文件中读取十六进制数据
with open('left.txt', 'r') as file:
    hex_data = file.read().strip()

# 将十六进制数据转换为字节流
image_data = bytes.fromhex(hex_data)

# 创建一个BytesIO对象，将图像数据加载到其中
image_stream = io.BytesIO(image_data)

# 使用PIL打开图像
image = Image.open(image_stream)

# 获取图像的宽度和高度
width, height = image.size

# 创建一个新的图像，宽度为图像的宽度，高度为一张图像的高度
new_image = Image.new('RGB', (width, height))

# 将第图像粘贴到左边
new_image.paste(image, (0, 0))

# 显示拼接后的图像
new_image.show()

new_image.save('left.jpg')

