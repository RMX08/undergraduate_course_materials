# host_verify_and_decrypt.py

import os

from cryptography import x509
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding, rsa
from cryptography.hazmat.primitives.ciphers.aead import AESGCM


HOST_DIR = "host_files"
RECEIVED_DIR = "received_from_vm"


def verify_certificate_by_ca(vm_cert, ca_cert):
    """
    验证虚拟机证书是否由物理机 CA 签发。
    
    验证思路：
    1. 取出 CA 证书中的公钥；
    2. 使用 CA 公钥验证 VM 证书上的签名；
    3. 如果验证成功，说明 VM 证书确实由 CA 私钥签发。
    """

    ca_public_key = ca_cert.public_key()

    # VM 证书中有一个 signature 字段
    # 这个 signature 是 CA 私钥对 VM 证书主体信息签出来的
    # 现在使用 CA 公钥进行验证
    ca_public_key.verify(
        vm_cert.signature,
        vm_cert.tbs_certificate_bytes,
        padding.PKCS1v15(),
        vm_cert.signature_hash_algorithm
    )


def main():
    # ==============================
    # 1. 读取物理机 CA 证书
    # ==============================

    with open(os.path.join(HOST_DIR, "ca_cert.pem"), "rb") as f:
        ca_cert = x509.load_pem_x509_certificate(f.read())

    print("[1] 物理机已读取自己的 CA 证书 ca_cert.pem")

    # ==============================
    # 2. 读取虚拟机发来的证书
    # ==============================

    with open(os.path.join(RECEIVED_DIR, "vm_cert.pem"), "rb") as f:
        vm_cert = x509.load_pem_x509_certificate(f.read())

    print("[2] 物理机已读取虚拟机发来的 vm_cert.pem")

    # ==============================
    # 3. 验证虚拟机证书是否由物理机 CA 签发
    # ==============================

    try:
        verify_certificate_by_ca(vm_cert, ca_cert)
        print("[3] VM 证书验证成功：该证书确实由物理机 CA 签发")
    except Exception as e:
        print("[3] VM 证书验证失败！")
        print("错误信息：", e)
        return

    # ==============================
    # 4. 从虚拟机证书中提取公钥
    # ==============================

    vm_public_key = vm_cert.public_key()

    print("[4] 已从 VM 证书中提取 VM 公钥")

    # ==============================
    # 5. 读取密文和签名
    # ==============================

    with open(os.path.join(RECEIVED_DIR, "secret.enc"), "rb") as f:
        ciphertext = f.read()

    with open(os.path.join(RECEIVED_DIR, "secret.sig"), "rb") as f:
        signature = f.read()

    print("[5] 已读取虚拟机发来的密文 secret.enc 和签名 secret.sig")

    # ==============================
    # 6. 使用 VM 公钥验证签名
    # ==============================

    try:
        vm_public_key.verify(
            signature,
            ciphertext,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        print("[6] 签名验证成功：密文确实由虚拟机私钥签名，且没有被篡改")
    except Exception as e:
        print("[6] 签名验证失败！密文可能被篡改，或签名不是 VM 私钥生成的")
        print("错误信息：", e)
        return

    # ==============================
    # 7. 读取物理机本地保存的 AES 会话密钥
    # ==============================

    with open(os.path.join(HOST_DIR, "session_key.bin"), "rb") as f:
        session_key = f.read()

    print("[7] 已读取物理机本地保存的 AES 会话密钥")

    # ==============================
    # 8. 读取 nonce 并解密文件
    # ==============================

    with open(os.path.join(RECEIVED_DIR, "nonce.bin"), "rb") as f:
        nonce = f.read()

    aesgcm = AESGCM(session_key)

    try:
        plaintext = aesgcm.decrypt(
            nonce,
            ciphertext,
            None
        )
        print("[8] 文件解密成功")
    except Exception as e:
        print("[8] 文件解密失败！")
        print("错误信息：", e)
        return

    # ==============================
    # 9. 保存解密后的文件
    # ==============================

    output_path = os.path.join(HOST_DIR, "decrypted_secret.txt")

    with open(output_path, "wb") as f:
        f.write(plaintext)

    print("[9] 解密后的文件已保存为：", output_path)
    print("\n解密后的文件内容如下：")
    print(plaintext.decode("utf-8"))


if __name__ == "__main__":
    main()