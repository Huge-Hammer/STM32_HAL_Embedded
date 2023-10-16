from PIL import Image
import io

with open('Binocular.txt', 'r') as file:
    hex_data = file.read().strip()

# 转换16进制数据为字节数据
byte_data = bytes.fromhex(hex_data)

# 找到第一个图像的起始位置
start1 = byte_data.find(b'\xFF\xD8')

# 找到第一个图像的结束位置
end1 = byte_data.find(b'\xFF\xD9', start1)

# 提取第一个图像数据
image1_data = byte_data[start1:end1 + 2]

# 找到第二个图像的起始位置
start2 = byte_data.find(b'\xFF\xD8', end1)

# 找到第二个图像的结束位置
end2 = byte_data.find(b'\xFF\xD9', start2)

# 提取第二个图像数据
image2_data = byte_data[start2:end2 + 2]

# 创建一个BytesIO对象，将图像数据加载到其中
image_stream1 = io.BytesIO(image1_data)
image_stream2 = io.BytesIO(image2_data)

# 使用PIL打开图像
image1 = Image.open(image_stream1)
image2 = Image.open(image_stream2)

# 确保两张图像具有相同的分辨率
if image1.size == image2.size:
    # 获取两张图像的宽度和高度
    width, height = image1.size

    # 创建一个新的图像，宽度为两张图像的宽度，高度为一张图像的高度
    new_image = Image.new('RGB', (width * 2, height))

    # 将第一张图像粘贴到左边
    new_image.paste(image1, (0, 0))

    # 将第二张图像粘贴到右边
    new_image.paste(image2, (width, 0))

    # 显示拼接后的图像
    new_image.show()
    # 如果需要保存拼接后的图像，可以使用以下代码
    new_image.save('output.jpg')

else:
    print("图像分辨率不一致，无法拼接。")