import numpy as np  # 处理矩阵数组
import matplotlib
matplotlib.use('TkAgg') # 或者 'Qt5Agg'，强制指定后端
import matplotlib.pyplot as plt  # 画图展示
import networkx as nx  # 辅助画图

# 获取邻接矩阵
def input_A():
    try:
        n = int(input("请输入顶点数 n："))
        print(f"请逐行输入邻接矩阵 (共 {n} 行，每行 {n} 个数字，空格分隔):")

        # while True:
        A = []
        for i in range(n):
            row_str = input(f"第 {i + 1} 行: ").split()  # 按照空格来分割字符
            row_int = [int(x) for x in row_str]   # 转换为int
            # 输入判断
            if len(row_int) != n:
                print("该行输入错误！！请重新输入！")
                i -= 1
                continue
            A.append(row_int)

        return np.array(A),n
    except ValueError:
        print("输入格式错误，请重新输入！")
        exit()

# 为图中的每一条边分配编号 (e1, e2, e3...)
def edge_label(n, A):
    label = {}
    count = 1

    for i in range(n):
        for j in range(i + 1, n):   # 上三角
            if A[i][j] != 0:
                # 规范化边 (小, 大)
                edge_tuple = tuple(sorted((i, j)))
                num = f"e{count}"
                label[edge_tuple] = num
                print(f"  {num}: ({i}, {j})") # 打印对照关系
                count += 1
    return label

# 将环路列表转换为格式字符串。
def cycle_output(cycles_list, label):
    output = []

    for cycle in cycles_list:
        if not cycle:  # 如果是空集
            output.append("Φ")
            continue

        # 把环里的每一条边转换为标签 (e1, e2...)
        elabel = []
        for edge in cycle:
            norm_edge = tuple(sorted(edge))
            if norm_edge in label:
                elabel.append(label[norm_edge])

        # 拼接成字符串 "e1e2e3"
        cycle_str = "".join(elabel)
        output.append(cycle_str)

    # 4. 最终用大括号包围
    return "{" + ", ".join(output) + "}"

# 转换为关联矩阵
def get_M(A,n):
    edges = []
    # 提取所有边 (只遍历上三角，避免重复)
    for i in range(n):
        for j in range(i + 1, n):
            if A[i][j] != 0:
                edges.append((i, j))

    m = len(edges)

    # 初始化关联矩阵 M: 行=顶点, 列=边
    M = np.zeros((n,m), dtype=int)

    # 构建"有向"关联矩阵,方便利用矩阵树定理
    for i, (u, v) in enumerate(edges):
        M[u][i] = 1         # 起点为1，终点为-1
        M[v][i] = -1

    return M, edges, m

# 递归求行列式的值
def value_of_matrix(m):
    n = len(m)
    if n == 1:
        return m[0][0]
    if n == 2:
        return m[0][0] * m[1][1] - m[0][1] * m[1][0]

    sum = 0
    # 沿第一行展开
    for k in range(n):
        if m[0][k] == 0:
            continue
        # 余子式矩阵
        sub_m = []
        for i in range(1, n):   # 除第一行
            row = []
            for j in range(n):
                if j == k:
                    continue
                row.append(m[i][j])
            sub_m.append(row)

        # 递归计算
        if k % 2 == 0:
            sum += m[0][k] * value_of_matrix(sub_m)
        else:
            sum -= m[0][k] * value_of_matrix(sub_m)

    return sum

# 利用关联矩阵计算生成树个数
# 生成树个数 = det(B_r * B_r.T)
def count_T(M):
    # Br 去除任意一行的余子式
    B_r = M[:-1, :]       # 去除最后一行

    # 计算矩阵乘积  B_r * B_r^T
    mul = np.dot(B_r, B_r.T)

    # 计算行列式
    # 转换成列表传给递归函数
    mul_list = mul.tolist()
    value = value_of_matrix(mul_list)

    return value

# 使用bfs广度构建生成树
def build_tree_bfs(n, A):
    tree_A = np.zeros((n, n), dtype=int)
    tree_edges = []
    visited = [False] * n

    # 从节点0开始
    queue = [0]
    visited[0] = True

    while queue:
        current = queue.pop(0)
        for v in range(n):
            # 如果current和v有边，且v未访问
            if A[current][v] == 1 and not visited[v]:
                visited[v] = True
                queue.append(v)

                tree_A[current][v] = tree_A[v][current] = 1
                #  (小, 大) 方便比较
                if current < v:
                    tree_edges.append((current, v))
                else:
                    tree_edges.append((v, current))

    return tree_A, tree_edges     # 树的邻接矩阵，树的边集合

# 在树中找路径
# 两点间的唯一路径
def path_in_tree(tree, start, end, path, visited):
    visited[start] = True
    path.append(start)

    if start == end:
        return True

    for node in range(len(tree)):
        if tree[start][node] == 1 and not visited[node]:
            if path_in_tree(tree, node, end, path, visited): # dfs
                return True

    path.pop()  # 回溯
    return False

# 求基本回路系统（最小绕行单元库）
# 遍历所有弦，每条弦加到树上形成唯一的环。
def get_fund_cycles(n, total_edges, tree_edges, tree_A):
    fund_cycles = []

    tree = set(tree_edges)        # 列表转为set方便运算

    # 找出弦 = 所有边 - 树边
    for (u, v) in total_edges:
        if (u, v) not in tree and (v, u) not in tree:       # 弦 (u, v)
            # 在树中找 u -> v 的路径
            path = []
            visited = [False] * n
            path_in_tree(tree_A, u, v, path, visited)

            cycle = []      # 表示为边的集合
            # 路径 + 弦 = 环
            for i in range(len(path) - 1):
                p1, p2 = path[i], path[i + 1]
                edge = tuple(sorted((p1, p2)))  # 将sorted返回的列表转化为元组
                cycle.append(edge)      # 路径 -> 环
            # 添加弦
            cycle.append(tuple(sorted((u, v))))

            fund_cycles.append(sorted(cycle))

    return fund_cycles

# 求所有绕行方案（环路空间）
# 对基本回路的所有子集进行异或运算。
def cycle_space(fund_cycles):
    cycles = []
    # 0到n的组合
    import itertools
    for i in range(len(fund_cycles) + 1):
        for combine in itertools.combinations(fund_cycles, i):   # 组合基本回路
            # 对 combine 中的所有环进行边集的异或运算
            count = {}
            for cycle in combine:
                for edge in cycle:
                    count[edge] = count.get(edge, 0) + 1   # 统计每条边在选中的那些回路中总共出现了多少次

            # 边出现奇数次保留，偶数次抵消
            c = [edge for edge, n in count.items() if n % 2 == 1]
            cycles.append(sorted(c))

    return cycles

# 可视化
def visualize(A,flag,tree,label):
    n = len(A)

    # 创建图对象
    G = nx.Graph()
    for i in range(n):
        G.add_node(i)
        for j in range(i + 1, n):
            if A[i][j] == 1:
                G.add_edge(i, j)

    # 布局
    pos = nx.spring_layout(G, seed=42)  # 固定种子保证两图位置一致
    plt.figure(figsize=(9,6))

    if flag == 1:   # 交通网络
        # plt.subplot(1, 2, 1)
        plt.title("origin transportation network")
        nx.draw(G, pos, with_labels=True, node_color='lightblue', edge_color='gray',
                node_size=600, font_weight='bold')
        nx.draw_networkx_edge_labels(G, pos, edge_labels=label,
                                     font_size=10, rotate=False)
    elif flag == 2:     # 生成树
        # 创建生成树对象
        T = nx.Graph()
        for i in range(n):
            T.add_node(i)
            for j in range(i + 1, n):
                if tree[i][j] == 1:
                    T.add_edge(i, j)

        # plt.subplot(1, 2, 2)
        plt.title("minimum necessary road network (spanning tree)")
        # 先画淡色的底图
        nx.draw(G, pos, with_labels=True, node_color='lightgray', edge_color='lightgray',
                style='dashed', node_size=600)
        # 再画深色的树
        nx.draw(T, pos, with_labels=True, node_color='lightblue', edge_color='red',
                width=2, node_size=500,font_weight='bold')

        nx.draw_networkx_edge_labels(G, pos, edge_labels=label,
                                     font_size=10, rotate=False)

    plt.tight_layout()  # 自动调整间距
    plt.show()


# 主程序
def main():
    print("========= 城市交通网络分析 =========")

    A, n = input_A()
    print(f"原始交通网络邻接矩阵 ({n}x{n}):")
    print(A)

    print("标定边：")
    label = edge_label(n, A)    # 生成边的编号 (e1, e2...)
    visualize(A, 1, A, label)

    # 生成关联矩阵 计算生成树个数
    print("\n========= 生成树个数 =========")
    M, M_edges, m = get_M(A,n)
    print(f"关联矩阵为 ({n}x{m}):")
    print(M)

    num_T = count_T(M)
    print(f"生成树个数为 : {num_T}")

    # 构建任意一棵生成树
    print("\n========= 最小必要路网（生成树） =========")
    tree_A, tree_edges = build_tree_bfs(n, A)
    visualize(A,2,tree_A, label)

    print(f"构建的 '最小必要路网' (生成树) 边集为:")
    list = [label[tuple(sorted(e))] for e in tree_edges]
    # list.sort(key=lambda x: int(x[1:]))
    print(f"{{ {', '.join(list)} }}")

    # 基本回路系统
    fund_cycles = get_fund_cycles(n, M_edges, tree_edges, tree_A)
    print(f"最小绕行单元库 (基本回路系统) - 共 {len(fund_cycles)} 个:")
    format_fund = cycle_output(fund_cycles, label)
    print(format_fund)

    # 环路空间
    print("\n========= 所有绕行方案（环路空间） =========")
    space = cycle_space(fund_cycles)
    print(f"所有绕行方案 (环路空间) - 共 {len(space)} 个:")
    space = cycle_output(space, label)
    print(space)

    visualize(A, 2, tree_A, label)

if __name__ == "__main__":
    main()