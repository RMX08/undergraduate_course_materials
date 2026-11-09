# host_encrypt_session_key.py

import os

from cryptography import x509
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import padding


HOST_DIR = "host_files"
VM_DIR = "vm_files"


def main():
    # ==============================
    # 1. 生成 AES 会话密钥
    # ==============================

    # AES-256 需要 32 字节密钥
    # os.urandom 可以生成安全随机数
    session_key = os.urandom(32)

    # 保存明文 AES 密钥到物理机本地
    # 注意：真实系统中不能随意保存明文密钥，这里是为了实验验证
    with open(os.path.join(HOST_DIR, "session_key.bin"), "wb") as f:
        f.write(session_key)

    print("[1] 物理机已生成 AES-256 会话密钥")

    # ==============================
    # 2. 读取虚拟机证书
    # ==============================

    with open(os.path.join(VM_DIR, "vm_cert.pem"), "rb") as f:
        vm_cert_data = f.read()

    # 加载 X.509 证书对象
    vm_cert = x509.load_pem_x509_certificate(vm_cert_data)

    print("[2] 已读取虚拟机证书 vm_cert.pem")

    # ==============================
    # 3. 从虚拟机证书中提取公钥
    # ==============================

    # 证书中包含虚拟机的公钥
    # 物理机使用这个公钥加密 AES 会话密钥
    vm_public_key = vm_cert.public_key()

    print("[3] 已从虚拟机证书中提取虚拟机公钥")

    # ==============================
    # 4. 使用虚拟机公钥加密 AES 会话密钥
    # ==============================

    encrypted_session_key = vm_public_key.encrypt(
        session_key,

        # OAEP 是比旧版 PKCS#1 v1.5 更安全的 RSA 填充方案
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )

    print("[4] 已使用虚拟机公钥加密 AES 会话密钥")

    # ==============================
    # 5. 保存加密后的会话密钥
    # ==============================

    with open(os.path.join(VM_DIR, "session_key.enc"), "wb") as f:
        f.write(encrypted_session_key)

    print("[5] 加密后的会话密钥已保存为 vm_files/session_key.enc")
    print("\n请将以下文件传输到虚拟机：")
    print("1. vm_private_key.pem")
    print("2. vm_cert.pem")
    print("3. ca_cert.pem")
    print("4. session_key.enc")


if __name__ == "__main__":
    main()