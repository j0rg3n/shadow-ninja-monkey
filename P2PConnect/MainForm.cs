using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace P2PConnect
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            string sessionID = DateTime.UtcNow.Ticks.ToString();
            foreach (string clientID in new[] { "A", "B" })
            {
                Client client = new Client(clientID, sessionID);
                client.MessageLogged += client_MessageLogged;
                client.Start();
                clients.Add(client);
            }
        }

        private void client_MessageLogged(string clientID, string message)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new Action<string, string>(client_MessageLogged), clientID, message);
                return;
            }

            if (clientID == "A")
            {
                LogTo(logA, message);
            }
            else
            {
                LogTo(logB, message);
            }
        }

        private static void LogTo(TextBox textBox, string message)
        {
            textBox.AppendText(message + "\r\n");
        }

        private readonly List<Client> clients = new List<Client>();
    }
}
