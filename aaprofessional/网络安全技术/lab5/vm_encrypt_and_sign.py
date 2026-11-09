# vm_encrypt_and_sign.py

import os

from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.ciphers.aead import AESGCM


VM_DIR = "lab5_vm"
OUTPUT_DIR = "send_to_host"

os.makedirs(OUTPUT_DIR, exist_ok=True)


def main():
    # ==============================
    # 1. 读取虚拟机私钥
    # ==============================

    with open(os.path.join(VM_DIR, "vm_private_key.pem"), "rb") as f:
        vm_private_key = serialization.load_pem_private_key(
            f.read(),
            password=None
        )

    print("[1] 虚拟机已读取自己的私钥 vm_private_key.pem")

    # ==============================
    # 2. 读取并解密 AES 会话密钥
    # ==============================

    with open(os.path.join(VM_DIR, "session_key.enc"), "rb") as f:
        encrypted_session_key = f.read()

    # 使用虚拟机私钥解密
    # 这里必须使用与物理机加密时相同的 OAEP 参数
    session_key = vm_private_key.decrypt(
        encrypted_session_key,
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )

    print("[2] 虚拟机已使用自己的私钥解密 AES 会话密钥")

    # 保存解密后的 AES 密钥，便于实验截图
    with open(os.path.join(OUTPUT_DIR, "session_key_decrypted.bin"), "wb") as f:
        f.write(session_key)

    # ==============================
    # 3. 准备需要加密的文件
    # ==============================

    # 如果 secret.txt 不存在，就自动创建一个测试文件
    secret_file_path = os.path.join(VM_DIR, "secret.txt")

    if not os.path.exists(secret_file_path):
        with open(secret_file_path, "w", encoding="utf-8") as f:
            f.write("这是来自虚拟机的机密文件内容。\n")
            f.write("本文件将使用 AES 会话密钥加密，并使用虚拟机私钥签名。\n")

    with open(secret_file_path, "rb") as f:
        plaintext = f.read()

    print("[3] 虚拟机已读取待加密文件 secret.txt")

    # ==============================
    # 4. 使用 AES-GCM 加密文件
    # ==============================

    # AESGCM 需要一个 12 字节的 nonce
    # nonce 不需要保密，但同一个密钥下不能重复
    nonce = os.urandom(12)

    aesgcm = AESGCM(session_key)

    # encrypt 参数说明：
    # nonce：随机数
    # plaintext：明文
    # associated_data：附加认证数据，这里不使用，所以为 None
    ciphertext = aesgcm.encrypt(
        nonce,
        plaintext,
        None
    )

    print("[4] 虚拟机已使用 AES-GCM 加密文件")

    # 保存 nonce 和密文
    with open(os.path.join(OUTPUT_DIR, "nonce.bin"), "wb") as f:
        f.write(nonce)

    with open(os.path.join(OUTPUT_DIR, "secret.enc"), "wb") as f:
        f.write(ciphertext)

    # ==============================
    # 5. 使用虚拟机私钥对密文签名
    # ==============================

    # 签名的对象是密文 secret.enc
    # 这样可以证明密文确实由虚拟机发送，且传输过程中没有被篡改
    signature = vm_private_key.sign(
        ciphertext,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )

    print("[5] 虚拟机已使用自己的私钥对密文进行数字签名")

    with open(os.path.join(OUTPUT_DIR, "secret.sig"), "wb") as f:
        f.write(signature)

    # ==============================
    # 6. 复制虚拟机证书
    # ==============================

    # 物理机需要虚拟机证书来验证：
    # 1. 证书是否由物理机 CA 签发；
    # 2. 签名是否来自虚拟机私钥。
    with open(os.path.join(VM_DIR, "vm_cert.pem"), "rb") as f:
        vm_cert_data = f.read()

    with open(os.path.join(OUTPUT_DIR, "vm_cert.pem"), "wb") as f:
        f.write(vm_cert_data)

    print("[6] 已准备好发送给物理机的文件")
    print("\n请将 send_to_host 目录中的以下文件发送回物理机：")
    print("1. nonce.bin")
    print("2. secret.enc")
    print("3. secret.sig")
    print("4. vm_cert.pem")


if __name__ == "__main__":
    main()