import socket
from cryptography.hazmat.primitives import hashes, serialization,hmac
from cryptography.hazmat.primitives.asymmetric import x25519, ed25519
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import os

def alice_client():
    # 初始化 Alice 的密钥系统
    alice_sk_x = x25519.X25519PrivateKey.generate()
    alice_pk_x = alice_sk_x.public_key()
    alice_sk_ed = ed25519.Ed25519PrivateKey.generate()
    alice_pk_ed = alice_sk_ed.public_key()

    # 创建网络连接到 Bob，使用IPv4地址族和 TCP 协议
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Bob的 IP 地址
    bob_ip = 'lab' 
    client.connect((bob_ip, 9999))

    # 交换公钥 (发送 Alice 的公钥) 按照标准Raw格式转换为 bytes
    client.send(alice_pk_x.public_bytes(serialization.Encoding.Raw, serialization.PublicFormat.Raw))
    client.send(alice_pk_ed.public_bytes(serialization.Encoding.Raw, serialization.PublicFormat.Raw))
    
    # 接收 Bob 的公钥
    bob_pk_x_bytes = client.recv(32)
    bob_pk_ed_bytes = client.recv(32)

    # Diffie-Hellman 密钥协商
    bob_pk_x = x25519.X25519PublicKey.from_public_bytes(bob_pk_x_bytes)
    shared_key = alice_sk_x.exchange(bob_pk_x)
    
    # 使用 HKDF 派生出两个密钥：k(用于加密) 和 k'(用于HMAC)
    derived_key = HKDF(
        algorithm=hashes.SHA256(), 
        length=64, 
        salt=None, 
        info=b'handshake data'
    ).derive(shared_key)
    k_enc = derived_key[:32]    # 对称加密密钥 k
    k_hmac = derived_key[32:]   # HMAC 密钥 k'
    print(f"协商成功！\nAES密钥 k: {k_enc.hex()[:10]}...\nHMAC密钥 k': {k_hmac.hex()[:10]}...")

    # 准备数据 M 并计算 HMAC 
    M = b"Hello Bob, this is a secure message from Alice!"
    h = hmac.HMAC(k_hmac, hashes.SHA256())
    h.update(M)
    hmac_tag = h.finalize()  # 得到 HMACk'(M)
    
    # 对称加密 C = Ek(M + HMAC) 
    iv = os.urandom(16)
    payload = M + hmac_tag # 拼接明文和HMAC
    cipher = Cipher(algorithms.AES(k_enc), modes.CFB(iv))
    encryptor = cipher.encryptor()
    C = encryptor.update(payload) + encryptor.finalize()

    # Alice 对 C 签名得到 Sig
    sig = alice_sk_ed.sign(C)

    # Alice 发送 (IV + C + Sig) 给 Bob
    # 将 IV, 密文, 签名拼接发送
    packet = iv + C + sig
    client.send(packet)
    print("加密数据和签名已发送。")

    client.close()

if __name__ == "__main__":
    alice_client()