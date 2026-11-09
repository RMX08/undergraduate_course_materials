import numpy as np

# 判断给顶点 v 涂上颜色 c 是否合法。
# v: 当前尝试涂色的顶点 color: 当前着色情况
def judeg_A(v, A, color, c):
    # 遍历，查找邻居
    for i in range(len(A)):
        if A[v][i] == 1 and color[i] == c:   # 邻居
            return False
    return True

# 为顶点 v 及其之后的顶点涂色，并返回合法方案总数。
# v：当前顶点 n：总顶点 k：可用颜色数
def count_schemes(v, n, k, A, color):
    if v == n:      #  0 到 n-1 的顶点都已经成功涂色了
        return 1

    count = 0
    # 为当前顶点 v 分配每一种可能的颜色
    for c in range(1, k + 1):
        # 检查是否与已涂色的邻居冲突
        if judeg_A(v, A, color, c):
            color[v] = c    # 如果不冲突，就把颜色 c 分配给顶点 v

            count += count_schemes(v + 1, n, k, A, color) # 给下一个顶点 (v + 1) 涂色

            color[v] = 0    # 回溯

    return count

# 主程序
def main():
    print("======== 无线基站频率分配（图点着色问题） ========")

    # 输入数据
    try:
        n = int(input("请输入顶点数 (n <= 10): "))
        k = int(input("请输入频段数 (k <= 4): "))

        print(f"请输入 {n}x{n} 的邻接矩阵: ")
        matrix = []
        for i in range(n):
            row_str = input(f"第 {i + 1} 行: ").split()  # 按照空格来分割字符
            row_int = [int(x) for x in row_str]  # 转换为int
            # 输入判断
            if len(row_int) != n:
                print("该行输入错误！！请重新输入！")
                i -= 1
                continue
            matrix.append(row_int)

        # 使用 numpy 将列表转换为矩阵格式，方便处理
        A = np.array(matrix)

    except ValueError:
        print("输入格式有误，请输入整数。")
        return

    node_color = [0] * n        # 记录每个顶点的颜色

    # 从第 0 个顶点开始递归计算
    sum = count_schemes(0, n, k, A, node_color)

    # 4. 输出结果
    print("\n======== 实验结果 ========")
    if sum > 0:
        print(f"可以使用{k}种颜色完成频段分配")
    else:
        print(f"不能使用{k}种颜色完成频段分配")

    print(f"所有合法的频段分配方案数为：{sum}")

if __name__ == "__main__":
    main()