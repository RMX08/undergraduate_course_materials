import socket
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import x25519


def start_dh_server():
    # --- 1. 生成服务端自己的密钥对 ---
    # 生成私钥
    server_private_key = x25519.X25519PrivateKey.generate()
    # 导出公钥
    server_public_key = server_private_key.public_key()

    # 将公钥对象转换为“字节流”，这样才能通过网络发送
    server_pub_bytes = server_public_key.public_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PublicFormat.Raw
    )

    # --- 2. 设置网络连接 (Socket) ---
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 绑定 0.0.0.0 表示监听虚拟机所有的网卡，端口选一个没被占用的，如 65432
    s.bind(('0.0.0.0', 65432))
    s.listen(1)
    print("等待物理机(客户端)连接...")

    conn, addr = s.accept()
    with conn:
        print(f"连接成功！来自: {addr}")

        # --- 3. 交换公钥 ---
        # 接收客户端发来的公钥 (X25519 公钥固定 32 字节)
        client_pub_bytes = conn.recv(32)
        # 将接收到的字节转回公钥对象
        client_public_key = x25519.X25519PublicKey.from_public_bytes(client_pub_bytes)

        # 发送服务端的公钥给客户端
        conn.sendall(server_pub_bytes)

        # --- 4. 计算共享密钥 ---
        # 核心步骤：使用“自己的私钥” + “对方的公钥”
        shared_key = server_private_key.exchange(client_public_key)

        print("\n[结果] 密钥协商成功！")
        print(f"协商出的共享密钥 (Hex): {shared_key.hex()}")


if __name__ == "__main__":
    start_dh_server()