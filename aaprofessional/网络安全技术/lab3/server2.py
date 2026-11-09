import socket
import base64
from cryptography.hazmat.primitives.asymmetric import x25519, ed25519
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.fernet import Fernet
from cryptography.hazmat.primitives import serialization


def start_secure_server():
    # 1. 加载身份密钥 (用于签名和验签)
    with open("server_identity.priv", "rb") as f:
        my_sign_priv = ed25519.Ed25519PrivateKey.from_private_bytes(f.read())
    with open("client_identity.pub", "rb") as f:
        peer_sign_pub = ed25519.Ed25519PublicKey.from_public_bytes(f.read())

    # 2. 准备 DH 密钥交换
    my_dh_priv = x25519.X25519PrivateKey.generate()
    my_dh_pub_bytes = my_dh_priv.public_key().public_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PublicFormat.Raw
    )

    # 核心：对自己的 DH 公钥进行数字签名
    signature = my_sign_priv.sign(my_dh_pub_bytes)

    # 3. Socket 连接
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('0.0.0.0', 65432))
    s.listen(1)
    print("安全服务端已启动，等待连接...")
    conn, addr = s.accept()

    with conn:
        # 4. 接收客户端的 [公钥(32字节) + 签名(64字节)]
        data = conn.recv(96)
        client_dh_pub_bytes = data[:32]
        client_sig = data[32:]

        # 验证客户端签名
        try:
            peer_sign_pub.verify(client_sig, client_dh_pub_bytes)
            print("身份验证成功：公钥确实来自客户端！")
        except:
            print("警告：身份验证失败！可能是中间人攻击！")
            return

        # 5. 发送自己的 [公钥 + 签名]
        conn.sendall(my_dh_pub_bytes + signature)

        # 6. 计算共享密钥并使用 HKDF 派生对称密钥
        shared_secret = my_dh_priv.exchange(x25519.X25519PublicKey.from_public_bytes(client_dh_pub_bytes))

        # HKDF 转换 (将 DH 结果变成 32 字节的 AES 密钥)
        derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'file-transfer').derive(shared_secret)
        # Fernet 库需要 base64 格式的密钥
        fernet_key = base64.urlsafe_b64encode(derived_key)
        cipher = Fernet(fernet_key)

        # 7. 接收加密的文件内容
        print("等待接收加密文件...")
        encrypted_data = conn.recv(4096)
        decrypted_data = cipher.decrypt(encrypted_data)

        with open("received_file.txt", "wb") as f:
            f.write(decrypted_data)
        print("文件接收成功并已解密为 'received_file.txt'")


if __name__ == "__main__":
    start_secure_server()