from cryptography.hazmat.primitives.asymmetric import ed25519
from cryptography.hazmat.primitives import serialization

# 1. 生成服务端的身份密钥（私钥和公钥）
s_priv = ed25519.Ed25519PrivateKey.generate()
s_pub = s_priv.public_key()

# 2. 生成客户端的身份密钥（私钥和公钥）
c_priv = ed25519.Ed25519PrivateKey.generate()
c_pub = c_priv.public_key()

# 3. 将这些密钥保存到当前文件夹
with open("server_identity.priv", "wb") as f:
    f.write(s_priv.private_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PrivateFormat.Raw,
        encryption_algorithm=serialization.NoEncryption()
    ))

with open("server_identity.pub", "wb") as f:
    f.write(s_pub.public_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PublicFormat.Raw
    ))

with open("client_identity.priv", "wb") as f:
    f.write(c_priv.private_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PrivateFormat.Raw,
        encryption_algorithm=serialization.NoEncryption()
    ))

with open("client_identity.pub", "wb") as f:
    f.write(c_pub.public_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PublicFormat.Raw
    ))

print("身份密钥文件已成功生成！")