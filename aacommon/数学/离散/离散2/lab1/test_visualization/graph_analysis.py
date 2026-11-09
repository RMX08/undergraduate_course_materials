import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib import rcParams

# 设置中文字体支持
rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']  # 使用黑体或微软雅黑
rcParams['axes.unicode_minus'] = False  # 解决负号显示问题


class GraphAnalyzer:
    """图分析器类：用于分析和可视化无向图"""
    
    def __init__(self, incidence_matrix):
        """
        初始化图分析器
        
        参数:
            incidence_matrix: numpy数组，关联矩阵（行为顶点，列为边）
        """
        self.incidence_matrix = np.array(incidence_matrix)
        self.num_vertices = self.incidence_matrix.shape[0]
        self.num_edges = self.incidence_matrix.shape[1]
        self.adjacency_matrix = None
        self.graph = None
        
    def visualize_graph(self, title="图的可视化表示", save_path=None):
        """
        可视化图，标注顶点和边
        
        参数:
            title: 图的标题
            save_path: 保存路径（可选）
        """
        # 创建NetworkX图
        G = nx.Graph()
        
        # 添加顶点
        for i in range(self.num_vertices):
            G.add_node(f'v{i+1}')
        
        # 添加边并收集边标签
        edge_labels = {}
        for j in range(self.num_edges):
            edge_col = self.incidence_matrix[:, j]
            vertices = np.where(edge_col == 1)[0]
            
            if len(vertices) == 2:  # 正常边
                v1, v2 = vertices
                G.add_edge(f'v{v1+1}', f'v{v2+1}')
                edge_labels[(f'v{v1+1}', f'v{v2+1}')] = f'e{j+1}'
            elif len(vertices) == 1:  # 自环
                v = vertices[0]
                G.add_edge(f'v{v+1}', f'v{v+1}')
                edge_labels[(f'v{v+1}', f'v{v+1}')] = f'e{j+1}'
        
        self.graph = G
        
        # 绘图
        plt.figure(figsize=(12, 9))
        
        # 使用spring布局
        pos = nx.spring_layout(G, k=2, iterations=50, seed=42)
        
        # 绘制节点
        nx.draw_networkx_nodes(G, pos, 
                               node_color='lightblue', 
                               node_size=1500,
                               edgecolors='navy',
                               linewidths=2)
        
        # 绘制边
        nx.draw_networkx_edges(G, pos, 
                               width=2, 
                               alpha=0.6,
                               edge_color='gray')
        
        # 绘制节点标签
        nx.draw_networkx_labels(G, pos, 
                                font_size=14,
                                font_weight='bold',
                                font_color='darkblue')
        
        # 绘制边标签
        nx.draw_networkx_edge_labels(G, pos, 
                                     edge_labels,
                                     font_size=10,
                                     font_color='red',
                                     bbox=dict(boxstyle='round,pad=0.3', 
                                              facecolor='yellow', 
                                              alpha=0.7))
        
        plt.title(title, fontsize=16, fontweight='bold', pad=20)
        plt.axis('off')
        plt.tight_layout()
        
        if save_path:
            plt.savefig(save_path, dpi=300, bbox_inches='tight')
        plt.show()
        
        return G
    
    def compute_adjacency_matrix(self):
        """
        根据关联矩阵计算邻接矩阵
        
        返回:
            adjacency_matrix: 邻接矩阵（A[i,j]表示vi和vj之间的边数）
        """
        n = self.num_vertices
        A = np.zeros((n, n), dtype=int)
        
        for j in range(self.num_edges):
            edge_col = self.incidence_matrix[:, j]
            vertices = np.where(edge_col == 1)[0]
            
            if len(vertices) == 2:  # 正常边
                v1, v2 = vertices
                A[v1, v2] += 1
                A[v2, v1] += 1
            elif len(vertices) == 1:  # 自环
                v = vertices[0]
                A[v, v] += 2  # 自环在邻接矩阵中贡献2
        
        self.adjacency_matrix = A
        return A
    
    def compute_degrees(self):
        """
        计算每个顶点的度数
        
        返回:
            degrees: 字典，键为顶点名称，值为度数
        """
        degrees = {}
        for i in range(self.num_vertices):
            # 度数 = 关联矩阵该行的和
            degree = np.sum(self.incidence_matrix[i, :])
            degrees[f'v{i+1}'] = degree
        
        return degrees
    
    def compute_neighborhoods(self):
        """
        计算每个顶点的邻域
        
        返回:
            neighborhoods: 字典，键为顶点名称，值为邻居列表
        """
        if self.adjacency_matrix is None:
            self.compute_adjacency_matrix()
        
        neighborhoods = {}
        for i in range(self.num_vertices):
            neighbors = []
            for j in range(self.num_vertices):
                if self.adjacency_matrix[i, j] > 0 and i != j:
                    neighbors.append(f'v{j+1}')
            neighborhoods[f'v{i+1}'] = neighbors
        
        return neighborhoods
    
    def is_simple_graph(self):
        """
        判断是否为简单图（无自环、无重边）
        
        返回:
            (bool, list): (是否为简单图, 问题列表)
        """
        issues = []
        
        # 检查自环
        for j in range(self.num_edges):
            edge_col = self.incidence_matrix[:, j]
            vertices = np.where(edge_col == 1)[0]
            if len(vertices) == 1:
                issues.append(f"边e{j+1}是自环（连接v{vertices[0]+1}到自身）")
        
        # 检查重边
        edge_set = set()
        for j in range(self.num_edges):
            edge_col = self.incidence_matrix[:, j]
            vertices = tuple(sorted(np.where(edge_col == 1)[0]))
            if vertices in edge_set and len(vertices) == 2:
                v1, v2 = vertices
                issues.append(f"存在重边连接v{v1+1}和v{v2+1}")
            edge_set.add(vertices)
        
        is_simple = len(issues) == 0
        return is_simple, issues
    
    def is_connected(self):
        """
        判断图是否连通，并返回连通分支信息
        
        返回:
            (bool, int, list): (是否连通, 分支数, 每个分支的顶点列表)
        """
        if self.adjacency_matrix is None:
            self.compute_adjacency_matrix()
        
        n = self.num_vertices
        visited = [False] * n
        components = []
        
        def dfs(v, component):
            """深度优先搜索"""
            visited[v] = True
            component.append(f'v{v+1}')
            for u in range(n):
                if self.adjacency_matrix[v, u] > 0 and not visited[u]:
                    dfs(u, component)
        
        # 找出所有连通分支
        for i in range(n):
            if not visited[i]:
                component = []
                dfs(i, component)
                components.append(sorted(component))
        
        num_components = len(components)
        is_connected = num_components == 1
        
        return is_connected, num_components, components
    
    def print_analysis(self):
        """打印完整的图分析结果"""
        print("="*60)
        print("图的分析报告")
        print("="*60)
        
        # 1. 基本信息
        print(f"\n【基本信息】")
        print(f"顶点数: {self.num_vertices}")
        print(f"边数: {self.num_edges}")
        
        # 2. 关联矩阵
        print(f"\n【关联矩阵】（{self.num_vertices}×{self.num_edges}）")
        print(self.incidence_matrix)
        
        # 3. 邻接矩阵
        adj_matrix = self.compute_adjacency_matrix()
        print(f"\n【邻接矩阵】（{self.num_vertices}×{self.num_vertices}）")
        print(adj_matrix)
        
        # 4. 度数分析
        degrees = self.compute_degrees()
        print(f"\n【度数分析】")
        for vertex, degree in sorted(degrees.items()):
            print(f"  {vertex}: {degree}")
        
        # 5. 邻域分析
        neighborhoods = self.compute_neighborhoods()
        print(f"\n【邻域分析】")
        for vertex, neighbors in sorted(neighborhoods.items()):
            neighbors_str = ", ".join(neighbors) if neighbors else "无"
            print(f"  {vertex}: {{{neighbors_str}}}")
        
        # 6. 简单图判定
        is_simple, issues = self.is_simple_graph()
        print(f"\n【简单图判定】")
        if is_simple:
            print("  ✓ 该图是简单图（无自环、无重边）")
        else:
            print("  ✗ 该图不是简单图，存在以下问题：")
            for issue in issues:
                print(f"    - {issue}")
        
        # 7. 连通性判定
        is_connected, num_components, components = self.is_connected()
        print(f"\n【连通性判定】")
        if is_connected:
            print("  ✓ 该图是连通图")
            print(f"  分支数: 1")
            print(f"  顶点: {{{', '.join(components[0])}}}")
        else:
            print("  ✗ 该图不是连通图")
            print(f"  分支数: {num_components}")
            for i, component in enumerate(components, 1):
                print(f"  分支{i}: {{{', '.join(component)}}}")
        
        print("\n" + "="*60)


def parse_incidence_matrix_from_string(matrix_str):
    """
    从字符串解析关联矩阵
    
    参数:
        matrix_str: 矩阵字符串，格式如 "110101001000000100000000010110011001"
                   或带逗号分隔的格式
    
    返回:
        numpy数组形式的关联矩阵
    """
    # 移除空格和逗号
    matrix_str = matrix_str.replace(" ", "").replace(",", "")
    
    # 计算矩阵维度（假设是方阵或需要用户输入）
    total_elements = len(matrix_str)
    
    # 尝试推断矩阵形状
    # 这里需要额外信息，我们假设用户会提供或通过交互获取
    print(f"矩阵字符串长度: {total_elements}")
    
    # 让用户输入行数和列数
    print("请输入矩阵的行数（顶点数）和列数（边数）：")
    n = int(input("顶点数 n = "))
    m = int(input("边数 m = "))
    
    if n * m != total_elements:
        raise ValueError(f"矩阵维度不匹配: {n}×{m} ≠ {total_elements}")
    
    # 转换为矩阵
    matrix = np.array([int(c) for c in matrix_str]).reshape(n, m)
    return matrix

# 主函数
def main():
    print("="*60)
    print("图的可视化分析系统")
    print("="*60)
    
    # 提供三个示例
    examples = {
        "M1": "110101001000000100000000010110011001",
        "M2": "101110011000000000000110",
        "M3": "1001110110010001"
    }

    print("1. M1 = 110101001000000100000000010110011001")
    print("2. M2 = 101110011000000000000110")
    print("3. M3 = 1001110110010001")
    
    choice = input("\n请选择 (1-4): ").strip()
    
    if choice == "1":
        matrix_str = examples["M1"]
        n, m = 6, 6  # M1是6个顶点，6条边
        title = "示例M1的图结构"
    elif choice == "2":
        matrix_str = examples["M2"]
        n, m = 4, 6  # M2是4个顶点，6条边
        title = "示例M2的图结构"
    elif choice == "3":
        matrix_str = examples["M3"]
        n, m = 4, 4  # M3是4个顶点，4条边
        title = "示例M3的图结构"
    else:
        matrix_str = input("请输入关联矩阵字符串: ").strip()
        n = int(input("请输入顶点数: "))
        m = int(input("请输入边数: "))
        title = "自定义图结构"
    
    # 解析矩阵
    matrix_str = matrix_str.replace(" ", "").replace(",", "")
    if len(matrix_str) != n * m:
        print(f"错误：矩阵维度不匹配 ({n}×{m} = {n*m} ≠ {len(matrix_str)})")
        return
    
    incidence_matrix = np.array([int(c) for c in matrix_str]).reshape(n, m)
    
    # 创建分析器并分析
    analyzer = GraphAnalyzer(incidence_matrix)
    
    # 打印分析结果
    analyzer.print_analysis()
    
    # 可视化图
    print("\n正在生成图的可视化...")
    analyzer.visualize_graph(title=title, save_path=f"graph_{choice}.png")
    
    print(f"\n图片已保存为: graph_{choice}.png")


if __name__ == "__main__":
    main()
