import base64

# 本题自定义 Base64 字符表
custom_table = "/+9876543210zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA"
# 标准 Base64 字符表
standard_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

# IDA 中找到的 cipher
cipher = "mZOemISenMjIxseeydLLzMnO0suayM/SnZvPndLKx8eZzp6dysfGzZyC"

# 将程序使用的自定义 Base64 编码结果还原为原始字符串
def decode_custom_base64(cipher_text):
    # 建立映射：
    # custom_table[i] -> standard_table[i]
    trans_table = str.maketrans(custom_table, standard_table)

    # 把自定义 Base64 转换成标准 Base64
    standard_b64 = cipher_text.translate(trans_table)

    # 标准 Base64 解码
    decoded_bytes = base64.b64decode(standard_b64)

    # 转换为字符串
    decoded_text = decoded_bytes.decode("utf-8")

    return decoded_text

if __name__ == "__main__":
    try:
        flag = decode_custom_base64(cipher)

        print("\n解码得到的 flag:")
        print(flag)

    except Exception as e:
        print("解码失败:", e)