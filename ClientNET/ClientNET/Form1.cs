﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ClientNET
{

    public partial class Client : Form
    {
        TcpClient tcpClient = new TcpClient();
        NetworkStream stream;

        string ip;
        string port;

        byte[] bytes = new byte[1024];

        public Client()
        {
            InitializeComponent();
        }

        private void Client_Load(object sender, EventArgs e)
        {

        }

        public void connectAsClient()
        {
            try
            {
                TcpClient tcpClient = new TcpClient();
                tcpClient.Connect(IPAddress.Parse(ip), int.Parse(port));
                //tcpClient.Connect("127.0.0.1", 7770);

                stream = tcpClient.GetStream();

                byte[] data = new byte[256];
                int bytes = stream.Read(data, 0, data.Length); // получаем количество считанных байтов
                string message = Encoding.ASCII.GetString(data, 0, bytes);
                chatBox.AppendText(message);

                подключитьсяToolStripMenuItem.Enabled = false;
                tabControl1.Enabled = true;
                ShowDB.Enabled = true;
                информацияToolStripMenuItem.Enabled = true;
            }
            catch
            {
                MessageBox.Show("Подключение к серверу невозможно.\nВозможные причины: сервер отключен;\nданные для подключения введены неверно.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void updateUI(string s)
        {
            Func<int> del = delegate ()
            {
                chatBox.AppendText(s + Environment.NewLine);
                return 0;
            };
            Invoke(del);
        }

        private void подключитьсяToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form2 f = new Form2();
            f.ShowDialog();
            this.ip = f.ip;
            this.port = f.port;
            connectAsClient();
        }

        private void информацияToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string s = "0";
            byte[] data = System.Text.Encoding.ASCII.GetBytes(s);
            stream.Write(data, 0, data.Length);
            tcpClient.Close();
            updateUI("Disconnect.");

            подключитьсяToolStripMenuItem.Enabled = true;
            информацияToolStripMenuItem.Enabled = false;
            tabControl1.Enabled = false;
            ShowDB.Enabled = false;
        }

        private void ShowDB_Click(object sender, EventArgs e)
        { //тут просто тру блок, вообще его на весь код в любой функции над
            string s = "1";
            byte[] data = System.Text.Encoding.ASCII.GetBytes(s);
            stream.Write(data, 0, data.Length);

            byte[] data2 = new byte[1024];
            int bytes = stream.Read(data2, 0, data2.Length); // получаем количество считанных байтов
            string message = Encoding.ASCII.GetString(data2, 0, bytes);
            chatBox.AppendText(message);
        }

        private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

        private void addr_Click(object sender, EventArgs e)
        {
            try
            {
                if (from_pointBox.Text.Length == 0 || to_pointBox.Text.Length == 0 ||
                    dateTimePicker1.Value.ToShortDateString().Length == 0 || comboBox1.Text.Length == 0 ||
                    comboBox2.Text.Length == 0)
                    throw new System.InvalidOperationException(""); ;

                string s = "2|";
                s += from_pointBox.Text + "|";
                s += to_pointBox.Text + "|";
                s += dateTimePicker1.Value.ToShortDateString() + "|";
                s += comboBox1.Text + ":";
                s += comboBox2.Text + "\0";
                chatBox.AppendText("Send to server: ");
                updateUI(s);

                byte[] data = System.Text.Encoding.ASCII.GetBytes(s);
                stream.Write(data, 0, data.Length);
            }
            catch
            {
                MessageBox.Show("Недопустимы пустые поля.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                if (textBox6.Text.Length == 0)
                    throw new System.InvalidOperationException(""); 

                string Str = textBox6.Text.Trim();
                int Num;
                bool isNum = int.TryParse(Str, out Num);
                if (isNum)
                {
                    string s = "3|";
                    s += textBox6.Text + "\0";
                    chatBox.AppendText("Send to server: ");
                    updateUI(s);

                    byte[] data = System.Text.Encoding.ASCII.GetBytes(s);
                    stream.Write(data, 0, data.Length);
                }
                else
                {
                    throw new System.InvalidOperationException("");
                }         
            }
            catch
            {
                MessageBox.Show("Недопустимое значение поля.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        { //тут проверочка
            string s = "4|";
            s += textBox1.Text + "|";
            s += textBox2.Text + "|";
            s += textBox3.Text + "|";
            s += textBox4.Text + "\0";
            chatBox.AppendText("Send to server: "); 
            updateUI(s);

            byte[] data = System.Text.Encoding.ASCII.GetBytes(s);
            stream.Write(data, 0, data.Length);
        }

        private void button2_Click_1(object sender, EventArgs e)
        {//тут проверочка
            string s = "5|";
            s += textBox5.Text + "\0";
            chatBox.AppendText("Send to server: ");
            updateUI(s);

            byte[] data = System.Text.Encoding.ASCII.GetBytes(s);
            stream.Write(data, 0, data.Length);
        }
    }
}
