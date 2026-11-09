import tkinter as tk
from tkinter import messagebox
import cv2
import dlib
import pandas as pd
import numpy as np
import time
from PIL import Image


# =========================
# 1. 用户信息配置
# =========================

USERS = {
    "yolanda": {
        "password": "123456",
        "face_name": "yolanda"
    }
}

FEATURES_CSV_PATH = "data/features_all.csv"
PREDICTOR_PATH = "data/data_dlib/shape_predictor_68_face_landmarks.dat"
FACE_RECO_MODEL_PATH = "data/data_dlib/dlib_face_recognition_resnet_model_v1.dat"

FACE_RECO_THRESHOLD = 0.45


# =========================
# 2. 初始化 Dlib 模型
# =========================

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(PREDICTOR_PATH)
face_reco_model = dlib.face_recognition_model_v1(FACE_RECO_MODEL_PATH)


# =========================
# 3. 密码认证
# =========================

def check_password(username, password):
    if username not in USERS:
        return False
    return USERS[username]["password"] == password


# =========================
# 4. 读取人脸特征库
# =========================

def load_face_database():
    """
    读取 data/features_all.csv
    返回：
    names: 人名列表
    features: 对应的 128 维特征列表
    """
    data = pd.read_csv(FEATURES_CSV_PATH, header=None, encoding="utf-8")

    names = []
    features = []

    for i in range(data.shape[0]):
        name = str(data.iloc[i, 0])
        feature = data.iloc[i, 1:].astype(float).values

        names.append(name)
        features.append(feature)

    return names, features


# =========================
# 5. 计算欧氏距离
# =========================

def euclidean_distance(feature_1, feature_2):
    return np.linalg.norm(feature_1 - feature_2)


# =========================
# 6. 人脸认证核心函数
# =========================

def face_auth(expected_name, timeout_seconds=10):
    """
    打开摄像头进行人脸认证。
    如果在 timeout_seconds 秒内识别到 expected_name，则返回 True。
    否则返回 False。
    """

    names, features = load_face_database()

    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        messagebox.showerror("错误", "无法打开摄像头")
        return False

    start_time = time.time()
    success_count = 0

    while True:
        ret, frame = cap.read()

        if not ret:
            break

        faces = detector(frame, 1)

        for face in faces:
            shape = predictor(frame, face)
            current_feature = np.array(
                face_reco_model.compute_face_descriptor(frame, shape)
            )

            min_distance = 999
            matched_name = "unknown"

            for i, known_feature in enumerate(features):
                distance = euclidean_distance(current_feature, known_feature)

                if distance < min_distance:
                    min_distance = distance
                    matched_name = names[i]

            if min_distance < FACE_RECO_THRESHOLD:
                display_name = matched_name
            else:
                display_name = "unknown"

            # 在画面上画框
            left = face.left()
            top = face.top()
            right = face.right()
            bottom = face.bottom()

            cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)
            cv2.putText(
                frame,
                str(display_name),
                (left, top - 10),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.8,
                (0, 255, 0),
                2
            )

            # 判断是否识别到目标用户
            if display_name == expected_name:
                success_count += 1
            else:
                success_count = 0

            # 连续 3 帧识别成功，才认为认证通过
            if success_count >= 3:
                cap.release()
                cv2.destroyAllWindows()
                return True

        cv2.imshow("Face Authentication - Press Q to quit", frame)

        # 按 q 手动退出
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

        # 超时退出
        if time.time() - start_time > timeout_seconds:
            break

    cap.release()
    cv2.destroyAllWindows()
    return False


# =========================
# 7. 登录按钮事件
# =========================

def login():
    username = entry_username.get().strip()
    password = entry_password.get().strip()

    if username == "" or password == "":
        messagebox.showwarning("提示", "请输入用户名和密码")
        return

    if not check_password(username, password):
        messagebox.showerror("登录失败", "用户名或密码错误")
        return

    messagebox.showinfo("密码认证成功", "密码正确，接下来进行人脸识别")

    expected_face_name = USERS[username]["face_name"]

    if face_auth(expected_face_name):
        messagebox.showinfo("登录成功", "双因子认证成功，登录系统")
        label_status.config(text="状态：登录成功")
    else:
        messagebox.showerror("登录失败", "人脸认证失败")
        label_status.config(text="状态：人脸认证失败")


# =========================
# 8. Tkinter 登录界面
# =========================

window = tk.Tk()
window.title("双因子认证登录系统")
window.geometry("360x240")

label_title = tk.Label(window, text="双因子认证登录系统", font=("Microsoft YaHei", 16))
label_title.pack(pady=15)

frame_username = tk.Frame(window)
frame_username.pack(pady=5)

label_username = tk.Label(frame_username, text="用户名：", font=("Microsoft YaHei", 11))
label_username.pack(side=tk.LEFT)

entry_username = tk.Entry(frame_username, width=25)
entry_username.pack(side=tk.LEFT)

frame_password = tk.Frame(window)
frame_password.pack(pady=5)

label_password = tk.Label(frame_password, text="密  码：", font=("Microsoft YaHei", 11))
label_password.pack(side=tk.LEFT)

entry_password = tk.Entry(frame_password, width=25, show="*")
entry_password.pack(side=tk.LEFT)

button_login = tk.Button(window, text="登录", width=15, command=login)
button_login.pack(pady=15)

label_status = tk.Label(window, text="状态：等待登录", font=("Microsoft YaHei", 10))
label_status.pack(pady=5)

window.mainloop()