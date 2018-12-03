using System;
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
    public partial class Form2 : Form
    {
        public string ip;
        public string port;
        
        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            inputIP.Text = "127.0.0.1";
            inputPort.Text = "7770";
        }

        private void connect_Click(object sender, EventArgs e)
        {
            ip = inputIP.Text;
            port = inputPort.Text;
            this.Close();
        }
    }
}
