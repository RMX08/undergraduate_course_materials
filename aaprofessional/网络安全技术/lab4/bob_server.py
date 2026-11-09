import socket
from cryptography.hazmat.primitives import hashes, serialization, hmac
from cryptography.hazmat.primitives.asymmetric import x25519, ed25519
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import os

def bob_server():
    # 1. 初始化 Bob 的密钥系统
    # 用于密钥交换的 X25519
    bob_sk_x = x25519.X25519PrivateKey.generate()
    bob_pk_x = bob_sk_x.public_key()
    # 用于身份验证的 Ed25519
    bob_sk_ed = ed25519.Ed25519PrivateKey.generate()
    bob_pk_ed = bob_sk_ed.public_key()

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 9999))
    server.listen(1)
    print("Bob 正在等待 Alice 连接...")

    conn, addr = server.accept()
    print(f"连接来自: {addr}")

    try:
        # 2. 交换公钥 (接收 Alice 的公钥)
        alice_pk_x_bytes = conn.recv(32)
        alice_pk_ed_bytes = conn.recv(32)
        
        # 发送 Bob 的公钥
        conn.send(bob_pk_x.public_bytes(serialization.Encoding.Raw, serialization.PublicFormat.Raw))
        conn.send(bob_pk_ed.public_bytes(serialization.Encoding.Raw, serialization.PublicFormat.Raw))

        # 3. Diffie-Hellman 密钥协商
        alice_pk_x = x25519.X25519PublicKey.from_public_bytes(alice_pk_x_bytes)
        shared_key = bob_sk_x.exchange(alice_pk_x)
        
        # 使用 HKDF 派生出两个密钥：k(用于加密) 和 k'(用于HMAC)
        derived_key = HKDF(
            algorithm=hashes.SHA256(), 
            length=64, # 直接派生 64 字节
            salt=None, 
            info=b'handshake data'
        ).derive(shared_key)
        
        # 前 32 字节给 k，后 32 字节给 k'
        k_enc = derived_key[:32]
        k_hmac = derived_key[32:]
        print(f"协商成功！\nAES密钥 k: {k_enc.hex()[:10]}...\nHMAC密钥 k': {k_hmac.hex()[:10]}...")

        # 6. 接收并验证解密
        data = conn.recv(4096)
        # 接收格式: IV(16B) + Ciphertext(可变) + Signature(64B)
        iv = data[:16]
        sig = data[-64:]
        ciphertext = data[16:-64]

        # 验证 Alice 的签名
        alice_pk_ed = ed25519.Ed25519PublicKey.from_public_bytes(alice_pk_ed_bytes)
        try:
            alice_pk_ed.verify(sig, ciphertext)
            print(" 签名验证通过！消息确实来自 Alice 且密文未被篡改。")
            
            # 解密得到 (M + HMAC)
            cipher = Cipher(algorithms.AES(k_enc), modes.CFB(iv))
            decryptor = cipher.decryptor()
            decrypted_payload = decryptor.update(ciphertext) + decryptor.finalize()
            
            # 拆分 M 和 HMAC (HMAC-SHA256 固定 32 字节)
            M = decrypted_payload[:-32]
            received_hmac = decrypted_payload[-32:]
            
            # 验证 HMAC 
            h = hmac.HMAC(k_hmac, hashes.SHA256())
            h.update(M)
            try:
                h.verify(received_hmac)
                print(" HMAC 验证成功：明文完整性校验通过。")
                print(f" 最终解密消息: {M.decode('utf-8')}")
            except Exception:
                print(" 错误：HMAC 验证失败，明文可能被篡改！")
            
        except Exception as e:
            print(f"验证或解密失败: {e}")

    finally:
        conn.close()
        server.close()

if __name__ == "__main__":
    bob_server()