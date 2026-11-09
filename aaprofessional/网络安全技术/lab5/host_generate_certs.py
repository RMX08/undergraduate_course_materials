# host_generate_certs.py

import os
from datetime import datetime, timedelta, timezone

from OpenSSL import crypto


# ==============================
# 1. 创建实验目录
# ==============================

# host_files 用来保存物理机自己的 CA 私钥和 CA 证书
HOST_DIR = "host_files"

# vm_files 用来保存将要下发给虚拟机的私钥和证书
VM_DIR = "vm_files"

os.makedirs(HOST_DIR, exist_ok=True)
os.makedirs(VM_DIR, exist_ok=True)


# ==============================
# 2. 生成 RSA 密钥函数
# ==============================

def generate_rsa_key():
    """
    生成 RSA 私钥。
    
    RSA 是非对称加密算法。
    每个 RSA 密钥对包含：
    1. 私钥：自己保存，不能泄露；
    2. 公钥：可以公开，用于加密或验证签名。
    
    这里使用 2048 位 RSA，满足一般实验要求。
    """
    key = crypto.PKey()
    key.generate_key(crypto.TYPE_RSA, 2048)
    return key


# ==============================
# 3. 生成 CA 自签名证书
# ==============================

def create_ca_certificate(ca_key):
    """
    创建物理机自己的 CA 证书。
    
    CA 证书是自签名证书：
    也就是证书的签发者和证书的拥有者是同一个。
    
    本实验中，物理机充当 CA，所以物理机自己给自己签发证书。
    """
    cert = crypto.X509()

    # 设置证书序列号，序列号用于区分不同证书
    cert.set_serial_number(1001)

    # 设置证书主题，也就是证书拥有者的信息
    subject = cert.get_subject()
    subject.C = "CN"                 # 国家
    subject.ST = "Beijing"           # 省份
    subject.L = "Beijing"            # 城市
    subject.O = "NetworkSecurityLab" # 组织名称
    subject.OU = "CA"                # 部门
    subject.CN = "Host-CA"           # Common Name，证书名称

    # 设置证书有效期
    # notBefore 表示从现在开始生效
    cert.gmtime_adj_notBefore(0)

    # notAfter 表示 365 天后过期
    cert.gmtime_adj_notAfter(365 * 24 * 60 * 60)

    # 设置证书签发者
    # 因为是自签名证书，所以 issuer 和 subject 相同
    cert.set_issuer(subject)

    # 把 CA 的公钥放进证书中
    cert.set_pubkey(ca_key)

    # 设置 CA 扩展，表明这个证书可以作为 CA 使用
    cert.add_extensions([
        crypto.X509Extension(b"basicConstraints", True, b"CA:TRUE"),
        crypto.X509Extension(b"keyUsage", True, b"keyCertSign, cRLSign"),
        crypto.X509Extension(b"subjectKeyIdentifier", False, b"hash", subject=cert)
    ])

    # 使用 CA 自己的私钥对证书签名
    # sha256 表示签名时使用 SHA-256 摘要算法
    cert.sign(ca_key, "sha256")

    return cert


# ==============================
# 4. 生成虚拟机证书
# ==============================

def create_vm_certificate(vm_key, ca_key, ca_cert):
    """
    创建虚拟机证书。
    
    虚拟机证书不是自签名证书。
    它由物理机 CA 签发。
    
    也就是说：
    证书拥有者：VM
    证书签发者：Host-CA
    签名私钥：ca_key
    """
    cert = crypto.X509()

    # 设置虚拟机证书序列号
    cert.set_serial_number(2001)

    # 设置虚拟机证书主题信息
    subject = cert.get_subject()
    subject.C = "CN"
    subject.ST = "Beijing"
    subject.L = "Beijing"
    subject.O = "NetworkSecurityLab"
    subject.OU = "VM"
    subject.CN = "Virtual-Machine"

    # 设置证书有效期
    cert.gmtime_adj_notBefore(0)
    cert.gmtime_adj_notAfter(365 * 24 * 60 * 60)

    # 设置签发者为 CA 证书的主题
    # 这表示 vm_cert.pem 是由 Host-CA 签发的
    cert.set_issuer(ca_cert.get_subject())

    # 把虚拟机的公钥放入虚拟机证书
    cert.set_pubkey(vm_key)

    # 设置证书用途
    cert.add_extensions([
        crypto.X509Extension(b"basicConstraints", True, b"CA:FALSE"),
        crypto.X509Extension(b"keyUsage", True, b"digitalSignature, keyEncipherment"),
        crypto.X509Extension(b"extendedKeyUsage", False, b"clientAuth, serverAuth")
    ])

    # 使用物理机 CA 私钥对虚拟机证书签名
    cert.sign(ca_key, "sha256")

    return cert


# ==============================
# 5. 保存密钥和证书
# ==============================

def save_private_key(path, key):
    """
    保存私钥到 PEM 文件。
    
    PEM 是常见的密钥和证书保存格式。
    """
    with open(path, "wb") as f:
        f.write(
            crypto.dump_privatekey(
                crypto.FILETYPE_PEM,
                key
            )
        )


def save_certificate(path, cert):
    """
    保存证书到 PEM 文件。
    """
    with open(path, "wb") as f:
        f.write(
            crypto.dump_certificate(
                crypto.FILETYPE_PEM,
                cert
            )
        )


# ==============================
# 6. 主函数
# ==============================

def main():
    print("[1] 正在生成物理机 CA 私钥...")
    ca_key = generate_rsa_key()

    print("[2] 正在生成物理机 CA 自签名证书...")
    ca_cert = create_ca_certificate(ca_key)

    print("[3] 正在生成虚拟机 VM 私钥...")
    vm_key = generate_rsa_key()

    print("[4] 正在使用物理机 CA 私钥签发虚拟机证书...")
    vm_cert = create_vm_certificate(vm_key, ca_key, ca_cert)

    print("[5] 正在保存文件...")

    # 保存物理机 CA 私钥和 CA 证书
    save_private_key(os.path.join(HOST_DIR, "ca_private_key.pem"), ca_key)
    save_certificate(os.path.join(HOST_DIR, "ca_cert.pem"), ca_cert)

    # 保存虚拟机私钥和虚拟机证书
    save_private_key(os.path.join(VM_DIR, "vm_private_key.pem"), vm_key)
    save_certificate(os.path.join(VM_DIR, "vm_cert.pem"), vm_cert)

    # 虚拟机也需要 CA 证书，用于之后知道谁是可信 CA
    save_certificate(os.path.join(VM_DIR, "ca_cert.pem"), ca_cert)

    print("\n证书和密钥生成完成！")
    print("物理机文件保存在 host_files 目录中")
    print("虚拟机文件保存在 vm_files 目录中")
    print("\n需要下发给虚拟机的文件：")
    print("1. vm_private_key.pem")
    print("2. vm_cert.pem")
    print("3. ca_cert.pem")


if __name__ == "__main__":
    main()