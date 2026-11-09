import numpy as np
import networkx as nx
import matplotlib
matplotlib.use('TkAgg') # 或者 'Qt5Agg'，强制指定后端
import matplotlib.pyplot as plt

# # 判断关联矩阵输入是否合法
# def judge_M(M,n,m):
#     for j in range(m):
#         col = M[:,j]
#         non_zero_count = np.count_nonzero(col)
#         positive_count = np.count_nonzero(col > 0)
#         col_sum = np.sum(col)
#
#         if non_zero_count >= 3:
#             print(f"第{j+1}条边（第{j+1}列）对应{non_zero_count}个顶点！\n请重新输入：")
#             return false
#         elif col_sum != 2:
#             print(f"第{j+1}条边（第{j+1}列）列和为{col_sum}!\n请重新输入：")
#             return false
#         else:
#             return True




# 输入关联矩阵
def input_M():
    try:
        n = int(input("请输入顶点数 n："))
        m = int(input("请输入边数 m: "))
        print(f"请逐行输入关联矩阵 (共 {n} 行，每行 {m} 个数字，空格分隔):")

        # while True:
        M = []
        for i in range(n):
            row_str = input(f"第 {i + 1} 行: ").split()  # 按照空格来分割字符
            row_int = [int(x) for x in row_str]   # 转换为int
            # 输入判断
            if len(row_int) != m:
                print("该行输入错误！！请重新输入！")
                i -= 1
                continue
            M.append(row_int)

            # if (judge_M(M,n,m)):
            #     break

        return np.array(M),n,m

    except ValueError:
        print("输入格式错误，请重新输入！")
        return None,0,0

# 生成相邻矩阵 A
def build_A(M,n,m):
    A = np.zeros((n, n), dtype=int)   # 全0初始化
    e_list = []

    # 按边遍历
    for j in range(m):
        nodes = np.where(M[:, j] > 0)[0]  # 找非0行 行号

        if len(nodes) == 2:  # 普通边
            u,v = nodes[0],nodes[1]
            A[u, v] += 1
            A[v, u] += 1
            e_list.append((u, v))

        elif len(nodes) == 1:   # 自环
            u = nodes[0]
            A[u][u] += 1   # 对角线位置 +1
            e_list.append((u, u))


    return A, e_list

# 矩阵基本要素
def analyze(M,A,n):
    print("---- 度数 ----")
    for i in range(n):   # 关联矩阵行和
        d = sum(M[i, :])
        print(f"顶点 v{i + 1} 的度数为: {d}")

    print("---- 邻域 ----")  # 遍历A
    for i in range(n):
        neighbors = []
        for j in range(n):
            if i == j: continue  # 自己
            if A[i][j] > 0:
                neighbors.append(f"v{j + 1}")
        print(f"顶点 v{i + 1} 的邻域: {neighbors}")

    print("---- 是否为简单图 ----")
    ans = []
    # 检查自环
    flag = False
    for i in range(n):
        if A[i][i] > 0:
            flag = True
            break
    if flag:
        ans.append("存在自环！")
    # 检查平行边
    flag = False
    for i in range(n):
        for j in range(i + 1, n):  # 对称,只看上三角
            if A[i][j] > 1:
                flag = True
                break
        if flag:
            break
    if flag:
        ans.append("存在平行边 ")

    if not ans:
        print("结果: 该图是简单图！！")
    else:
        print(f"结果: 不是简单图！！")


    print("---- 是否为连通图 ----") # BFS
    visited = [False] * n
    list = []
    for i in range(n):
        if not visited[i]: # 未被访问新的分支
            nodes = []
            queue = [i]
            visited[i] = True
            while queue:
                curr = queue.pop(0)
                nodes.append(curr)
                # 找所有邻居
                for neighbor in range(n):
                    if A[curr][neighbor] > 0 and not visited[neighbor]:
                        visited[neighbor] = True
                        queue.append(neighbor)
            list.append([f"v{x + 1}" for x in sorted(nodes)])

    print(f"连通分支数: {len(list)}")
    for index, value in enumerate(list):
        print(f"分支 {index + 1}: {value}")

    if len(list) == 1:
        print("结论: 该无向图是连通图")
    else:
        print("结论: 该无向图不是连通图")

def visualize(e_list,n):
    G = nx.Graph()
    for i in range(n):
        G.add_node(f"v{i + 1}")
    for u, v in e_list:
        G.add_edge(f"v{u + 1}", f"v{v + 1}")

    plt.figure(figsize=(8, 6))
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels=True, node_color='lightblue',
            node_size=800, font_weight='bold', edge_color='gray')
    plt.title("Graph Visualization")
    plt.show()

def main():
    print("\n------- 获取图的参数 ------")

    # 关联矩阵M - 行代表顶点, 列代表边
    M,n,m = input_M()
    while M is None:
        print("请重新输入! ！\n")
        M,n,m = input_M()

    print("\n当前关联矩阵 M 为：")
    print(M)

    # 生成相邻矩阵A - 行 顶点，列 顶点
    print("\n------- 生成相邻矩阵 ------")
    A, e_list = build_A(M,n,m)

    print("当前相邻矩阵 A 为：")
    print(A)

    # 矩阵属性分析
    print("\n------- 矩阵基本要素 ------")
    analyze(M,A,n)

    # 图可视化
    print("\n------- 图可视化 ------")
    visualize(e_list,n)



# 主函数入口
if __name__ == "__main__":
    main()


