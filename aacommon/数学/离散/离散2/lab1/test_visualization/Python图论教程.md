# Python图论编程完整教程
## 从基础到NetworkX和Matplotlib可视化

---

## 目录
1. [Python基础回顾](#1-python基础回顾)
2. [NumPy数组操作](#2-numpy数组操作)
3. [图论基础概念](#3-图论基础概念)
4. [NetworkX图库入门](#4-networkx图库入门)
5. [Matplotlib绘图美化](#5-matplotlib绘图美化)
6. [完整项目实战](#6-完整项目实战)

---

## 1. Python基础回顾

**本章导读**

在深入学习图论编程之前，我们需要先掌握Python的基础知识。Python是一门简洁优雅的编程语言，非常适合科学计算和数据处理。本章将带你快速回顾Python的核心概念，包括变量、数据结构、控制流、函数和面向对象编程。

即使你是Python初学者也不用担心，这些内容都是从零开始讲解的。如果你已经熟悉Python，可以快速浏览作为复习，或直接跳到第2章。

**学习目标**
- 掌握Python基本数据类型和数据结构
- 理解条件判断和循环控制
- 学会定义和使用函数
- 了解面向对象编程的基本概念

### 1.1 变量和数据类型

```python
# 基本数据类型
number = 42              # 整数
pi = 3.14159            # 浮点数
name = "Graph"          # 字符串
is_connected = True     # 布尔值

# 列表（可变序列）
vertices = ['v1', 'v2', 'v3', 'v4']
degrees = [2, 3, 1, 2]

# 元组（不可变序列）
edge = ('v1', 'v2')
position = (10, 20)

# 字典（键值对）
graph_info = {
    'vertices': 6,
    'edges': 5,
    'connected': True
}

# 集合（无序不重复）
neighbors = {'v1', 'v3', 'v4'}
```

### 1.2 控制流

```python
# if-elif-else 条件判断
degree = 3
if degree == 0:
    print("孤立顶点")
elif degree == 1:
    print("叶子节点")
else:
    print(f"度数为 {degree}")

# for 循环
vertices = ['v1', 'v2', 'v3']
for v in vertices:
    print(f"顶点: {v}")

# 带索引的循环
for i, v in enumerate(vertices):
    print(f"第{i+1}个顶点: {v}")

# while 循环
count = 0
while count < 3:
    print(f"迭代 {count}")
    count += 1
```

### 1.3 函数定义

```python
def calculate_degree(adjacency_row):
    """
    计算顶点的度数
    
    参数:
        adjacency_row: 邻接矩阵的一行
    
    返回:
        度数（整数）
    """
    return sum(adjacency_row)

# 调用函数
row = [0, 1, 1, 0]
degree = calculate_degree(row)
print(f"度数: {degree}")  # 输出: 度数: 2

# 带默认参数的函数
def greet(name="访客", greeting="你好"):
    return f"{greeting}, {name}!"

print(greet())                    # 你好, 访客!
print(greet("张三"))              # 你好, 张三!
print(greet("李四", "欢迎"))      # 欢迎, 李四!
```

### 1.4 列表推导式

```python
# 基本列表推导式
squares = [x**2 for x in range(1, 6)]
print(squares)  # [1, 4, 9, 16, 25]

# 带条件的推导式
even_squares = [x**2 for x in range(1, 11) if x % 2 == 0]
print(even_squares)  # [4, 16, 36, 64, 100]

# 字典推导式
degree_dict = {f'v{i}': i for i in range(1, 5)}
print(degree_dict)  # {'v1': 1, 'v2': 2, 'v3': 3, 'v4': 4}
```

### 1.5 类和面向对象编程

```python
class Vertex:
    """顶点类"""
    
    def __init__(self, name):
        """初始化顶点"""
        self.name = name
        self.neighbors = []
    
    def add_neighbor(self, vertex):
        """添加邻居"""
        if vertex not in self.neighbors:
            self.neighbors.append(vertex)
    
    def get_degree(self):
        """获取度数"""
        return len(self.neighbors)
    
    def __str__(self):
        """字符串表示"""
        return f"Vertex({self.name})"

# 使用类
v1 = Vertex("v1")
v2 = Vertex("v2")
v1.add_neighbor(v2)
print(f"{v1} 的度数: {v1.get_degree()}")
```

---

## 2. NumPy数组操作

**本章导读**

NumPy（Numerical Python）是Python科学计算的基石，它提供了高效的多维数组对象和丰富的数学函数库。在图论中，我们经常需要使用矩阵来表示图的结构（如邻接矩阵、关联矩阵），NumPy能让这些操作变得简单高效。

想象一下，如果用纯Python列表来处理一个100×100的矩阵，代码会非常冗长且运行缓慢。而NumPy只需几行代码就能完成同样的任务，且速度快10-100倍！

**学习目标**
- 掌握NumPy数组的创建和基本属性
- 学会数组索引、切片和条件筛选
- 理解数组运算和矩阵操作
- 熟练使用NumPy的常用函数

**为什么学习NumPy？**
- 图的邻接矩阵和关联矩阵都是NumPy数组
- 图算法中的许多计算都依赖矩阵运算
- NumPy是pandas、NetworkX等库的基础

### 2.1 安装和导入

```python
# 安装（在命令行中执行）
# pip install numpy

# 导入
import numpy as np
```

### 2.2 创建数组

```python
import numpy as np
# 从列表创建
arr1 = np.array([1, 2, 3, 4])
print(arr1)  # [1 2 3 4]

# 创建二维数组（矩阵）
matrix = np.array([
    [1, 0, 1],
    [0, 1, 1],
    [1, 1, 0]
])
print(matrix)

# 创建特殊矩阵
zeros = np.zeros((3, 3))        # 零矩阵
ones = np.ones((2, 4))          # 全1矩阵
identity = np.eye(4)            # 单位矩阵
random_matrix = np.random.rand(3, 3)  # 随机矩阵
```

### 2.3 数组属性

```python
import numpy as np
matrix = np.array([
    [1, 0, 1, 0],
    [0, 1, 1, 0],
    [1, 1, 0, 1]
])

print(f"形状: {matrix.shape}")      # (3, 4)
print(f"维度: {matrix.ndim}")       # 2
print(f"元素总数: {matrix.size}")   # 12
print(f"数据类型: {matrix.dtype}")  # int32 或 int64
```

### 2.4 索引和切片

```python
import numpy as np
matrix = np.array([
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
])

# 索引（从0开始）
print(matrix[0, 0])     # 1 (第一行第一列)
print(matrix[1, 2])     # 6 (第二行第三列)

# 切片
print(matrix[0, :])     # [1 2 3] (第一行)
print(matrix[:, 0])     # [1 4 7] (第一列)
print(matrix[0:2, 1:3]) # [[2 3]   (子矩阵)
                        #  [5 6]]

# 条件索引
print(matrix > 5)       # 布尔数组
print(matrix[matrix > 5])  # [6 7 8 9] (所有大于5的元素)
```

### 2.5 数组运算

```python
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

# 逐元素运算
print(a + b)    # [5 7 9]
print(a * b)    # [4 10 18]
print(a ** 2)   # [1 4 9]

# 聚合函数
print(np.sum(a))     # 6
print(np.mean(a))    # 2.0
print(np.max(a))     # 3
print(np.min(a))     # 1

# 矩阵运算
A = np.array([[1, 2], [3, 4]])
B = np.array([[5, 6], [7, 8]])

print(A @ B)         # 矩阵乘法
print(A.T)           # 转置
```

### 2.6 实用函数

```python
# 重塑数组
arr = np.array([1, 2, 3, 4, 5, 6])
matrix = arr.reshape(2, 3)
print(matrix)
# [[1 2 3]
#  [4 5 6]]

# 查找索引
arr = np.array([10, 20, 30, 40, 50])
indices = np.where(arr > 25)
print(indices)  # (array([2, 3, 4]),)
print(arr[indices])  # [30 40 50]

# 排序
arr = np.array([3, 1, 4, 1, 5])
print(np.sort(arr))  # [1 1 3 4 5]
```

---

## 3. 图论基础概念

**本章导读**

图论是数学的一个分支，研究由节点（顶点）和连接它们的边组成的结构。它看似抽象，实际上在我们生活中无处不在：社交网络中的好友关系、城市间的交通网络、互联网的网页链接、甚至是大脑中的神经元连接，都可以用图来表示。

理解图论的基本概念是编程实现的前提。本章将用通俗易懂的语言和直观的图示，帮助你建立图论的思维模型。不需要深厚的数学功底，只要跟着示例走，你就能掌握这些重要概念。

**学习目标**
- 理解图、顶点、边的基本定义
- 掌握图的两种主要表示方法（邻接矩阵、关联矩阵）
- 理解度数、邻域、路径等核心概念
- 区分简单图、连通图等不同类型的图

**本章内容在实验中的应用**
- 关联矩阵是实验的输入数据格式
- 邻接矩阵是实验要求计算的结果之一
- 度数和邻域是基本要素分析的重点
- 连通性判定是实验的核心任务

### 3.1 图的定义

**图（Graph）** 是由顶点（Vertex）集合和边（Edge）集合组成的数学结构，记作 G = (V, E)。

- **顶点（Vertex/Node）**: 图中的基本单元，用圆圈表示
- **边（Edge/Link）**: 连接两个顶点的线段
- **无向图**: 边没有方向
- **有向图**: 边有方向（箭头）

### 3.2 图的表示方法

#### 3.2.1 邻接矩阵（Adjacency Matrix）

n×n 矩阵 A，其中 A[i][j] 表示顶点 i 和顶点 j 之间的边数。

```
例：无向图
  v1 - v2
  |    |
  v3 - v4

邻接矩阵 A:
     v1  v2  v3  v4
v1 [ 0   1   1   0 ]
v2 [ 1   0   0   1 ]
v3 [ 1   0   0   1 ]
v4 [ 0   1   1   0 ]
```

#### 3.2.2 关联矩阵（Incidence Matrix）

n×m 矩阵 M，其中 n 是顶点数，m 是边数。M[i][j] = 1 表示顶点 i 与边 j 相关联。

```
例：
  v1 -e1- v2
  |       |
 e2      e4
  |       |
  v3 -e3- v4

关联矩阵 M:
     e1  e2  e3  e4
v1 [ 1   1   0   0 ]
v2 [ 1   0   0   1 ]
v3 [ 0   1   1   0 ]
v4 [ 0   0   1   1 ]
```

### 3.3 重要概念

- **度数（Degree）**: 与顶点相连的边数
- **邻域（Neighborhood）**: 与顶点相邻的所有顶点集合
- **路径（Path）**: 顶点序列，相邻顶点之间有边
- **连通图（Connected Graph）**: 任意两个顶点之间都存在路径
- **简单图（Simple Graph）**: 无自环、无重边的图
- **自环（Loop）**: 连接顶点到自身的边
- **重边（Multiple Edges）**: 连接同一对顶点的多条边

---

## 4. NetworkX图库入门

**本章导读**

NetworkX是Python中最强大、最流行的图论库，它让复杂的图论算法变得触手可及。不需要自己从零实现深度优先搜索、最短路径算法，NetworkX已经为你准备好了上百个现成的函数。

这个库由美国洛斯阿拉莫斯国家实验室开发，被全世界数万名研究者使用。从社交网络分析到生物信息学，从推荐系统到交通规划，NetworkX都能大显身手。

本章将带你从零开始学习NetworkX，从最简单的创建图开始，逐步掌握各种高级功能。每个知识点都配有实际代码示例，你可以直接运行验证。

**学习目标**
- 掌握创建和操作图的基本方法
- 学会从矩阵构建图结构
- 理解如何查询图的属性（度数、邻居等）
- 掌握连通性检测等常用算法
- 了解NetworkX提供的特殊图类型

**NetworkX的优势**
- 简洁的API，易学易用
- 支持有向图、无向图、多重图等多种图类型
- 内置几十种经典图算法
- 与NumPy、Matplotlib无缝集成
- 开源免费，社区活跃

### 4.1 安装和导入

```python
# 安装（在命令行中执行）
# pip install networkx

# 导入
import networkx as nx
```

### 4.2 创建图

```python
# 创建空图
G = nx.Graph()        # 无向图
D = nx.DiGraph()      # 有向图
M = nx.MultiGraph()   # 多重图（允许重边）

# 添加单个节点
G.add_node('v1')
G.add_node('v2')

# 批量添加节点
G.add_nodes_from(['v3', 'v4', 'v5'])

# 添加单条边
G.add_edge('v1', 'v2')

# 批量添加边
G.add_edges_from([
    ('v1', 'v3'),
    ('v2', 'v3'),
    ('v3', 'v4')
])

# 一次性创建图
G = nx.Graph([
    ('v1', 'v2'),
    ('v2', 'v3'),
    ('v3', 'v4'),
    ('v4', 'v1')
])
```

### 4.3 图的基本操作

```python
# 查看节点和边
print(f"节点: {G.nodes()}")
print(f"边: {G.edges()}")
print(f"节点数: {G.number_of_nodes()}")
print(f"边数: {G.number_of_edges()}")

# 查看度数
print(f"所有度数: {dict(G.degree())}")
print(f"v1的度数: {G.degree('v1')}")

# 查看邻居
print(f"v1的邻居: {list(G.neighbors('v1'))}")

# 检查边是否存在
print(f"v1-v2之间有边吗? {G.has_edge('v1', 'v2')}")
```

### 4.4 从矩阵创建图

```python
import numpy as np

# 从邻接矩阵创建
adjacency_matrix = np.array([
    [0, 1, 1, 0],
    [1, 0, 1, 1],
    [1, 1, 0, 1],
    [0, 1, 1, 0]
])
G = nx.from_numpy_array(adjacency_matrix)

# 重命名节点
mapping = {i: f'v{i+1}' for i in range(4)}
G = nx.relabel_nodes(G, mapping)
```

### 4.5 图的分析

```python
# 连通性检查
is_connected = nx.is_connected(G)
print(f"是否连通: {is_connected}")

# 获取连通分支
components = list(nx.connected_components(G))
print(f"连通分支: {components}")

# 最短路径
if nx.has_path(G, 'v1', 'v4'):
    path = nx.shortest_path(G, 'v1', 'v4')
    print(f"v1到v4的最短路径: {path}")

# 图的直径（最长最短路径）
if is_connected:
    diameter = nx.diameter(G)
    print(f"图的直径: {diameter}")
```

### 4.6 特殊图的创建

```python
# 完全图（所有顶点两两相连）
K5 = nx.complete_graph(5)

# 循环图
C6 = nx.cycle_graph(6)

# 路径图
P4 = nx.path_graph(4)

# 星形图
S5 = nx.star_graph(5)

# 网格图
Grid = nx.grid_2d_graph(3, 3)

# 随机图
Random = nx.erdos_renyi_graph(10, 0.3)  # 10个节点，边概率0.3
```

---

## 5. Matplotlib绘图美化

**本章导读**

"一图胜千言"，好的可视化能让复杂的图结构一目了然。Matplotlib是Python中最强大的绘图库，它能将NetworkX创建的抽象图结构转化为直观美观的图形。

想象一下，如果只有一堆数字和矩阵，你很难理解一个社交网络的结构。但如果画出来，你马上就能看出谁是中心人物、哪些人形成了小圈子。这就是可视化的力量。

本章将教你如何用Matplotlib绘制专业级的图形可视化。从基本的绘图开始，到调整颜色、布局、标签，最后生成可以直接用于报告和论文的高质量图片。

**学习目标**
- 掌握基本的图形绘制方法
- 理解不同布局算法的特点和应用场景
- 学会美化节点、边和标签
- 掌握配色方案和样式设置
- 学会保存高质量图片

**可视化的重要性**
- 实验要求必须提供图的可视化表示
- 好的可视化能帮助你调试代码
- 专业的图形让报告更有说服力
- 可视化是数据分析的重要工具

### 5.1 安装和基本设置

```python
# 安装
# pip install matplotlib

# 导入
import matplotlib.pyplot as plt
from matplotlib import rcParams

# 设置中文字体支持
rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
rcParams['axes.unicode_minus'] = False
```

### 5.2 基本绘图

```python
import networkx as nx
import matplotlib.pyplot as plt

# 创建图
G = nx.Graph([
    ('v1', 'v2'),
    ('v2', 'v3'),
    ('v3', 'v4'),
    ('v4', 'v1'),
    ('v1', 'v3')
])

# 最简单的绘图
plt.figure(figsize=(8, 6))
nx.draw(G, with_labels=True)
plt.show()
```

### 5.3 布局算法

布局决定了节点在画布上的位置。

```python
# Spring布局（力导向，最常用）
pos = nx.spring_layout(G, k=2, iterations=50, seed=42)

# 圆形布局
pos = nx.circular_layout(G)

# 随机布局
pos = nx.random_layout(G, seed=42)

# Shell布局（分层圆形）
pos = nx.shell_layout(G)

# Kamada-Kawai布局
pos = nx.kamada_kawai_layout(G)

# 手动指定位置
pos = {
    'v1': (0, 1),
    'v2': (1, 1),
    'v3': (1, 0),
    'v4': (0, 0)
}
```

### 5.4 美化节点

```python
plt.figure(figsize=(10, 8))

# 获取布局
pos = nx.spring_layout(G, seed=42)

# 绘制节点
nx.draw_networkx_nodes(
    G, pos,
    node_color='lightblue',      # 节点颜色
    node_size=1500,               # 节点大小
    edgecolors='navy',            # 节点边框颜色
    linewidths=2,                 # 边框宽度
    alpha=0.9                     # 透明度
)

# 绘制边
nx.draw_networkx_edges(
    G, pos,
    width=2,                      # 边的宽度
    alpha=0.6,                    # 透明度
    edge_color='gray'             # 边的颜色
)

# 绘制标签
nx.draw_networkx_labels(
    G, pos,
    font_size=14,                 # 字体大小
    font_weight='bold',           # 字体粗细
    font_color='darkblue'         # 字体颜色
)

plt.title("美化的图", fontsize=16, fontweight='bold')
plt.axis('off')  # 隐藏坐标轴
plt.tight_layout()
plt.show()
```

### 5.5 不同颜色节点

```python
# 根据度数着色
node_colors = []
for node in G.nodes():
    degree = G.degree(node)
    if degree <= 2:
        node_colors.append('lightgreen')
    elif degree == 3:
        node_colors.append('yellow')
    else:
        node_colors.append('red')

nx.draw_networkx_nodes(G, pos, node_color=node_colors, node_size=1500)
```

### 5.6 边标签

```python
# 添加边标签
edge_labels = {
    ('v1', 'v2'): 'e1',
    ('v2', 'v3'): 'e2',
    ('v3', 'v4'): 'e3',
    ('v4', 'v1'): 'e4',
    ('v1', 'v3'): 'e5'
}

nx.draw_networkx_edge_labels(
    G, pos,
    edge_labels,
    font_size=10,
    font_color='red',
    bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.7)
)
```

### 5.7 保存图片

```python
# 保存为高清PNG
plt.savefig('graph.png', dpi=300, bbox_inches='tight')

# 保存为PDF（矢量图）
plt.savefig('graph.pdf', bbox_inches='tight')

# 保存为SVG
plt.savefig('graph.svg', bbox_inches='tight')
```

### 5.8 多子图布局

```python
fig, axes = plt.subplots(1, 3, figsize=(15, 5))

# 子图1：Spring布局
pos1 = nx.spring_layout(G, seed=42)
nx.draw(G, pos1, ax=axes[0], with_labels=True, node_color='lightblue', 
        node_size=500)
axes[0].set_title('Spring布局')

# 子图2：圆形布局
pos2 = nx.circular_layout(G)
nx.draw(G, pos2, ax=axes[1], with_labels=True, node_color='lightgreen', 
        node_size=500)
axes[1].set_title('圆形布局')

# 子图3：Shell布局
pos3 = nx.shell_layout(G)
nx.draw(G, pos3, ax=axes[2], with_labels=True, node_color='lightyellow', 
        node_size=500)
axes[2].set_title('Shell布局')

plt.tight_layout()
plt.show()
```

---

## 6. 完整项目实战

**本章导读**

前面五章我们学习了Python基础、NumPy、图论概念、NetworkX和Matplotlib。现在是时候将所有知识整合起来，完成一个真实的图分析项目了。

本章将带你从零开始构建一个完整的图分析系统，这正是你实验报告所需要的核心代码。我们会用面向对象的方式组织代码，实现关联矩阵输入、邻接矩阵计算、度数分析、简单图判定、连通性检测等全部功能。

这不是一个玩具项目，而是一个可以处理真实数据、具有良好代码结构、易于扩展的专业系统。通过学习本章，你不仅能完成实验，还能掌握如何设计和实现一个完整的Python项目。

**学习目标**
- 理解如何设计一个图分析系统的架构
- 掌握面向对象编程在实际项目中的应用
- 学会将算法需求转化为Python代码
- 理解如何组织和模块化代码
- 掌握调试和优化的技巧

**项目特点**
- 完全符合实验要求的所有功能
- 清晰的代码结构和注释
- 健壮的错误处理
- 易于测试和扩展
- 包含完整的示例数据

### 6.1 项目需求

实现一个图分析系统，能够：
1. 从关联矩阵创建图
2. 可视化图结构
3. 计算度数和邻域
4. 判断是否为简单图
5. 判断连通性

### 6.2 完整代码示例

```python
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib import rcParams

# 设置中文字体
rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
rcParams['axes.unicode_minus'] = False


class GraphAnalyzer:
    """图分析器"""
    
    def __init__(self, incidence_matrix):
        self.incidence_matrix = np.array(incidence_matrix)
        self.num_vertices = self.incidence_matrix.shape[0]
        self.num_edges = self.incidence_matrix.shape[1]
        self.graph = None
        
    def build_graph(self):
        """根据关联矩阵构建NetworkX图"""
        G = nx.Graph()
        
        # 添加顶点
        for i in range(self.num_vertices):
            G.add_node(f'v{i+1}')
        
        # 添加边
        for j in range(self.num_edges):
            edge_col = self.incidence_matrix[:, j]
            vertices = np.where(edge_col == 1)[0]
            
            if len(vertices) == 2:  # 正常边
                v1, v2 = vertices
                G.add_edge(f'v{v1+1}', f'v{v2+1}')
            elif len(vertices) == 1:  # 自环
                v = vertices[0]
                G.add_edge(f'v{v+1}', f'v{v+1}')
        
        self.graph = G
        return G
    
    def visualize(self):
        """可视化图"""
        if self.graph is None:
            self.build_graph()
        
        plt.figure(figsize=(12, 9))
        pos = nx.spring_layout(self.graph, k=2, iterations=50, seed=42)
        
        # 绘制节点
        nx.draw_networkx_nodes(self.graph, pos, 
                               node_color='lightblue',
                               node_size=1500,
                               edgecolors='navy',
                               linewidths=2)
        
        # 绘制边
        nx.draw_networkx_edges(self.graph, pos,
                               width=2,
                               alpha=0.6,
                               edge_color='gray')
        
        # 绘制标签
        nx.draw_networkx_labels(self.graph, pos,
                                font_size=14,
                                font_weight='bold',
                                font_color='darkblue')
        
        plt.title("图的可视化", fontsize=16, fontweight='bold')
        plt.axis('off')
        plt.tight_layout()
        plt.show()
    
    def analyze(self):
        """分析图的性质"""
        if self.graph is None:
            self.build_graph()
        
        print("="*50)
        print("图分析报告")
        print("="*50)
        
        # 度数
        print("\n【度数分析】")
        for node in sorted(self.graph.nodes()):
            degree = self.graph.degree(node)
            print(f"  {node}: {degree}")
        
        # 邻域
        print("\n【邻域分析】")
        for node in sorted(self.graph.nodes()):
            neighbors = list(self.graph.neighbors(node))
            neighbors_str = ", ".join(neighbors) if neighbors else "无"
            print(f"  {node}: {{{neighbors_str}}}")
        
        # 简单图判定
        is_simple = nx.is_simple_path(self.graph, list(self.graph.nodes()))
        print(f"\n【简单图】: {is_simple}")
        
        # 连通性
        is_connected = nx.is_connected(self.graph)
        print(f"【连通性】: {'连通' if is_connected else '不连通'}")
        
        if not is_connected:
            components = list(nx.connected_components(self.graph))
            print(f"  分支数: {len(components)}")
            for i, comp in enumerate(components, 1):
                print(f"  分支{i}: {sorted(comp)}")


# 使用示例
if __name__ == "__main__":
    # 示例：关联矩阵 (4个顶点，5条边)
    incidence = [
        [1, 1, 0, 0, 0],
        [1, 0, 1, 1, 0],
        [0, 1, 1, 0, 1],
        [0, 0, 0, 1, 1]
    ]
    
    analyzer = GraphAnalyzer(incidence)
    analyzer.analyze()
    analyzer.visualize()
```

### 6.3 高级技巧

#### 6.3.1 渐变色节点

```python
import matplotlib.cm as cm

# 根据度数使用渐变色
degrees = [G.degree(n) for n in G.nodes()]
max_degree = max(degrees)

node_colors = [degree / max_degree for degree in degrees]

nx.draw_networkx_nodes(G, pos,
                       node_color=node_colors,
                       cmap=cm.YlOrRd,  # 黄-橙-红渐变
                       node_size=1500,
                       vmin=0, vmax=1)
```

#### 6.3.2 边宽度根据权重

```python
# 假设边有权重
G = nx.Graph()
G.add_edge('v1', 'v2', weight=1)
G.add_edge('v2', 'v3', weight=3)
G.add_edge('v3', 'v4', weight=2)

# 边宽度与权重成正比
widths = [G[u][v]['weight'] for u, v in G.edges()]

nx.draw_networkx_edges(G, pos, width=widths, edge_color='gray')
```

#### 6.3.3 动画效果（需要额外库）

```python
from matplotlib.animation import FuncAnimation

fig, ax = plt.subplots(figsize=(10, 8))

def update(frame):
    ax.clear()
    # 逐步添加边
    H = nx.Graph()
    H.add_nodes_from(G.nodes())
    edges = list(G.edges())
    H.add_edges_from(edges[:frame+1])
    
    nx.draw(H, pos, ax=ax, with_labels=True, 
            node_color='lightblue', node_size=1000)
    ax.set_title(f'边数: {frame+1}/{len(edges)}')

anim = FuncAnimation(fig, update, frames=len(G.edges()), 
                     interval=500, repeat=True)
plt.show()
```

---

## 7. 常见问题与解决方案

**本章导读**

在学习和使用Python进行图论编程的过程中，你可能会遇到各种问题。有些是环境配置问题，有些是代码错误，还有些是性能瓶颈。不要担心，这些都是学习过程中的正常现象。

本章收集了最常见的问题和经过验证的解决方案。这些都是实际开发中真实遇到的问题，每个解决方案都经过测试可用。遇到问题时先在这里查找，能节省你大量的时间。

**常见问题分类**
- 环境和配置问题（中文乱码、库安装失败等）
- 可视化问题（图太密集、布局不美观等）
- 性能问题（大图处理慢、内存不足等）
- 代码错误（语法错误、逻辑错误等）

**使用建议**
- 先尝试理解错误信息
- 对照本章查找类似问题
- 如果找不到，尝试Google搜索错误信息
- 实在解决不了可以在社区提问

### 7.1 中文显示乱码

```python
# 方法1：设置字体
from matplotlib import rcParams
rcParams['font.sans-serif'] = ['SimHei']
rcParams['axes.unicode_minus'] = False

# 方法2：指定字体路径
from matplotlib.font_manager import FontProperties
font = FontProperties(fname='C:\\Windows\\Fonts\\simhei.ttf')
plt.title('中文标题', fontproperties=font)
```

### 7.2 图太密集难以阅读

```python
# 增加节点间距
pos = nx.spring_layout(G, k=3, iterations=100)  # k越大，间距越大

# 调整图片大小
plt.figure(figsize=(16, 12))

# 减小节点大小
nx.draw_networkx_nodes(G, pos, node_size=800)
```

### 7.3 大图性能问题

```python
# 对于大图（>1000个节点），使用更快的绘图方法
nx.draw_networkx(G, pos, 
                 arrows=False,
                 with_labels=False,  # 不显示标签
                 node_size=50,       # 小节点
                 width=0.5)          # 细边
```

---

## 9. 参考资源

**本章导读**

学习是一个持续的过程，本教程只是你图论编程之旅的起点。要成为真正的专家，你需要不断学习新知识、查阅文档、阅读优秀代码。本章列出了最权威、最有价值的学习资源。

这些资源包括官方文档（最准确最全面）、经典教材（系统深入）、在线教程（实用易懂）。建议你收藏这些链接，在需要时随时查阅。

**如何使用这些资源**
- **官方文档**：查找函数用法、API参数
- **书籍教材**：系统学习理论知识
- **在线教程**：学习实用技巧和最佳实践
- **GitHub项目**：阅读优秀代码，学习工程实践

**学习路线建议**
1. 先完成本教程，打好基础
2. 查阅官方文档，深入了解各种API
3. 阅读经典书籍，加强理论功底
4. 做实际项目，积累实战经验

- **NetworkX官方文档**: https://networkx.org/documentation/
- **Matplotlib官方文档**: https://matplotlib.org/
- **NumPy官方文档**: https://numpy.org/doc/
- **图论入门书籍**:《离散数学及其应用》
- **在线教程**: https://www.python-course.eu/graphs_python.php

---

**祝学习愉快！** 🎓📊🐍
