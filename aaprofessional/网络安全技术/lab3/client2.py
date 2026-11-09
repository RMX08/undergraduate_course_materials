import socket
import base64
from cryptography.hazmat.primitives.asymmetric import x25519, ed25519
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.fernet import Fernet
from cryptography.hazmat.primitives import serialization


def start_secure_client(server_ip):
    # 1. 加载身份密钥
    with open("client_identity.priv", "rb") as f:
        my_sign_priv = ed25519.Ed25519PrivateKey.from_private_bytes(f.read())
    with open("server_identity.pub", "rb") as f:
        peer_sign_pub = ed25519.Ed25519PublicKey.from_public_bytes(f.read())

    # 2. 准备 DH
    my_dh_priv = x25519.X25519PrivateKey.generate()
    my_dh_pub_bytes = my_dh_priv.public_key().public_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PublicFormat.Raw
    )
    signature = my_sign_priv.sign(my_dh_pub_bytes)

    # 3. 连接
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((server_ip, 65432))

    # 4. 发送 [公钥 + 签名]
    s.sendall(my_dh_pub_bytes + signature)

    # 5. 接收服务端的 [公钥 + 签名]
    data = s.recv(96)
    server_dh_pub_bytes = data[:32]
    server_sig = data[32:]

    # 6. 验证服务端签名
    try:
        peer_sign_pub.verify(server_sig, server_dh_pub_bytes)
        print("身份验证成功：公钥确实来自服务端！")
    except:
        print("异常：服务端签名错误！")
        return

    # 7. 派生对称密钥
    shared_secret = my_dh_priv.exchange(x25519.X25519PublicKey.from_public_bytes(server_dh_pub_bytes))
    derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'file-transfer').derive(shared_secret)
    cipher = Fernet(base64.urlsafe_b64encode(derived_key))

    # 8. 读取文件、加密并发送
    file_to_send = "test.txt"  # 请确保本地有一个 test.txt
    with open(file_to_send, "rb") as f:
        content = f.read()

    encrypted_content = cipher.encrypt(content)
    s.sendall(encrypted_content)
    print(f"文件 {file_to_send} 已加密发送。")
    s.close()


if __name__ == "__main__":
    ip = '192.168.126.128'
    start_secure_client(ip)