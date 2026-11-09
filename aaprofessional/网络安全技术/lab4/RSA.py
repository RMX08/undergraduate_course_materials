import random

# 扩展欧几里得算法，用于计算模逆 d
# 寻找满足 ax + by = gcd(a, b) 的整数 x 和 y
def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        # 递归调用，利用辗转相除法的原理
        g, y, x = extended_gcd(b % a, a)
        return g, x - (b // a) * y, y

# 计算模逆 公式：e * d ≡ 1 (mod phi)
# 已知公钥 e 和欧拉函数 phi，计算出私钥 d。
def mod_inverse(e, phi):
    g, x, y = extended_gcd(e, phi)
    # 如果 g (最大公约数) 不等于 1，说明 e 和 phi 不互质，模逆不存在
    if g != 1:
        raise Exception('模逆不存在')
    else:
        # x 可能为负数，通过 % phi 将其转为正整数
        return x % phi

# 素性检验 (用于生成大素数)
# n: 要检测的数
# k: 检测轮数，轮数越多，是素数的概率越接近 100%
def is_prime(n, k=5): # Miller-Rabin 测试
    if n <= 1: return False
    if n <= 3: return True
    if n % 2 == 0: return False     # 排除偶数
    
    # 找到 r, d 使得 n-1 = 2^r * d
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2
    # 进行 k 轮随机测试
    for _ in range(k):
        a = random.randint(2, n - 2)    # 随机选取一个证据基数
        x = pow(a, d, n)    # 计算 a^d mod n
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)    # 连续平方
            if x == n - 1:
                break
        else:
            return False    # 确定是合数
    return True     # 极大概率是素数

# 生成一个指定位数的随机大素数
def generate_large_prime(keysize=1024):
    while True:
        num = random.getrandbits(keysize)
        num |= (1 << keysize - 1) | 1  # 确保最高位为 1 (保证位数) 且最低位为 1 (保证是奇数)
        if is_prime(num):
            return num

# RSA 密钥生成
def generate_keypair(keysize=1024):
    # 1. 选两个大素数 p, q
    p = generate_large_prime(keysize // 2)  # 每个素数占用一半的位数
    q = generate_large_prime(keysize // 2)  # 2048位密钥需要两个 1024位的素数

    # 2. 计算 n = p * q 和 欧拉函数 φ(n) = (p-1)(q-1)
    n = p * q
    phi = (p - 1) * (q - 1)
    
    # 3. 选择 e (通常固定为 65537)
    e = 65537

    # 4. 计算 d (e 关于 phi 的模逆)
    d = mod_inverse(e, phi)
    
    # 返回 ((公钥e, 模数n), (私钥d, 模数n))
    return ((e, n), (d, n))

# 加密公式：C = M^e mod n
def encrypt(public_key, plaintext):
    e, n = public_key
    # 将字符串明文编码为字节，再转换为一个大的整数 M
    m = int.from_bytes(plaintext.encode('utf-8'), byteorder='big')

    # # RSA 要求明文数值 M 必须小于模数 n
    if m >= n:
        raise ValueError("明文太长，超过模数 n")
    
    # C = M^e mod n
    c = pow(m, e, n)
    return c

# 解密公式：M = C^d mod n
def decrypt(private_key, ciphertext):
    d, n = private_key
    # M = C^d mod n
    m = pow(ciphertext, d, n)
    # 将大整数 M 转换回字节，再解码为字符串
    bytes_len = (m.bit_length() + 7) // 8
    try:
        return m.to_bytes(bytes_len, byteorder='big').decode('utf-8')
    except Exception:
        return "解密失败或编码错误"


if __name__ == "__main__":
    print("正在生成 2048 位 RSA 密钥对...")
    pub_key, priv_key = generate_keypair(2048)
    print(f"公钥 (e, n): ({pub_key[0]}, {hex(pub_key[1])[:30]}...)")
    
    msg = "Hello, this is a RSA test!"
    print(f"\n原始消息: {msg}")
    
    # 加密
    c = encrypt(pub_key, msg)
    print(f"加密后的密文: {hex(c)[:50]}...")
    
    # 解密
    m_decrypted = decrypt(priv_key, c)
    print(f"解密后的明文: {m_decrypted}")

    if msg == m_decrypted:
        print("\n结论：加解密成功，结果一致！")