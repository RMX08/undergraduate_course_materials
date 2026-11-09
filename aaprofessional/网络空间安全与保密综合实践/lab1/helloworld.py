import requests

base_url = "http://10.140.32.159:47336"

# 使用 Session 保存服务器返回的 Cookie
session = requests.Session()

# 访问首页，观察响应
r1 = session.get(base_url)
print("Step 1 状态码:", r1.status_code)
print(r1.text[:500])

# 发送 POST 请求
data = {
    "key": "flag"
}

r2 = session.post(base_url, data=data)
print("Step 2 状态码:", r2.status_code)
print(r2.text)

# 根据第二步源码中看到的条件，填写 flag 参数
value = "sdfcfcvfgfdeeffgdsd"

r3 = session.get(base_url + "/get.php", params={"flag": value})
print("Step 3 状态码:", r3.status_code)
print(r3.text)