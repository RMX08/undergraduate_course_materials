def rol8(x, n):
    n %= 8
    return ((x << n) | (x >> (8 - n))) & 0xFF   
    # 左移n位，拼接上扔掉的前8-n位，最终只保留低8位


def make_password(username):
    name_len = len(username)
    result = []

    for i, ch in enumerate(username):
        # v5 = username[i] ^ (name_len + 7 * i)
        v5 = (ord(ch) ^ (name_len + 7 * i)) & 0xFF
        # ord() 把字符变成整数

        # 循环左移3位
        v5 = rol8(v5, 3)

        # 映射到 0~61
        v4 = v5 % 62

        # 映射到 0-9 / A-Z / a-z
        if v4 <= 9:
            c = chr(v4 + 48)       # 0~9
        elif v4 <= 35:
            c = chr(v4 + 55)       # A~Z
        else:
            c = chr(v4 + 61)       # a~z

        result.append(c)

    return ''.join(result)      # 拼接成字符串返回


username = "rev3admin"
password = make_password(username)

print("username =", username)
print("password =", password)