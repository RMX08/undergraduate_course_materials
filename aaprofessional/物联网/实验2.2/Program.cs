using System;
using System.IO;

namespace KL_W6000解析
{
    class Program
    {
        static void Main(string[] args)
        {
            // 读取文件中的数据
            string data = File.ReadAllText("D:\\yyr\\study\\物联网\\实验2.2\\KL-W6000.txt").Replace(" ", "");
            // 拆分字节（每2个字符为1个字节）
            string[] bytes = new string[13];
            for (int i = 0; i < 13; i++)
            {
                bytes[i] = data.Substring(i * 2, 2);
            }

            // 解析开关量：寄存器1第二个字节30
            string switchHex = bytes[4];
            string switchBin = Convert.ToString(Convert.ToInt32(switchHex, 16), 2).PadLeft(8, '0');
            char[] switchStatus = switchBin.Substring(0, 4).ToCharArray(); // 前4位
            string smoke = switchStatus[0] == '0' ? "关" : "开";
            string fire = switchStatus[1] == '0' ? "关" : "开";
            string infrared = switchStatus[2] == '0' ? "关" : "开";
            string water = switchStatus[3] == '0' ? "关" : "开";

            // 解析温度：寄存器3 00AB
            string tempHex = bytes[7] + bytes[8];
            double temp = Convert.ToInt32(tempHex, 16) / 10.0;

            // 解析湿度：寄存器4 01A3
            string humiHex = bytes[9] + bytes[10];
            double humi = Convert.ToInt32(humiHex, 16) / 10.0;

            // 输出结果
            Console.WriteLine("KL-W6000 返回数据：");
            Console.WriteLine($"日期时间：{DateTime.Now.ToString("yyyy-MM-dd, HH:mm:ss")}");
            Console.WriteLine($"烟感状态：{smoke}；");
            Console.WriteLine($"火感状态：{fire}；");
            Console.WriteLine($"红外状态：{infrared}；");
            Console.WriteLine($"水浸状态：{water}；");
            Console.WriteLine($"环境温度：{temp} ℃；");
            Console.WriteLine($"环境湿度：{humi}%；");
            return;
        }
    }
}